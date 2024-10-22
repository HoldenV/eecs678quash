/*
Authors: Holden Vail, Blake Carlson
Date: 10-09-2024
Class: EECS 678
Assignment: Quash Project
File: execute.cpp
Description: This is the main executable for our project. It runs the quash shell 
*/

#include <string>
#include <iostream>


int main(){
    using namespace std;

    cout << "[QUASH]$ ";

    string input;
    cin >> input;

    cout << input;
    return 0;
}
