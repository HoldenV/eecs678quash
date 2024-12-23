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
#include <cstring>

using namespace std;

void pwd() {
    char cwd[512];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        cout << cwd << endl;
    } else {
        perror("getcwd");
    }
}


void cd(const vector<string> &args) {
    char* default_path = getenv("HOME");
    const char* path;

    
    if (args.empty()) {             // If there are no arguments, move to the default directory (home)
        path = default_path;
    } else if (args[0] == "-") {                // If "cd -" is called, set the path equal to the value of the OLDPWD environment variable.
        path = getenv("OLDPWD");
        if (path == nullptr) {  // If OLDPWD is not set (Should not happen) an error is printed
            cerr << "cd: OLDPWD not set" << endl;
            return;             // Return since it cant do anything if there is no old working directory.
        }
    } else if (args[0] == "~") { // If "cd ~" is called, return to the default directory (home)
        path = default_path;
    } else {
        path = args[0].c_str();  // Otherwise, change the directory to the directory given by making it a c string
    }

    char current_directory[1024];               // Set a buffer to store the current directory

    if (getcwd(current_directory, sizeof(current_directory)) != nullptr) {
        setenv("OLDPWD", current_directory, 1);  // Set the OLDPWD env variable to the current directory
    } else {
        perror("getcwd");  // If it fails print an error and return since it cant do anything.
        return;
    }

    if (chdir(path) != 0) {  // Attempt to change the directory. If it fails return an error.
        perror("chdir");
        return;  // If the directory cannot be changed properly, just return and exit
    }

    if (getcwd(current_directory, sizeof(current_directory)) != nullptr) {
        setenv("PWD", current_directory, 1);  // Update the PWD environment variable as the new current directory
    } else {
        perror("getcwd");  // If it fails handle the error
    }

    
    if (args.size() > 0 && args[0] == "-") {                // If "cd -" is called, the new directory is printed
        cout << current_directory << endl;  // Prints the new directory
    }

    cout.flush();  // Make sure that all output 
}



void echo(vector<string> &args) {
    bool newline = true;
    size_t start = 0;           // inits

    if (!args.empty() && args[0] == "-n") {     // for case of -n to supress newlines
        newline = false;
        start = 1;
    }

    for (size_t i = start; i < args.size(); i++) {          // for each string
        for (size_t j = 0; j < args[i].length(); j++) {     // for each char
            if (args[i][j] == '\\' && j + 1 < args[i].length()) {
                switch (args[i][j + 1]) {
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
            } else if (args[i][j] == '$') {             // Checks for an environment variable
                string var_name = args[i].substr(j+1);
                char* value = getenv(var_name.c_str());
                if (value) {
                    cout << value;
                } else {
                    cout << "";
                }
                break;
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

void my_export(const vector<string> &args){
    if (args.size() != 1) {            // The command should only accept 2 arguments
        cerr << "Error: invalid input. should be VAR=value" << endl;
        return;
    }
    size_t equals_position = args[0].find('=');            // Make a size_t variable that stores the position on the equal sign


    if (equals_position == std::string::npos) {  // If no '=' is found
        cerr << "Error: invalid input. should be VAR=value" << endl;
        return;
    }

    string var_name = args[0].substr(0, equals_position);            // The name of the environment variable is whats on the left of the equal sign
    string var_value = args[0].substr(equals_position + 1);            // The value of the environment variable is whats on the right of the equal sign

    int result = setenv(var_name.c_str(), var_value.c_str(), 1);            // Sets/Modifies the environment variable. The 1 says to overwrite the variable if it already exists
    if (result != 0) {                // If it succeeds in modifying the value then result should be 0. If its not 0 that means it fails and an error is printed
        cerr << "Error: failed to set the environment variable" << endl;
    }
}
