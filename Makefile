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
	@echo "ls" | ./$(target)
	@echo "pwd" | ./$(target)
	@echo "echo Hello" | ./$(target)
	@echo "echo \\n" | ./$(target)
    
	@echo "Running Tier 2 tests..."
	@echo "ls -a /" | ./$(target)
	@echo "echo Hello World" | ./$(target)
	@echo "cd /" | ./$(target)
	@echo "export TEST_VAR=123" | ./$(target)
	@echo "echo \$TEST_VAR" | ./$(target)
	@echo "du -H \$PWD/.." | ./$(target)
	@echo "echo \\n" | ./$(target)
    
	@echo "Running Tier 3 tests..."
	@echo "jobs" | ./$(target)
	@echo "kill 1" | ./$(target)
	@echo "find . -type f | grep '*.cpp'" | ./$(target)
    # @echo "cat < src/built_in_commands.cpp" | ./$(target)
    # @echo "echo Hello > test_output.txt" | ./$(target)
	@echo "cat test_output.txt" | ./$(target)
	@echo "sleep 5 &" | ./$(target)
	@echo "echo \\n" | ./$(target)
    
	@echo "Running Tier 4 tests (extra credit)..."
	@echo "cat < src/built_in_commands.cpp | grep -o include | cat > test_output.txt" | ./$(target)
	@echo "cat test_output.txt" | ./$(target)
	@echo "echo Hello >> test_output.txt" | ./$(target)
	@echo "cat test_output.txt" | ./$(target)
	@echo "echo \\n" | ./$(target)