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
#include <fcntl.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>


using namespace std;

// function declarations
vector<string> tokenize(string &someString);
vector<vector<string> > command_parser(vector<string> tokens);
void executor(vector<vector<string> > user_commands);


int main(){
    signal(SIGCHLD, sigchild_handler); // include signal handler

    vector<string> tokens;
    string input;

    while (1){
        cout << "[QUASH]$ ";            // printing and input for user
        cout.flush();
        if (!getline(cin, input)) {
            break;
        }

        tokens = tokenize(input);                                           // tokenizes input string
        vector<vector<string> > user_commands = command_parser(tokens);     // separates tokens into commands
        executor(user_commands);                                            // executes the commands
    }
    return 0;   
}


vector<string> tokenize(string &someString) {
    // separates the input string into words
    vector<string> tokens;
    string token;
    bool in_quotes = false;

    for (size_t i = 0; i < someString.length(); i++) { // for each char in the string
        if (someString[i] == '"') { // if in quote detected, change quotes variable
            in_quotes = !in_quotes;
        } else if (someString[i] != ' ' || in_quotes) { // if not whitespace or in quotes, continue appending
            token += someString[i];
        } else {
            if (!token.empty()) {
                if (token[0] == '$') { // if $ is detected, 
                    char* value = getenv(token.substr(1).c_str()); // tries to get environment variable of token 
                    if (value) {            // if getenv returns success
                        token = value;
                    } else {        // else, set token of non-existent environment variable to null string
                        token = "";
                    }
                }
                tokens.push_back(token);  // at this point token should encounter whitespace. Push token to tokens
                token.clear();
            }
        }
    }
    if (!token.empty()) {               // ensures the final token is pushed to tokens
        tokens.push_back(token);
    }
    return tokens;
}


vector<vector<string> > command_parser(vector<string> tokens) {
    vector<vector<string> > user_commands;
    vector<string> currentCommand;

    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "|") {
            if (!currentCommand.empty()) {
                user_commands.push_back(currentCommand);
                currentCommand.clear();
            }
            // Don't add the pipe symbol as a separate command
        }
        else if (tokens[i] == "<" || tokens[i] == ">" || tokens[i] == ">>") {
            if (!currentCommand.empty()) {
                user_commands.push_back(currentCommand);
                currentCommand.clear();
            }
            currentCommand.push_back(tokens[i]);
        }
        else {
            currentCommand.push_back(tokens[i]);
        }
    }
    if (!currentCommand.empty()) {
        user_commands.push_back(currentCommand);
    }

    return user_commands;
}


bool execute_builtin(vector<string> &command) {
    if (command.empty()){
        return false;
    }

    vector<string> args(command.begin() + 1, command.end());
    const string &cmd = command[0];

    if (cmd == "cd") {
        cd(args);
        print_completed_jobs();
        return true;
    } 
    else if (cmd == "pwd") {
        pwd();
        print_completed_jobs();
        return true;
    } 
    else if (cmd == "echo") {
        echo(args);
        print_completed_jobs();
        return true;
    }
    else if (cmd == "quit" || cmd == "exit") {
        exit(0);
        print_completed_jobs();
        return true;
    }
    else if (cmd == "export") {
        my_export(args);
        print_completed_jobs();
        return true;
    }
    else if (cmd == "jobs") {
        list_jobs();
        return true;
    }
    return false;
}


void executor(vector<vector<string> > user_commands) {
    int pipe_fds[2];                   // init file descriptor arrays
    int prev_pipe_read = -1;  // Store the read end of the previous pipe

    for (size_t i = 0; i < user_commands.size(); i++) { // for each command 
        int saved_stdout = dup(STDOUT_FILENO); // Save the original stdout file descriptor
        int saved_stdin = dup(STDIN_FILENO);   // Save stdin as well

        // Background proccess handling
        bool is_background_job = false;
        if (!user_commands[i].empty() && user_commands[i].back() == "&") {
            is_background_job = true;
            user_commands[i].pop_back();
        }

        if (user_commands[i].empty()) { // continue on empty commands
            continue;
        }

        // Check if this command pipes to the next one
        bool pipes_to_next = (i < user_commands.size() - 1);

        // Create new pipe if needed
        if (pipes_to_next) {
            if (pipe(pipe_fds) == -1) {
                cerr << "Pipe creation failed" << endl;
                return;
            }
        }

        // File redirection
        if (i + 1 < user_commands.size() && user_commands[i + 1][0] == ">") { // if the next command begins with >
            if (user_commands[i + 1].size() < 2) {
                cerr << "No file specified for redirection" << endl;
                return;
            }
            int fd = open(user_commands[i + 1][1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644); // open new file with filename
            if (fd == -1) {
                cerr << "Failed to open file for writing: " << user_commands[i + 1][1] << endl; // or error
                return;
            }
            dup2(fd, STDOUT_FILENO); // set stdout to the new file
            close(fd);
            user_commands.erase(user_commands.begin() + i + 1); // erase the command
        } 

        if (i + 1 < user_commands.size() && user_commands[i + 1][0] == ">>") { // if the next command begins with >>
            if (user_commands[i + 1].size() < 2) {
                cerr << "No file specified for redirection" << endl;
                return;
            }
            int fd = open(user_commands[i + 1][1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644); // open existing file with filename
            if (fd == -1) {
                cerr << "Failed to open file for appending: " << user_commands[i + 1][1] << endl; // or error
                return;
            }
            dup2(fd, STDOUT_FILENO); // set stdout to the file
            close(fd);
            user_commands.erase(user_commands.begin() + i + 1); // erase the command
        } 

        if (i + 1 < user_commands.size() && user_commands[i + 1][0] == "<") { // if the next command begins with <
            if (user_commands[i + 1].size() < 2) {
                cerr << "No file specified for redirection" << endl;
                return;
            }
            int fd = open(user_commands[i + 1][1].c_str(), O_RDONLY); // open file for reading with filename
            if (fd == -1) {
                cerr << "Failed to open file for reading: " << user_commands[i + 1][1] << endl;
                return;
            }
            dup2(fd, STDIN_FILENO); // set stdin to file
            close(fd);
            user_commands.erase(user_commands.begin() + i + 1);
        }

        // check and execute any custom built-ins
        if (execute_builtin(user_commands[i])) { 
            if (prev_pipe_read != -1) {
                close(prev_pipe_read);
            }
            if (pipes_to_next) {
                close(pipe_fds[0]);
                close(pipe_fds[1]);
            }
            continue;
        }

        // execution of all other functions
        pid_t pid = fork();
        if (pid == -1) {
            cerr << "Fork Failed" << endl;
            exit(EXIT_FAILURE);
        } 
        else if (pid == 0) {
            //child process block

            // Set up input from previous pipe if it exists
            if (prev_pipe_read != -1) {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }

            // Set up output to next pipe if it exists
            if (pipes_to_next) {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[0]);
                close(pipe_fds[1]);
            }

            vector<char*> c_args;
            for (size_t j = 0; j < user_commands[i].size(); j++) { // for each string in command
                c_args.push_back(const_cast<char*>(user_commands[i][j].c_str())); // push a char* to new vector c_args
            }
            c_args.push_back(nullptr); // add a null pointer for execvp

            if (execvp(c_args[0], c_args.data()) == -1) { // execute or error
                cerr << "Command execution failed: " << user_commands[i][0] << endl;
                exit(EXIT_FAILURE);
            }
        } 
        else {
            // Parent process

            // Close previous pipe read end if it exists
            if (prev_pipe_read != -1) {
                close(prev_pipe_read);
            }

            // Close write end of current pipe and save read end for next iteration
            if (pipes_to_next) {
                close(pipe_fds[1]);
                prev_pipe_read = pipe_fds[0];
            }
            else {
                prev_pipe_read = -1;
            }

            if (is_background_job) {
                // Handle background job
                string full_command;
                for (size_t j = 0; j < user_commands[i].size(); j++) {
                    if (j > 0) full_command += " ";
                    full_command += user_commands[i][j];
                }
                add_job(pid, full_command);
            } 
            else {
                // Handle foreground job
                int status;
                waitpid(pid, &status, 0);
            }

            // Restore stdin and stdout
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);
        }
        // After the foreground job finishes, check for completed background jobs
        print_completed_jobs();
    }
}
