#ifndef __PRINT__TABLE__
#define __PRINT__TABLE__

#include "./Process.h"

void print_gantt_chart_simple(Process *p, int len)
{
	int i, j;

	printf("\t ");
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < p[i].burst; j++)
			printf("--");
		printf(" ");
	}
	printf("\n");

	printf("\t|");
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < p[i].burst - 1; j++)
			printf(" ");
		printf("%s", p[i].id);
		for (j = 0; j < p[i].burst - 1; j++)
			printf(" ");
		printf("|");
	}
	printf("\n");

	printf("\t ");
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < p[i].burst; j++)
			printf("--");
		printf(" ");
	}
	printf("\n");

	printf("\t0");
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < p[i].burst; j++)
			printf("  ");
		if (p[i].return_time > 9)
			printf("\b");
		printf("%d", p[i].return_time);
	}
	printf("\n");
}

void print_table(Process p[], int n)
{
	int i;

	puts("\t+-----+------------+-------------+----------+-------------+-----------------+--------------+-----------------+");
	puts("\t| PID | Burst Time | Arrive Time | Priority | Return Time |  Response Time  | Waiting Time | Turnaround Time |");
	puts("\t+-----+------------+-------------+----------+-------------+-----------------+--------------+-----------------+");

	for (i = 0; i < n; i++){
		printf("\t| %3s |     %3d    |     %3d     |    %3d   |     %3d     |      %3d        |      %3d     |        %3d      |\n",
			p[i].id, p[i].burst, p[i].arrive_time, p[i].priority, p[i].return_time, p[i].response_time, p[i].waiting_time, p[i].turnaround_time);

		puts("\t+-----+------------+-------------+----------+-------------+-----------------+--------------+-----------------+");
	}
	puts("\n");
}

#endif
