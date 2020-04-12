#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"
#include "policy.h"

int FIFO_cmp(const void *a, const void *b){
	Child *pa = (Child*)a;
	Child *pb = (Child*)b; 
	return pa->ready_time - pb->ready_time;
}

void FIFO(int nprocess, Child process[]){
	bool running = false;
	int end_time = 0;
	qsort(process, nprocess, sizeof(Child), FIFO_cmp);

	for (int time = 0, i = 0; i < nprocess; time++){
		if (process[i].ready_time <= time && !running){
			running = true;
			end_time = time + process[i].exe_time;
			fflush(stdout);
			fork_process(&(process[i]));
		}
		if (time == end_time){
			running = false;
			i++;
		}

		unit_time();
	}
}

int SJF_cmp(const void *a, const void *b){
	Child *pa = (Child*)a;
	Child *pb = (Child*)b; 
	if (pa->ready_time == pb->ready_time){
		return pa->exe_time - pb->exe_time;
	}
	else
		return pa->ready_time - pb->ready_time;
}

void SJF(int nprocess, Child process[]){

	bool running = false;
	int end_time = 0;	
	qsort(process, nprocess, sizeof(Child), SJF_cmp);

	for (int time = 0, i = 0; i < nprocess; time++){
		if (process[i].ready_time <= time && !running){
			running = true;
			end_time = time + process[i].exe_time;
			fflush(stdout);
			fork_process(&(process[i]));
		}
		if (time == end_time){
			running = false;
			i++;
		}

		unit_time();
	}
}

void RR(int nprocess, Child process[]){}

void PSJF(int nprocess, Child process[]){}
