#pragma once
#include "Process.h"
#include <sstream>

inline string fmt_t(float t) {
    if (t == (float)(int)t) return to_string((int)t);
    ostringstream oss;
    oss << fixed << setprecision(1) << t;
    return oss.str();
}

inline void reset_all(Processes &p) {
    for (auto &x : p)
        x.reset();
}
inline void sort_by_arrive(Processes &p) {
    sort(p.begin(), p.end(), [](const Process &a, const Process &b) {
        return a.arrive_time < b.arrive_time;
    });
}
struct GanttSeg {
    int idx;
    float start, end;
};
inline void print_gantt(const Processes &p, const vector<GanttSeg> &segs) {
    if (segs.empty()) return;
    int cw = 6;
    for (auto &s : segs) {
        int need = max((int)p[s.idx].id.size(), (int)fmt_t(s.start).size()) + 2;
        cw = max(cw, need);
    }
    cw = max(cw, (int)fmt_t(segs.back().end).size() + 2);
    auto bar = [&] {
        cout << "\t ";
        for (int i = 0; i < (int)segs.size(); i++)
            cout << string(cw, '-') << " ";
        cout << "\n";
    };
    bar();
    cout << "\t|";
    for (auto &s : segs) {
        int il = (int)p[s.idx].id.size();
        int pl = max(0, (cw - il) / 2);
        int pr = max(0, cw - il - pl);
        cout << string(pl, ' ') << p[s.idx].id << string(pr, ' ') << "|";
    }
    cout << "\n";
    bar();
    cout << "\t";
    for (auto &s : segs)
        cout << left << setw(cw + 1) << fmt_t(s.start);
    cout << fmt_t(segs.back().end) << "\n";
}
inline float executed_before(const Processes &p, const vector<GanttSeg> &segs, int i, float t) {
    float ex = 0;
    for (auto &s : segs) {
        if (s.idx != i) continue;
        float e = min(s.end, t);
        if (e > s.start) ex += (e - s.start);
    }
    return ex;
}
inline float remaining_at(const Processes &p, const vector<GanttSeg> &segs, int i, float t) {
    return max(0.0f, p[i].burst - executed_before(p, segs, i, t));
}
inline int running_at(const vector<GanttSeg> &segs, float t) {
    for (auto &s : segs)
        if (t >= s.start && t < s.end) return s.idx;
    return -1;
}
inline vector<float> collect_times(const Processes &p, const vector<GanttSeg> &segs) {
    vector<float> times;
    for (auto &s : segs) {
        times.push_back(s.start);
        times.push_back(s.end);
    }
    for (int i = 0; i < (int)p.size(); i++)
        times.push_back(p[i].arrive_time);
    sort(times.begin(), times.end());
    times.erase(unique(times.begin(), times.end()), times.end());
    return times;
}

inline void print_table(const Processes &p, const vector<GanttSeg> &segs) {
    if (segs.empty()) return;
    int n = (int)p.size();
    vector<float> times = collect_times(p, segs);
    const char* RED = "\033[31m";
    const char* BLU = "\033[34m";
    const char* RST = "\033[0m";
    int cw = 5;
    auto sep = [&]() {
        cout << "\t+" << string(cw + 1, '-');
        for (int j = 0; j < (int)times.size(); j++)
            cout << "+" << string(cw, '-');
        cout << "+\n";
    };
    auto print_colored = [&](int rem, const char* color) {
        string s = to_string(rem);
        int pad = max(0, cw - 1 - (int)s.size());
        cout << "| " << string(pad, ' ') << color << s << RST;
    };
    sep();
    cout << "\t| " << left << setw(cw) << "time";
    for (float t : times)
        cout << "| " << right << setw(cw - 1) << fmt_t(t);
    cout << "|\n";
    sep();
    for (int i = 0; i < n; i++) {
        cout << "\t| " << left << setw(cw) << p[i].id;
        for (float t : times) {
            if (t < p[i].arrive_time) {
                cout << "| " << right << setw(cw - 1) << "";
                continue;
            }
            float rem = remaining_at(p, segs, i, t);
            bool running = (running_at(segs, t) == i);
            bool completed = (rem == 0 && p[i].completed && p[i].completion_time == t);
            if (running)
                print_colored((int)rem, RED);
            else if (completed)
                print_colored((int)rem, BLU);
            else
                cout << "| " << right << setw(cw - 1) << (int)rem;
        }
        cout << "|\n";
        sep();
    }
    cout << "\n";
}
inline void print_averages(const Processes &p) {
    double sw = 0;
    int n = (int)p.size();
    for (auto &x : p)
        sw += x.waiting_time;
    cout << "\n\tAverage waiting time = [";
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << "+";
        cout << "(" << fmt_t(p[i].completion_time) << "-" << fmt_t(p[i].arrive_time)
             << "-" << p[i].burst << ")";
    }
    cout << "]/" << n;
    cout << " = " << fmt_t((float)sw) << "/" << n;
    cout << " = " << fixed << setprecision(2) << sw / n << "ms\n\n";
}
inline void print_results(const string &title, const Processes &p, const vector<GanttSeg> &segs) {
    cout << "\n\t" << title << "\n\n";
    print_gantt(p, segs);
    print_averages(p);
    print_table(p, segs);
}
template <typename Picker>
inline vector<GanttSeg> preemptive_sim(Processes &p, Picker pick) {
    int n = (int)p.size();
    int total = 0;
    vector<int> remain(n);
    vector<GanttSeg> segs;
    for (int i = 0; i < n; ++i) {
        remain[i] = p[i].burst;
        total += p[i].burst;
    }
    int done = 0, prev = -1;
    float t = 0;
    while (done < total) {
        int k = pick(p, remain, t);
        if (k == -1) {
            t++;
            prev = -1;
            continue;
        }
        if (k != prev || segs.empty())
            segs.push_back({k, t, t + 1});
        else
            segs.back().end++;
        prev = k;
        remain[k]--;
        done++;
        t++;
        if (remain[k] == 0) {
            p[k].completed = true;
            p[k].completion_time = t;
            p[k].waiting_time = t - p[k].burst - p[k].arrive_time;
        }
    }
    return segs;
}
