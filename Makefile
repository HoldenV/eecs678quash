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
	@echo "ls"
	@echo "pwd"
	@echo "echo Hello"
	@echo "echo \\n"
    
	@echo "Running Tier 2 tests..."
	@echo "ls -a /"
	@echo "echo Hello World"
	@echo "cd /"
	@echo "export TEST_VAR=123"
	@echo "echo \$TEST_VAR"
	@echo "du -H \$PWD/.."
	@echo "echo \\n"
    
	@echo "Running Tier 3 tests..."
	@echo "jobs"
	@echo "kill 1"
	@echo "find . -type f | grep '*.cpp'"
# @echo "cat < src/built_in_commands.cpp"
# @echo "echo Hello > test_output.txt"
	@echo "cat test_output.txt"
	@echo "sleep 5 &"
	@echo "echo \\n"
    
	@echo "Running Tier 4 tests (extra credit)..."
# @echo "cat < src/built_in_commands.cpp | grep -o include | cat > test_output.txt"
	@echo "cat test_output.txt"
# @echo "echo Hello >> test_output.txt"
	@echo "cat test_output.txt"
	@echo "echo \\n"