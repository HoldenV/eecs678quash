/*
Author: Blake Carlson
Date: 10-22-2024
Class: EECS 678
Assignment: Quash Project
File: builtInCommands.cpp
Description: File containing the implementation for the built-in commands liike pwd, cd, etc.
*/

#include "built_in_commands.hpp"      // Include the header file

#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

void pwd() {
    char cwd[512];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        cout << cwd << endl;
    } else {
        perror("getcwd");
    }
}


void cd(const std::vector<std::string> &args) {
    const char* path = args.empty() ? ".." : args[0].c_str();

    if (chdir(path) != 0) {
        perror("chdir");
    }
}


void echo(std::vector<std::string> &args) {
    bool newline = true;
    size_t start = 0;           // inits

    if (!args.empty() && args[0] == "-n") {     // for case of -n to supress newlines
        newline = false;
        start = 1;
    }

    for (size_t i = start; i < args.size(); i++) {          // for each string
        for (size_t j = 0; j < args[i].length(); j++) {     // for each char
            if (args[i][j] == '\\' && j + 2 < args[i].length()) {
                switch (args[i][j + 2]) {
                    case 'n':
                        cout << '\n';
                        j+=2;
                        break;
                    case 't':
                        cout << '\t';
                        j+=2;
                        break;
                    case '\\':
                        cout << '\\';
                        j+=2;
                        break;
                    default:
                        cout << args[i][j];
                        break;
                }
            } else {
                cout << args[i][j];
            }
        }
        if (i < args.size() - 1) {
            cout << " ";
        }
    }

    if (newline) {
        cout << endl;
    }
}

void my_export(const std::vector<std::string> &args){
    if (args.size() != 2) {            // The command should only accept 2 arguments
        cerr << "Error: invalid input. should be VAR=value" << endl;
    }
    size_t equals_position = args[1].find('=');            // Make a size_t variable that stores the position on the equal sign

    string var_name = args[1].substr(0, equals_position);            // The name of the environment variable is whats on the left of the equal sign
    string var_value = args[1].substr(equals_position + 1);            // The value of the environment variable is whats on the right of the equal sign

    int result = setenv(var_name.c_str(), var_value.c_str(), 1);            // Sets/Modifies the environment variable. The 1 says to overwrite the variable if it already exists
    if (result != 0) {                // If it succeeds in modifying the value then result should be 0. If its not 0 that means it fails and an error is printed
        cerr << "Error: failed to set the environment variable" << endl;
}