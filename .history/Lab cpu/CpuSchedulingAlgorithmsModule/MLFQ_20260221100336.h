#ifndef __MULTILEVEL__FEEDBACK__QUEUE__
#define __MULTILEVEL__FEEDBACK__QUEUE__

#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"

void MLFQ(Process *p, int len)
{
	int i;
	int curr_time = 0;
	int completed = 0;
	int total_waiting_time = 0;
	int total_turnaround_time = 0;
	int total_response_time = 0;
	int *remain_burst_time = (int *)malloc(sizeof(int) * len);
	int *queue_level = (int *)malloc(sizeof(int) * len);
	int *first_response = (int *)malloc(sizeof(int) * len);
	int quantum[3] = {2, 4, 1000};

	process_init(p, len);
	merge_sort_by_arrive_time(p, 0, len);

	for (i = 0; i < len; i++)
	{
		remain_burst_time[i] = p[i].burst;
		queue_level[i] = 0;
		first_response[i] = false;
	}

	while (completed < len)
	{
		int idx = -1;

		for (int level = 0; level < 3; level++)
		{
			for (i = 0; i < len; i++)
			{
				if (p[i].arrive_time <= curr_time && 
					remain_burst_time[i] > 0 && 
					queue_level[i] == level)
				{
					idx = i;
					break;
				}
			}
			if (idx != -1) break;
		}

		if (idx != -1)
		{
			if (first_response[idx] == false)
			{
				p[idx].response_time = curr_time - p[idx].arrive_time;
				first_response[idx] = true;
			}

			int level = queue_level[idx];
			int exec_time;

			if (level == 2)
			{
				exec_time = remain_burst_time[idx];
			}
			else
			{
				exec_time = (remain_burst_time[idx] > quantum[level]) ? quantum[level] : remain_burst_time[idx];
			}

			curr_time += exec_time;
			remain_burst_time[idx] -= exec_time;

			if (remain_burst_time[idx] == 0)
			{
				p[idx].waiting_time = curr_time - p[idx].burst - p[idx].arrive_time;
				p[idx].return_time = curr_time;
				p[idx].turnaround_time = p[idx].return_time - p[idx].arrive_time;
				completed++;
			}
			else if (level < 2 && exec_time == quantum[level])
			{
				queue_level[idx]++;
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

	printf("\n\tMultilevel Feedback Queue Scheduling\n");
	printf("\t(Queue 0: Quantum=2, Queue 1: Quantum=4, Queue 2: FCFS)\n\n");

	printf("\n\tAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)len);
	printf("\tAverage Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)len);
	printf("\tAverage Response Time    : %-2.2lf\n\n", (double)total_response_time / (double)len);

	print_table(p, len);

	free(remain_burst_time);
	free(queue_level);
	free(first_response);
}

#endif
