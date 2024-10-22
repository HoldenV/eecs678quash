/*
Authors: Holden Vail, Blake Carlson
Date: 10-09-2024
Class: EECS 678
Assignment: Quash Project
File: execute.cpp
Description: This is the main executable for our project. It runs the quash shell 
*/

#include "built_in_commands.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>

using namespace std;

// function declarations
vector<string> tokenize(string &someString);
vector<vector<string> > command_parser(vector<string> tokens);
void executor(vector<vector<string> > user_commands);


int main(){
    vector<string> tokens;
    string input;

    while (1){
        cout << "[QUASH]$ ";
        getline(cin, input);
        cout << "input: " << input << endl;

        tokens = tokenize(input);
        cout << "tokens: "; 
        for (int i = 0; i < tokens.size(); i++){
            cout << tokens[i] << ", ";
        }
        cout << endl;

        vector<vector<string> > user_commands = command_parser(tokens);
        cout << "Commands: ";
        for (int i = 0; i < user_commands.size(); i++){
            for (int j = 0; j < user_commands[i].size(); j++){
                cout << user_commands[i][j] << " ";
            }
            cout << ", ";
        }
        cout << endl <<"begin execution: "<<endl << endl ;

        executor(user_commands);

    }
    return 0;   
}


vector<string> tokenize(string &someString) {
    // separates the input string into words
    vector<string> tokens;
    string token;
    bool in_quotes = false;

    for (int i = 0; i < someString.length(); i++) {
        if (someString[i] == '"') {
            in_quotes = !in_quotes;
        } else if (someString[i] != ' ' || in_quotes) {
            token += someString[i];
        } else {
            if (!token.empty()) {
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


void executor(vector<vector<string> > user_commands) {
    // executes the given commands
    for (int i = 0; i < user_commands.size(); i++) {
        if (user_commands[i].empty()) {
            continue;
        }

        pid_t pid = fork();
        if (pid == -1) {
            cerr << "Fork Failed" << endl;
            continue;
        } else if (pid == 0) {
            // Child process
            vector<string> args; // init args
            for (int j = 0; j < user_commands[i].size(); j++) { // for all strings in command
                args.push_back(user_commands[i][j]); // add strings from command to args
            }

            // Convert vector<string> to vector<char*>
            vector<char*> c_args;
            for (int j = 0; j < args.size(); j++) {
                c_args.push_back(const_cast<char*>(args[j].c_str()));
            }
            c_args.push_back(nullptr); // execvp expects a null-terminated array

            // Execute the command
            if (execvp(c_args[0], c_args.data()) == -1) {
                cerr << "Command execution failed: " << user_commands[i][0] << endl;
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;
            if (waitpid(pid, &status, 0) == -1) {
                cerr << "Waitpid failed" << endl;
            }
        }
    }
}
