/*
Author: Blake Carlson, Holden Vail
Date: 10-23-2024
Class: EECS 678
Assignment: Quash Project
File: jobs.hpp
Description: This is the header file for jobs.cpp which contains the function headers for the jobs struct and functions related to jobs
*/

#ifndef JOBS_HPP
#define JOBS_HPP

#include <vector>
#include <string>

using namespace std;

struct Job {
    int pid;             // Store the process id as an integer
    string command;             // The command is stored as a string
    bool is_currently_running;              // Whether the process is currently running or not is a boolean
};

void add_job(int pid, string& command);             // Adds a job
void list_jobs();               // Returns all the jobs completed or running
void sigchild_handler(int sig);                // Handles the SIGCHLD signals when they occur
#endif