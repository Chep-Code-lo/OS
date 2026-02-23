#ifndef __PRIORITY__ROUND__ROBIN__
#define __PRIORITY__ROUND__ROBIN__

#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"

void PRR(Process *p, int len, Quantum quantum)
{
	int i;
	int curr_time = 0;
	int completed = 0;
	int total_waiting_time = 0;
	int total_turnaround_time = 0;
	int total_response_time = 0;
	int *remain_burst_time = (int *)malloc(sizeof(int) * len);
	int *first_response = (int *)malloc(sizeof(int) * len);

	process_init(p, len);
	quick_sort_by_arrive_time(p, len);

	for (i = 0; i < len; i++)
	{
		remain_burst_time[i] = p[i].burst;
		first_response[i] = false;
	}

	while (completed < len)
	{
		int idx = -1;
		int highest_priority = INT_MAX;

		for (i = 0; i < len; i++)
		{
			if (p[i].arrive_time <= curr_time && remain_burst_time[i] > 0)
			{
				if (p[i].priority < highest_priority)
				{
					highest_priority = p[i].priority;
					idx = i;
				}
			}
		}

		if (idx != -1)
		{
			if (first_response[idx] == false)
			{
				p[idx].response_time = curr_time - p[idx].arrive_time;
				first_response[idx] = true;
			}

			int exec_time = (remain_burst_time[idx] > quantum) ? quantum : remain_burst_time[idx];
			curr_time += exec_time;
			remain_burst_time[idx] -= exec_time;

			if (remain_burst_time[idx] == 0)
			{
				p[idx].waiting_time = curr_time - p[idx].burst - p[idx].arrive_time;
				p[idx].return_time = curr_time;
				p[idx].turnaround_time = p[idx].return_time - p[idx].arrive_time;
				completed++;
			}
		}
		else
		{
			curr_time++;
		}
	}

	for (i = 0; i < len; i++)
	{
		total_waiting_time += p[i].waiting_time;
		total_turnaround_time += p[i].turnaround_time;
		total_response_time += p[i].response_time;
	}

	printf("\n\tPriority with Round Robin Scheduling ( Quantum : %d )\n\n", quantum);
	printf("\t(Processes scheduled by priority, Round Robin for same priority level)\n\n");

	printf("\n\tAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)len);
	printf("\tAverage Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)len);
	printf("\tAverage Response Time    : %-2.2lf\n\n", (double)total_response_time / (double)len);

	print_table(p, len);

	free(remain_burst_time);
	free(first_response);
}

#endif
