/*
Author: Blake Carlson, Holden Vail
Date: 10-22-2024
Class: EECS 678
Assignment: Quash Project
File: builtInCommands.h
Description: This is the header file for builtInCommands.cpp which contains the function headers for the built-in commands liike pwd, cd, etc.
*/

#ifndef BUILTINCOMMANDS_H
#define BUILTINCOMMANDS_H

#include <string>
#include <vector>

void pwd();
void cd(const std::vector<std::string>& args);
void echo(std::vector<std::string>& args);
#endif
