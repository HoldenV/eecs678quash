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
static int next_job_id = 1;             // Have a variable which keeps track of the next available job id. Starts at 1 and goes up when a new job is created and goes down when one finishes.

void add_job(int pid, string& command) {                // Creates a new jobs and sets its attributes accordingly
    Job new_job;
    new_job.pid = pid;
    new_job.job_id = next_job_id++;         // Sets the jobs id as the next available id. Then increase it by 1 for the next job.
    new_job.command = command;
    new_job.is_currently_running = true;
    new_job.been_printed_done = false;

    job_list.push_back(new_job);                // Uses the push_back function to add it to the global list of jobs
    cout << "Background job started: [" << new_job.job_id << "] " << new_job.pid << " " << new_job.command << endl;
}

void list_jobs() {
    for (size_t i = 0; i < job_list.size(); ) {
        cout << "[" << job_list[i].job_id << "] ";
        if (job_list[i].is_currently_running) {
            cout << "Running " << job_list[i].command << endl;
            i++; // Only increment if not removing
        } else {
            cout << "Done " << job_list[i].command << endl;
            job_list[i].been_printed_done = true;
            // Remove the job if it has been printed as done
            job_list.erase(job_list.begin() + i);
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
                }
                break;              // Break out of the loop once the finished job is updated
            }
        }
    }
}