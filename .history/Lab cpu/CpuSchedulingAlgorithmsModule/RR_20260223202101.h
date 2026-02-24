#pragma once
#include "Scheduler.h"

inline void RR(Processes &p, Quantum quantum) {
    reset_all(p);
    sort_by_arrive(p);
    int n = (int)p.size();
    vector<int> remain(n);
    vector<GanttSeg> segs;
    for (int i = 0; i < n; i++) remain[i] = p[i].burst;
    int t = 0, prev = -1;
    bool running = true;
    while (running) {
        running = false;
        for (int i = 0; i < n; i++) {
            if (remain[i] <= 0) continue;
            running = true;
            int exec = min(remain[i], quantum);
            if (i != prev || segs.empty()) segs.push_back({i, t, t + exec});
            else                           segs.back().end += exec;
            prev = i;
            t += exec;
            remain[i] -= exec;
            if (remain[i] == 0) {
                p[i].waiting_time = (t - p[i].arrive_time) - p[i].burst;
                p[i].completion_time = t;
                p[i].completed = true;
            }
        }
    }
    cout << "\tRound Robin Scheduling Algorithm ( Quantum : " << quantum << " )\n\n";
    print_gantt(p, segs);
    print_averages(p);
    print_table(p, segs);
}
