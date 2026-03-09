#pragma once
#include "Scheduler.h"

inline void NPPS(Processes &p) {
    reset_all(p);
    sort_by_arrive(p);
    int n = (int)p.size(), done = 0; float t = 0;
    vector<GanttSeg> segs;
    while (done < n) {
        int k = -1;
        for (int i = 0; i < n; i++)
            if (!p[i].completed && p[i].arrive_time <= t
                && (k == -1 || p[i].priority < p[k].priority))
                k = i;
        if (k == -1) {
            t++;
            continue;
        }
        p[k].waiting_time = t - p[k].arrive_time;
        segs.push_back({k, t, t + p[k].burst});
        t += p[k].burst;
        p[k].completion_time = t;
        p[k].completed = true;
        done++;
    }
    print_results("Non-preemptive Priority Scheduling Algorithm", p, segs);
}
