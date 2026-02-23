#pragma once
#include "Scheduler.h"

inline void MLFQ(Processes &p) {
    reset_all(p);
    sort_by_arrive(p);

    int n = (int)p.size();
    int q[] = {2, 4, 0};  
    vector<int> remain(n), level(n, 0);
    vector<GanttSeg> segs;
    for (int i = 0; i < n; i++) remain[i] = p[i].burst;

    int t = 0, completed = 0, prev = -1;
    while (completed < n) {
        int idx = -1;
        for (int lv = 0; lv < 3 && idx == -1; lv++)
            for (int i = 0; i < n; i++)
                if (p[i].arrive_time <= t && remain[i] > 0 && level[i] == lv) { idx = i; break; }
        if (idx == -1) { t++; prev = -1; continue; }

        int lv   = level[idx];
        int exec = (lv == 2) ? remain[idx] : min(remain[idx], q[lv]);

        if (idx != prev || segs.empty()) segs.push_back({idx, t, t + exec});
        else                             segs.back().end += exec;
        prev = idx;
        t += exec;
        remain[idx] -= exec;

        if (remain[idx] == 0) {
            p[idx].waiting_time = (t - p[idx].arrive_time) - p[idx].burst;
            p[idx].completion_time = t;
            completed++;
        } else if (lv < 2 && exec == q[lv]) {
            level[idx]++;
        }
    }

    cout << "\n\tMultilevel Feedback Queue Scheduling\n"
         << "\t(Queue 0: Quantum=2, Queue 1: Quantum=4, Queue 2: FCFS)\n\n";
    print_gantt(p, segs);
    print_averages(p);
    print_table(p, segs);
}
