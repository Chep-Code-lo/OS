#ifndef __FIRST__COME__FIRST__SERVED__
#define __FIRST__COME__FIRST__SERVED__

#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"

void FCFS(Process *p, int len)
{
    process_init(p, len);
    quick_sort_by_arrive_time(p, len);

    int t = 0;            
    double sumW = 0, sumT = 0, sumR = 0;

    for(int i=0; i < len; i++){
        if (t < p[i].arrive_time) t = p[i].arrive_time;  

        p[i].waiting_time    = t - p[i].arrive_time;     
        p[i].response_time   = p[i].waiting_time;      
        t += p[i].burst;                                 
        p[i].return_time     = t;                       
        p[i].turnaround_time = p[i].return_time - p[i].arrive_time;

        sumW += p[i].waiting_time;
        sumT += p[i].turnaround_time;
        sumR += p[i].response_time;
    }

    printf("\n\tFCFS Scheduling Algorithm\n\n");
    print_gantt_chart_simple(p, len);
    printf("\n\tAverage Waiting Time     : %.2f\n", sumW / len);
    printf("\tAverage Turnaround Time  : %.2f\n", sumT / len);
    printf("\tAverage Response Time    : %.2f\n\n", sumR / len);

    print_table(p, len);
}

#endif
