/*
Authors: Holden Vail, Blake Carlson
Date: 10-09-2024
Class: EECS 678
Assignment: Quash Project
File: execute.cpp
Description: This is the main executable for our project. It runs the quash shell 
*/

#include "built_in_commands.hpp"
#include "jobs.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


using namespace std;

// function declarations
vector<string> tokenize(string &someString);
vector<vector<string> > command_parser(vector<string> tokens);
void executor(vector<vector<string> > user_commands);


int main(){

    // Includes the signal handler for SIGCHLD
    signal(SIGCHLD, sigchild_handler);

    vector<string> tokens;
    string input;

    while (1){
        cout << "[QUASH]$ ";
        getline(cin, input);
        // cout << "input: " << input << endl;

        tokens = tokenize(input);
        /*
        cout << "tokens: "; 
        for (int i = 0; i < tokens.size(); i++){
            cout << tokens[i] << ", ";
        }
        cout << endl;
        */

        vector<vector<string> > user_commands = command_parser(tokens);
        /*
        cout << "Commands: ";
        for (int i = 0; i < user_commands.size(); i++){
            for (int j = 0; j < user_commands[i].size(); j++){
                cout << user_commands[i][j] << " ";
            }
            cout << ", ";
        }
        cout << endl <<"begin execution: "<<endl << endl ;
        */

        executor(user_commands);

    }
    return 0;   
}


vector<string> tokenize(string &someString) {
    // separates the input string into words
    vector<string> tokens;
    string token;
    bool in_quotes = false;

    for (size_t i = 0; i < someString.length(); i++) {
        if (someString[i] == '"') {
            in_quotes = !in_quotes;
        } else if (someString[i] != ' ' || in_quotes) {
            token += someString[i];
        } else {
            if (!token.empty()) {
                // Adding the following block to hopefully implement environment variable functionality
                
                //Adding a check to see if the first character of the token is '$' (environment variable)
                if (token[0] == '$') {
                    char* value = getenv(token.substr(1).c_str());        // Turns the rest of the value into the remainder of the env variable. $HOME -> HOME
                    if (value) {                // If value is not null, set the token to the value, otherwise make it an empty string
                        token = value;
                    } else {
                        token = "";
                    }
                }
                tokens.push_back(token);
                token.clear();
            }
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<vector<string> > command_parser(vector<string> tokens) {
    // parses words and parameters into commands
    vector<vector<string> > user_commands;
    vector<string> currentCommand;

    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "|" || tokens[i] == "<" || tokens[i] == ">" || tokens[i] == ">>") {
            if (!currentCommand.empty()) {
                user_commands.push_back(currentCommand);
                currentCommand.clear();
            }
            currentCommand.push_back(tokens[i]);
            user_commands.push_back(currentCommand);
            currentCommand.clear();
        } else {
            currentCommand.push_back(tokens[i]);
        }
    }
    if (!currentCommand.empty()) {
        user_commands.push_back(currentCommand);
    }
    return user_commands;
}


bool execute_builtin(const vector<string> &command) {
    if (command.empty()) return false;

    vector<string> args(command.begin() + 1, command.end());

    const string &cmd = command[0];
    if (cmd == "cd") {
        cd(args);
        return true;
    } 
    else if (cmd == "pwd") {
        pwd();
        return true;
    } 
    else if (cmd == "echo") {
        echo(args);
        return true;
    }
    else if (cmd == "quit" || cmd == "exit") {
        exit(0);
        return true;
    }
    else if (cmd == "export") {
        my_export(args);
        return true;
    }
    // Added jobs as a builtin command that lists the jobs when the jobs command is called
    else if (cmd == "jobs") {
        list_jobs();
        return true;
    }
    return false;
}


void executor(vector<vector<string> > user_commands) {

    // inits
    int pipe_fds[2];
    int prev_pipe_read_end = -1;

    // executes the given commands
    for (size_t i = 0; i < user_commands.size(); i++) {
        if (user_commands[i].empty()) {
            continue;
        }

        // Check and execute built-in commands
        if (execute_builtin(user_commands[i])) {
            continue;
        }

        // NOTE: ADDING FUNCTIONALITY FOR BACKGROUND JOB HANDLING! DELETE THE FOLLOWING BLOCK IF IT BREAKS FUNCTIONALITY!
        bool is_background_job = false;
        if (user_commands[i].back() == "&") {               // Checks if the end of the command is an & character. If so, it is a background job
            is_background_job = true;
            user_commands[i].pop_back();                // Removes the & character from the command now that it has been identified as a background job
        }

        // Checks if next command is a pipe
        bool is_pipe = (i < user_commands.size() - 1 && user_commands[i + 1].size() == 1 && user_commands[i + 1][0] == "|");

        // Create a pipe if the next command is a pipe
        if (is_pipe) {
            if (pipe(pipe_fds) == -1) {
                cerr << "Pipe creation failed" << endl;
                return;
            }
        }

        pid_t pid = fork();
        if (pid == -1) {
            cerr << "Fork Failed" << endl;
            continue;
        } else if (pid == 0) {
            // Child process

            // If not the first command and the previous command was a pipe, read from the previous pipe
            if (prev_pipe_read_end != -1) {
                dup2(prev_pipe_read_end, STDIN_FILENO);
                close(prev_pipe_read_end);
            }

            // If the next command is a pipe, write to the pipe
            if (is_pipe) {
                close(pipe_fds[0]); // Close unused read end
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }

            vector<string> args; // init args
            for (size_t j = 0; j < user_commands[i].size(); j++) { // for all strings in command
                args.push_back(user_commands[i][j]); // add strings from command to args
            }

            // Convert vector<string> to vector<char*>
            vector<char*> c_args;
            for (size_t j = 0; j < args.size(); j++) {
                c_args.push_back(const_cast<char*>(args[j].c_str()));
            }
            c_args.push_back(nullptr); // execvp expects a null-terminated array

            // Execute the command
            if (execvp(c_args[0], c_args.data()) == -1) {
                cerr << "Command execution failed: " << user_commands[i][0] << endl;
                exit(EXIT_FAILURE);
            }
        } 
        else {
        // Parent process

        // Close the previous pipe read end if it exists
        if (prev_pipe_read_end != -1) {
            close(prev_pipe_read_end);
        }

        // If the next command is a pipe, update the previous pipe read end
        if (is_pipe) {
            close(pipe_fds[1]); // Close unused write end
            prev_pipe_read_end = pipe_fds[0];
        } else {
            prev_pipe_read_end = -1; // Reset if no pipe
        }

        /*
        This is commented out since the functionality gets replaced on whether the job is in the background or foreground. 
        Code is being kept here incase the new implementation does not work.
        // Wait for the child process to finish
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            cerr << "Waitpid failed" << endl;
        }
        */

        if (is_background_job) {
            add_job(pid, user_commands[i][0]);
            //cout << "Job added: [" << pid << "]" << endl;
        } else {
            int job_status;
            if (waitpid(pid, &job_status, 0) == -1) {
                cerr << "Waitpid failed" << endl;
            }
        }

        // Skip the pipe character in the next iteration
        if (is_pipe) {
            i++;
        }
        }
    }
}
