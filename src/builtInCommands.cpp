#include "builtInCommands.h"      // Include the header file

#include <cstdlib>
#include <iostream>
#include <signal.h>

void pwd() {              // Implements the pwd function. Prints the current working directory
  char cwd[512];
  getcwd(cwd, sizeof(cwd));          // Gets the current working directory
  cout << cwd << endl;
}

void cd(const vector<string>& args) {              // Implements the change directory function
  if (args.size() < 2) {          // Checks if an argument has been entered. If no argument was given, then it has no directory to move to
    cerr << "Missing directory to change to." << endl;          // Prints the error if no argument is given
  }
  else {
    chdir(args[1]);          // If an argument is given, change the current directory to it
  } 
}

void echo(vector<string>& args) {            // Implements the echo function. Writes arguments to the standard output
  for (int i = 1; i < args.size(); i++) {          // Iterates through each argument after the echo function call
    cout << args[i] << " ";          // Prints the arguments to the echo call separated by a space
  }
  cout << endl;
}


