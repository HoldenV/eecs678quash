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
vector<vector<string>> commandParser(vector<string> tokens);
void executor(vector<vector<string>> userCommands);


int main(){
    vector<string> tokens;

    string input;

    while (1){
        cout << "[QUASH]$ ";
        getline(cin, input);
        tokens = tokenize(input);
        vector<vector<string>> userCommands = commandParser(tokens);
    }

    return 0;   
}


vector<string> tokenize(string &someString){
    // separates the input string in to words
    vector<string> tokens;
    string token;
    for (int i = 0; i < someString.length(); i++)
        if (someString[i] != ' '){
            token += (someString[i]);
        }
        else{
            tokens.push_back(token);
            token.clear();
        }
    return tokens;
}


vector<vector<string>> commandParser(vector<string> tokens){
    // parses words and parameters into commands
    vector<vector<string>> userCommands;
    vector<string> currentCommand;

    for (int i = 0; i < tokens.size(); i++){                                            // for each token, if a token is special
        if (tokens[i] == "|" || tokens[i] == "<" || tokens[i] == ">" || tokens[i] = ">>"){
            if (!currentCommand.empty()){                                               // and there is a command being built already
                userCommands.push_back(currentCommand);                                 // push the previous command to commands vector
                currentCommand.clear();                                                 // clear the current command vector
            }
            userCommands.push_back(vector<string>{tokens[i]});                          // push the special character to the command vector on its own
        }
        else{
            currentCommand.push_back(tokens[i]);                                        // if no special characters are incurred then just continue building current command
        }
    }
    if (!currentCommand.empty()){                                                       // if a command was still being built, push it to commands vector
        userCommands.push_back(currentCommand);
    }
    return userCommands;                                                                // return command vector
}


void executor(vector<vector<string>> userCommands){
    // executes the given commands
}
