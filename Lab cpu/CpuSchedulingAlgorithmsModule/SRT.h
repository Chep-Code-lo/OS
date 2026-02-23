#pragma once
#include "Scheduler.h"

inline void SRT(Processes &p) {
    reset_all(p);
    sort_by_arrive(p);

    auto segs = preemptive_sim(p,
        [](const Processes &p, const vector<int> &r, int t) -> int {
            int best = -1, bv = INT_MAX;
            for (int i = 0; i < (int)p.size(); i++) {
                if (p[i].arrive_time > t || r[i] <= 0) continue;
                if (r[i] < bv) {
                    bv = r[i];
                    best = i;
                } else if (r[i] == bv && best != -1) {
                    if (p[i].arrive_time < p[best].arrive_time)
                        best = i;
                    else if (p[i].arrive_time == p[best].arrive_time && p[i].id < p[best].id)
                        best = i;
                }
            }
            return best;
        });

    cout << "\tSJF Preemptive (Shortest Remaining Time)\n\n";
    print_gantt(p, segs);
    print_averages(p);
    print_table(p, segs);
}
