/*
Authors: Holden Vail, Blake Carlson
Date: 10-09-2024
Class: EECS 678
Assignment: Quash Project
Reference: https://zarrar1607.github.io/KUEECS678-Fall2024/projects/quash/doc/html/parsing__interface_8h.html
Description: This provides an interface between C and the parser. 
*/
 
 #ifndef SRC_PARSING_PARSING_INTERFACE_H
 #define SRC_PARSING_PARSING_INTERFACE_H
 
 #include <stdbool.h>
 
 #include "command.h"
 #include "deque.h"
 #include "quash.h"
 
 typedef struct Redirect {
   char* in;    
   char* out;   
   bool append; 
 } Redirect;
 
 IMPLEMENT_DEQUE_STRUCT(CmdStrs, char*);
 
 IMPLEMENT_DEQUE_STRUCT(Cmds, CommandHolder);
 
 PROTOTYPE_DEQUE(CmdStrs, char*);
 PROTOTYPE_DEQUE(Cmds, CommandHolder);
 /*************************************************************
  * Functions used by the parser
  *************************************************************/
 Redirect mk_redirect(char* in, char* out, bool append);
 
 char* interpret_complex_string_token(const char* str);
 
 
 /*************************************************************
  * Functions used by the parser
  *************************************************************/
 CommandHolder* parse(QuashState* state);
 
 void destroy_parser();
 
 #endif

