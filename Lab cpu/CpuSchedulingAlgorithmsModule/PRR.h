#pragma once
#include "Scheduler.h"

inline void PRR(Processes &p, Quantum quantum) {
    reset_all(p);
    sort_by_arrive(p);
    int n = (int)p.size();
    vector<int> remain(n);
    vector<GanttSeg> segs;
    for (int i = 0; i < n; i++)
        remain[i] = p[i].burst;

    int t = 0, completed = 0, prev = -1;
    while (completed < n) {
        int idx = -1, hp = INT_MAX;
        for (int i = 0; i < n; i++)
            if (p[i].arrive_time <= t && remain[i] > 0 && p[i].priority < hp) {
                hp = p[i].priority;
                idx = i;
            }
        if (idx == -1) {
            t++;
            prev = -1;
            continue;
        }
        int exec = min(remain[idx], quantum);
        if (idx != prev || segs.empty())
            segs.push_back({idx, t, t + exec});
        else
            segs.back().end += exec;
        prev = idx;
        t += exec;
        remain[idx] -= exec;
        if (remain[idx] == 0) {
            p[idx].waiting_time = (t - p[idx].arrive_time) - p[idx].burst;
            p[idx].completion_time = t;
            completed++;
        }
    }
    print_results("Priority with Round Robin ( Quantum : " + to_string(quantum) + " )", p, segs);
}
