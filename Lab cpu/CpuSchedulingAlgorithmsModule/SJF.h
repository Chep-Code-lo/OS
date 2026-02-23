#pragma once
#include "Scheduler.h"

inline void SJF(Processes &p){
    reset_all(p);
    sort_by_arrive(p);
    int n = (int)p.size(), t = 0, done = 0;
    vector<GanttSeg> segs;
    while(done < n){
        int k = -1;
        for (int j = 0; j < n; j++)
            if (!p[j].completed && p[j].arrive_time <= t && (k == -1 || p[j].burst < p[k].burst))
                k = j;
        if (k == -1){ 
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

    cout << "\tSJF Scheduling Algorithm (Non-preemptive)\n\n";
    print_gantt(p, segs);
    print_averages(p);
    print_table(p, segs);
}
