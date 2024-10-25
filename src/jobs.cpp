/*
Author: Blake Carlson, Holden Vail
Date: 10-23-2024
Class: EECS 678
Assignment: Quash Project
File: jobs.cpp
Description: This is the header file for jobs.cpp which contains the implementation for the functions related to jobs
*/

#include "jobs.hpp"

#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>
#include <signal.h>
#include <unistd.h>

//Testing to fix compile issues
#include <cstring>

using namespace std;


vector<Job> job_list;               // Make a vector of Jobs, which each represent a job, with process id, command, and whether it is running or not

void add_job(int pid, string& command) {                // Creates a new jobs and sets its attributes accordingly
    Job new_job;
    new_job.pid = pid;
    new_job.command = command;
    new_job.is_currently_running = true;

    job_list.push_back(new_job);                // Uses the push_back function to add it to the global list of jobs
    cout << "Job added: [" << pid << "] " << command << endl;           // Indicate the has been added and give its process id
}

void list_jobs() {
    cout << "Current jobs: " << endl;
    for (size_t i = 0; i < job_list.size(); i++) {
        cout << "[" << job_list[i].pid << "] " << job_list[i].command << " - ";             // Print the process id, followed by the command, followed by the status (running/stopped)
        if (job_list[i].is_currently_running) {             // Print the status of the job
            cout << "Running" << endl;
        } else {
            cout << "Stopped" << endl;
        }
    }
}

void sigchild_handler(int sig) {                // Takes a signal number that is changing state
    int status;
    int pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {              // Checks for any child processes that have changed state. wnohang is a flag, which prevents waitpid from blocking if no child processes have changed state
        for (size_t i = 0; i < job_list.size(); i++) {
            if (job_list[i].pid == pid) {
                if (WIFEXITED(status) || WIFSIGNALED(status)) {             // If the process has exited, either normally or through a signal, set its running status to false and print that it finished
                    job_list[i].is_currently_running = false;
                    const char* msg = "Job finished.\n";
                    write(STDOUT_FILENO, msg, strlen(msg));
                    job_list.erase(job_list.begin() + i);               // Remove the job from the list
                    cout << "[QUASH]$ ";                // Reprint the [QUASH]$ message and flush again once a background process has finished
                    cout.flush();
                }
                break;              // Break out of the loop once the finished job is updated
            }
        }
    }
}
