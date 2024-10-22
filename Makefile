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
source = execute.cpp

run:
	./$(target)


all:
	$(compiler) $(flags) $(target) src/$(source)

mac:
	$(compiler) $(paths) $(flags) $(target) src/$(source)

clean:
	rm -f $(target)

test:
# Do all the actions define in the requirements



