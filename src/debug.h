/*
Authors: Holden Vail, Blake Carlson
Date: 10-09-2024
Class: EECS 678
Assignment: Quash Project
Description: 
*/
 
 #include <stdio.h>
 
 #ifdef DEBUG
 #  define PRINT_DEBUG(fmt, ...) \
     fprintf(stderr, "DEBUG: %s(), %s:%d: " fmt,         \
         __func__, __FILE__, __LINE__, ##__VA_ARGS__)
 #  define IFDEBUG(x) x
 #else
 // Does nothing
 #  define PRINT_DEBUG(fmt, ...)
 // Does nothing
 #  define IFDEBUG(x)
 #endif
 