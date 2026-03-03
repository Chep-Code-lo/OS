#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>
#include <iomanip>

using namespace std;

struct Process {
    string id;
    int arrive_time = 0, burst = 0, priority = 0;
    int waiting_time = 0, return_time = 0, turnaround_time = 0, response_time = 0;
    int completion_time = 0;
    bool completed = false;

    void reset() {
        waiting_time = return_time = turnaround_time = response_time = completion_time = 0;
        completed = false;
    }
};

using Processes = vector<Process>;
using Quantum   = int;
