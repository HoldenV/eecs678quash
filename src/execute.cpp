/*
Authors: Holden Vail, Blake Carlson
Date: 10-09-2024
Class: EECS 678
Assignment: Quash Project
File: execute.cpp
Description: This is the main executable for our project. It runs the quash shell 
*/

#include <string>
#include <iostream>
#include <vector>

using namespace std;

// function declarations
vector<string> tokenize(string &someString);
vector<vector<string> > commandParser(vector<string> tokens);
void executor(vector<vector<string> > userCommands);


int main(){
    vector<string> tokens;
    string input;

    while (1){
        cout << "[QUASH]$ ";
        getline(cin, input);
        tokens = tokenize(input);
        vector<vector<string> > userCommands = commandParser(tokens);

        // Print out the commands
        for (int i = 0; i < userCommands.size(); i++) {
            for (int j = 0; j < userCommands[i].size(); j++) {
                cout << userCommands[i][j] << " ";
            }
            cout << endl;
        }
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

vector<vector<string> > commandParser(vector<string> tokens) {
    // parses words and parameters into commands
    vector<vector<string> > userCommands;
    vector<string> currentCommand;

    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "|" || tokens[i] == "<" || tokens[i] == ">" || tokens[i] == ">>") {
            if (!currentCommand.empty()) {
                userCommands.push_back(currentCommand);
                currentCommand.clear();
            }
            currentCommand.push_back(tokens[i]);
            userCommands.push_back(currentCommand);
            currentCommand.clear();
        } else {
            currentCommand.push_back(tokens[i]);
        }
    }
    if (!currentCommand.empty()) {
        userCommands.push_back(currentCommand);
    }
    return userCommands;
}

void executor(vector<vector<string> > userCommands) {
    // executes the given commands
    // Implementation needed
}
