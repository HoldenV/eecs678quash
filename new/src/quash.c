/*
Authors: Holden Vail, Blake Carlson
Date: 10-09-2024
Class: EECS 678
Assignment: Quash Project
File: quash.c
Description: This is the interfacing function for quash. It collects input and calls to execute the commands
*/
 
 #include "execute.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

 #define MAX_TOKENS 100

int main(){
    while(1){
        // say hello
        printf("[QUASH]$ ");

        // collect input from line
        char* input = NULL;
        size_t len = 0;
        ssize_t read;
        read = getline(&input, &len, stdin);
        if (read == -1){
            perror("input collection error");
            free(input);
            return 1;
        }

        // Tokenize the input
        char* tokens[MAX_TOKENS];
        int tokenCount = 0;
        char* token = strtok(input, " ");
        while (token != NULL && tokenCount < MAX_TOKENS){
            tokens[tokenCount++] = token;
            token = strtok(NULL, " ");
        }

        // Handle empty input
        if (tokenCount == 0){
            free(input);
            continue;
        }

        // Execute the command or exit
        if (excute_command(tokens)){
            free(input);
            return 0;
        }

        free(input);
    }
    return 0;
}
