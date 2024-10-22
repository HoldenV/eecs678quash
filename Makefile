# Authors: Holden Vail, Blake Carlson
# Date: 10-09-2024
# Class: EECS 678
# Assignment: Quash Project
# File: Makefile
# Description: Makefile for quash shell


compiler = g++
flags = -Wall -g -o
paths = -I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1 
target = execute
source = src/execute.cpp src/built_in_commands.cpp

run:
	./$(target)


all:
	$(compiler) $(flags) $(target) $(source)

mac:
	$(compiler) $(paths) $(flags) $(target) $(source)

clean:
	rm -f $(target)

test:
# Do all the actions define in the requirements
