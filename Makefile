# Authors: Holden Vail, Blake Carlson
# Date: 10-09-2024
# Class: EECS 678
# Assignment: Quash Project
# File: Makefile
# Description: Makefile for quash shell


compiler = clang++
flags = -Wall -g -o
target = execute
source = execute.cpp

run:
	./$(target)


all:
	$(compiler) $(flags) $(target) $(source)

clean:
	rm -f $(target)

test:
# Do all the actions define in the requirements



