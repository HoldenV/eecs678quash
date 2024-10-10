/*
Authors: Holden Vail, Blake Carlson
Date: 10-09-2024
Class: EECS 678
Assignment: Quash Project
Description: These functions help abstract quashes use of memory to guarantee the program is free from memory leaks. 
Reference: https://zarrar1607.github.io/KUEECS678-Fall2024/projects/quash/doc/html/memory__pool_8h.html#a1ad04a8d891de237fc5d91191c569777
*/
 
 #ifndef SRC_PARSING_MEMORY_POOL_H
 #define SRC_PARSING_MEMORY_POOL_H
 
 #include <stdlib.h>

 // these are function declarations. They still need to be implemented in this file.
 
 void initialize_memory_pool(size_t size);
 
 void* memory_pool_alloc(size_t size);
 
 void destroy_memory_pool();
 
 char* memory_pool_strdup(const char* str);
 
 #endif

// implement the above functions here:

