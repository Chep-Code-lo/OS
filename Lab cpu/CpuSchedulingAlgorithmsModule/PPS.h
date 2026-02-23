#pragma once
#include "Scheduler.h"

inline void PPS(Processes &p) {
    reset_all(p);
    sort_by_arrive(p);

    auto segs = preemptive_sim(p,
        [](const Processes &p, const vector<int> &r, int t) -> int {
            int best = -1, bv = INT_MAX;
            for (int i = 0; i < (int)p.size(); i++)
                if (!p[i].completed && p[i].arrive_time <= t && r[i] > 0 && p[i].priority < bv)
                    { bv = p[i].priority; best = i; }
            return best;
        });

    cout << "\tPreemptive Priority Scheduling Algorithm\n\n";
    print_gantt(p, segs);
    print_averages(p);
    print_table(p, segs);
}
