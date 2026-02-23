#ifndef __COMPARE__FUNCTION__
#define __COMPARE__FUNCTION__

#include "./Process.h"

int compare_by_return_time(const void *a, const void *b){
	Process *ptA = (Process *)a;
	Process *ptB = (Process *)b;

	if (ptA->return_time < ptB->return_time) return -1;
	else if (ptA->return_time > ptB->return_time) return 1;
	else return 0;
}

int compare_by_arrive_time(const void *a, const void *b){
    Process *ptA = (Process *)a;
    Process *ptB = (Process *)b;

    if (ptA->arrive_time < ptB->arrive_time) return -1;
    if (ptA->arrive_time > ptB->arrive_time) return 1;
    return 0;
}

void quick_sort_by_return_time(Process p[], int len){
	qsort(p, len, sizeof(Process), compare_by_return_time);
}

void quick_sort_by_arrive_time(Process p[], int len){
    qsort(p, len, sizeof(Process), compare_by_arrive_time);
}
#endif
