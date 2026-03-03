#pragma once
#include "Scheduler.h"

inline void FCFS(Processes &p) {
    reset_all(p);
    sort_by_arrive(p);
    int t = 0;
    vector<GanttSeg> segs;
    for (int i = 0; i < (int)p.size(); i++) {
        if (t < p[i].arrive_time) t = p[i].arrive_time;
        p[i].waiting_time = t - p[i].arrive_time;
        segs.push_back({i, t, t + p[i].burst});
        t += p[i].burst;
        p[i].completion_time = t;
        p[i].completed = true;
    }
    print_results("FCFS Scheduling Algorithm", p, segs);
}
