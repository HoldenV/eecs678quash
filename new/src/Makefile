# Authors: Holden Vail, Blake Carlson
# Date: 10-09-2024
# Class: EECS 678
# Assignment: Quash Project
# File: Makefile
# Description: Makefile for quash shell


CC = gcc
CFLAGS = -Wall -g
TARGET = quash
SRC = quash.c

run: $(TARGET)
	./$(TARGET)


all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)


test:
# Do all the actions define in the requirements

clean:
	rm -f $(TARGET)

