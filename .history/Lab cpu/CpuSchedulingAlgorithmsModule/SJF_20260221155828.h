#ifndef __SHORTEST__JOB__FIRST__
#define __SHORTEST__JOB__FIRST__



#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"

void sjf_calculate_time(Process *p, int len, int *excution_order){
	int curr_time = 0;
	int completed_count = 0;
	while(completed_count < len){
		int min = -1;
		for(int j=0; j<len; ++j){
			if(!p[j].completed && p[j].arrive_time <= curr_time)
				if(min == -1 || p[j].burst < p[min].burst)
					min = j;
		}

		if(min == - 1){
			curr_time++;
			continue;
		}

		excution_order[completed_count] = min;

		p[min].waiting_time = curr_time - p[min].arrive_time;
		curr_time += p[min].burst;
		p[min].return_time = curr_time;
		p[min].turnaround_time = p[min].return_time - p[min].arrive_time;
		p[min].completed = true;
		completed_count++;
	}
}
void SJF(Process *p, int len)
{
	int total_waiting_time = 0;
	int total_turnaround_time = 0;
	int total_response_time = 0;
	int execution_order[len];

	process_init(p, len);

	quick_sort_by_arrive_time(p, len);

	sjf_calculate_time(p, len, execution_order);

	for(int i=0; i<len; ++i){
		p[i].response_time = p[i].waiting_time;

		total_waiting_time += p[i].waiting_time;
		total_turnaround_time += p[i].turnaround_time;
		total_response_time += p[i].response_time;
	}

	printf("\tSJF Scheduling Algorithm (Non-preemptive)\n\n");

	Process gantt[len];
	for(int i=0; i<len; ++i){
		gantt[i] = p[execution_order[i]];
	}
	print_gantt_chart_simple(gantt, len);


	printf("\n\tAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)len);
	printf("\tAverage Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)len);
	printf("\tAverage Response Time    : %-2.2lf\n\n", (double)total_response_time / (double)len);

	print_table(p, len);
}

#endif
