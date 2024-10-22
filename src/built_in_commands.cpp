/*
Author: Blake Carlson
Date: 10-22-2024
Class: EECS 678
Assignment: Quash Project
File: builtInCommands.cpp
Description: File containing the implementation for the built-in commands liike pwd, cd, etc.
*/

#include "built_in_commands.h"      // Include the header file

#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <unistd.h>

void pwd() {              // Implements the pwd function. Prints the current working directory
  using namespace std;
  char cwd[512];
  if (getcwd(cwd, sizeof(cwd)) != nullptr) {
    cout << cwd << endl;
  }
  else {
    cerr << "Error getting current working directory" << endl;
  }
}


void cd(const std::vector<std::string>& args) {              // Implements the change directory function
    using namespace std;
    cout << "testprint";
    if (args.empty()) {
        cerr << "cd: missing argument" << endl;
        return;
    }

    const char* path = args[0].c_str();
    if (chdir(path) != 0) {
        perror("cd");
    }
}


void echo(std::vector<std::string>& args) {
    using namespace std;
    cout << "testprint";
    bool newline = true;
    size_t start = 0;           // inits

    if (!args.empty() && args[0] == "-n") {     // for case of -n to supress newlines
        newline = false;
        start = 1;
    }

    for (size_t i = start; i < args.size(); i++) {          // for each arg
        for (size_t j = 0; j < args[i].length(); j++) {     // for each char
            if (args[i][j] == '\\' && j + 1 < args[i].length()) { // to validly detect escape sequences (c++ uses \\ to indicate '\')
                switch (args[i][j + 1]) {
                    case 'n':
                        cout << '\n';
                        j++;
                        break;
                    case 't':
                        cout << '\t';
                        j++;
                        break;
                    case '\\':
                        cout << '\\';
                        j++;
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
