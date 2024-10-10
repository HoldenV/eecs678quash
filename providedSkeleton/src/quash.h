/*
Authors: Holden Vail, Blake Carlson
Date: 10-09-2024
Class: EECS 678
Assignment: Quash Project
Description: 
*/
 
 #ifndef SRC_QUASH_H
 #define SRC_QUASH_H
 
 #include <stdbool.h>
 #include <stdio.h>
 #include <stdlib.h>
 
 #include "execute.h"
 
 typedef struct QuashState {
   bool running;     
   bool is_a_tty;    
   char* parsed_str; 
 } QuashState;
 
 QuashState initial_state();
 
 char* get_command_string();
 
 bool is_running();
 
 void end_main_loop();
 
 #endif // QUASH_H
 