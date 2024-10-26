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
source = src/execute.cpp src/built_in_commands.cpp src/jobs.cpp

run:
	./$(target)

all:
	$(compiler) $(flags) $(target) $(source)

mac:
	$(compiler) $(paths) $(flags) $(target) $(source)

clean:
	rm -f $(target)
	rm -f test_output.txt

test:
	@echo "Running Tier 1 tests..."
	@echo "ls: " 
	@echo "ls" | ./$(target)
	@echo "\n"

	@echo "pwd:"
	@echo "pwd" | ./$(target)
	@echo "\n"

	@echo "echo Hello:"
	@echo "echo Hello" | ./$(target)
	@echo "\n"
    
	@echo "Running Tier 2 tests..."
	@echo "ls -a /:"
	@echo "ls -a /" | ./$(target)
	@echo "\n"

	@echo "echo Hello World:"
	@echo "echo Hello World" | ./$(target)
	@echo "\n"

	@echo "cd src:"
	@echo "cd src" | ./$(target)
	@echo "\n"

	@echo "export TEST_VAR=123:"
	@echo "export TEST_VAR=123" | ./$(target)
	@echo "\n"
 
	@echo "Running Tier 3 tests..."
	@echo "jobs:"
	@echo "jobs" | ./$(target)
	@echo "\n"

	@echo "kill:"
	@echo "kill" | ./$(target)
	@echo "\n"

	@echo "echo Hello > test_output.txt:"
	@echo "echo Hello > test_output.txt" | ./$(target)
	@echo "\n"

	@echo "ls | grep .txt"
	@echo "ls | grep .txt" | ./$(target)
	@echo "\n"

	@echo "cat < test_output.txt:"
	@echo "cat < test_output.txt" | ./$(target)
	@echo "\n"

	@echo "cat test_output.txt:"
	@echo "cat test_output.txt" | ./$(target)
	@echo "\n"

	@echo "sleep 5 &:"
	@echo "sleep 5 &" | ./$(target)
	@echo "\n"
