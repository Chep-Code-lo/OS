#ifndef __SHORTEST__REMAINING__TIME
#define __SHORTEST__REMAINING__TIME

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"

static int pick_srt(Process *p, int *remain, int len, int t){
	int k = -1, best = INT_MAX;
	for(int i=0; i<len; ++i){
		if(!p[i].completed && p[i].arrive_time <= t){
			if(remain[i] < best)
				best = remain[i], k = i;
			else if(remain[i] == best && k != -1){
				if(p[i].arrive_time < p[k].arrive_time) k = i;
				else if(p[i].arrive_time == p[k].arrive_time && i < k) k = i;
			}
		}
	}
	return k;
}
void srt_calculate_time(Process *p, int len){
	int *remain = (int *)malloc(sizeof(int) * len);
	bool *started = (bool *)malloc(sizeof(bool) * len);

	for(int i=0; i<len; ++i){
		remain[i] = p[i].burst;
		started[i] = false;
	}

	int t = 0, completed_cnt = 0;
	while(completed_cnt < len){
		int k = pick_srt(p, remain, len, t);
		if(k == -1){
			t++;
			continue;
		}
		
	}
}
#endif