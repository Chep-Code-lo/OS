#pragma once
#include "Scheduler.h"
#include <deque>

inline void MLFQ(Processes &p) {
    reset_all(p);
    sort_by_arrive(p);
    int n = (int)p.size();
    int quanta[] = {8, 16, 0};
    vector<int> remain(n), level(n, 0);
    vector<GanttSeg> segs;
    for (int i = 0; i < n; i++)
        remain[i] = p[i].burst;
    deque<int> queues[3];
    vector<bool> inQueue(n, false);
    float t = 0; int completed = 0, prev = -1;
    int curProc = -1, curUsed = 0;
    auto enqueueArrivals = [&](int time) {
        for (int i = 0; i < n; i++)
            if (p[i].arrive_time <= time && remain[i] > 0
                && !inQueue[i] && i != curProc) {
                queues[level[i]].push_back(i);
                inQueue[i] = true;
            }
    };
    while (completed < n) {
        enqueueArrivals(t);
        int bestLv = -1;
        for (int lv = 0; lv < 3; lv++)
            if (!queues[lv].empty()) {
                bestLv = lv;
                break;
            }
        if (curProc != -1 && bestLv != -1 && bestLv < level[curProc]) {
            queues[level[curProc]].push_front(curProc);
            inQueue[curProc] = true;
            curProc = -1;
            curUsed = 0;
        }
        if (curProc == -1)
            for (int lv = 0; lv < 3; lv++)
                if (!queues[lv].empty()) {
                    curProc = queues[lv].front();
                    queues[lv].pop_front();
                    inQueue[curProc] = false;
                    curUsed = 0;
                    break;
                }
        if (curProc == -1) {
            t++;
            prev = -1;
            continue;
        }
        if (curProc != prev || segs.empty())
            segs.push_back({curProc, t, t + 1});
        else
            segs.back().end++;
        prev = curProc;
        t++;
        remain[curProc]--;
        curUsed++;
        if (remain[curProc] == 0) {
            p[curProc].waiting_time = (t - p[curProc].arrive_time) - p[curProc].burst;
            p[curProc].completion_time = t;
            p[curProc].completed = true;
            completed++;
            curProc = -1;
            curUsed = 0;
            prev = -1;
        } else {
            int lv = level[curProc];
            int q = quanta[lv];
            if (q > 0 && curUsed >= q) {
                if (lv < 2)
                    level[curProc]++;
                queues[level[curProc]].push_back(curProc);
                inQueue[curProc] = true;
                curProc = -1;
                curUsed = 0;
                prev = -1;
            }
        }
    }
    print_results("Multilevel Feedback Queue Scheduling\n\t(Q1: Quantum=8, Q2: Quantum=16, Q3: FCFS)", p, segs);
}
