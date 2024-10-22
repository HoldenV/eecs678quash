/*
Authors: Holden Vail, Blake Carlson
Date: 10-09-2024
Class: EECS 678
Assignment: Quash Project
File: built_in_commands.hpp
Description: This is the main header file that contain definitions for all the custom-built built-in funcitons.
*/

#include <string>
#include <vector>
#include <iostream>

using namespace std; // must be commented out when prototyps are implemented

void my_export();
void cd(const vector<string>& args);
void pwd();
void quit();
void exit();
void jobs();

void echo(vector<string>& args) {
    using namespace std;
    bool newline = true;
    size_t start = 0;           // inits

    if (!args.empty() && args[0] == "-n") {     // for case of -n to supress newlines
        newline = false;
        start = 1;
    }

    for (size_t i = start; i < args.size(); i++) {          // for each arg
        for (size_t j = 0; j < args[i].length(); j++) {     // for each char
            if (args[i][j] == '\\' && j + 1 < args[i].length()) { // to validly detect escape sequences (cpp uses \\ to indicate '\')
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
