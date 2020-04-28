#define _GNU_SOURCE
#include "process.h"
#include "policy.h"
#include <stddef.h>

int main(){

	if (setvbuf(stdout, NULL, _IONBF, 0)){
		perror("setvbuf");
	};

	char policy[5];
	int nprocess = 0;
	scanf("%s%d", policy, &nprocess);

	Child process[nprocess];
	for(int i = 0; i < nprocess; i++){
		scanf("%s%d%d", process[i].name, &process[i].ready_time, &process[i].exe_time);
		process[i].pid = -1;
		process[i].used_time = 0;
		process[i].turn = 0;
	}

	assign_cpu(getpid(), 0);
	set_priority(getpid(), SCHED_FIFO, 50);

	switch(policy[0]) {
	    case 'F':
			FIFO(nprocess, process); 
			break;
	    case 'R': 
			RR(nprocess, process);
			break;
	    case 'S': 
			SJF(nprocess, process);
			break;
	    case 'P': 
			PSJF(nprocess, process);
			break;
    }

	return 0;
}
