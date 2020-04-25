#define _GNU_SOURCE
#include "process.h"
#include "policy.h"

void info(Child process[], int nprocess){
	printf("================================\n");
	for (int i = 0; i < nprocess; i++){
		printf("%s %d\n", process[i].name, process[i].exe_time);
	}
	printf("================================\n");
}

void assign_cpu(int pid, int num){
	cpu_set_t mask;							
	CPU_ZERO(&mask);						
	CPU_SET(num, &mask);						
	if (sched_setaffinity(pid, sizeof(mask), &mask) != 0) {	
	    perror("sched_setaffinity");				
	}	
}

void set_priority(int pid, int policy, int priority){
	struct sched_param param;
	param.sched_priority = priority;
	if (sched_setscheduler(pid, policy, &param) != 0){
		perror("sched_setscheduler");
	};
}

int sort_ready(const void *a, const void *b){
	Child *pa = (Child*)a;
	Child *pb = (Child*)b; 
	return pa->ready_time - pb->ready_time;
}

int sort_exe(const void *a, const void *b){
	Child *pa = (Child*)a;
	Child *pb = (Child*)b; 
	return pa->exe_time - pb->exe_time;
}

void FIFO(int nprocess, Child process[]){
	int running_pid = -1;
	int running_index = -1;
	int finished = 0;
	qsort(process, nprocess, sizeof(Child), sort_ready);
	for (int time = 0; time >= 0; time++){
		if (time % 500 == 0)
			fprintf(stderr, "Scheduler execute round %d\n", time);
		if (running_pid != -1 && process[running_index].exe_time == 0){
			//printf("waiting for %d to terminate\n", running_pid);
			waitpid(running_pid, NULL, 0);
			running_pid = -1;
			running_index = -1;
			finished++;	
		}

		if (finished == nprocess){
			//printf("finished\n");
			return;
		}

		for (int i = 0; i < nprocess; i++){
			if (process[i].ready_time == time){
				//printf("%s is ready: ", process[i].name);
				fflush(stdout);
				process[i].pid = fork_process(&(process[i]));
				assign_cpu(process[i].pid, 1);
				//printf("%s created with pid %d\n", process[i].name, process[i].pid);
				printf("%s %d\n", process[i].name, process[i].pid);				
				set_priority(process[i].pid, SCHED_FIFO, 50);
			}
			if (running_pid == -1 && process[i].pid != -1 && process[i].exe_time != 0){
				//printf("%s is next\n", process[i].name);
				running_pid = process[i].pid;
				running_index = i;
				set_priority(running_pid, SCHED_FIFO, 99);
			}						
		}

		unit_time();
		//long t = syscall(335);
		//printf("timestamp %ld.%09ld: ", t/1000000000, t%1000000000);
		
		if (running_index != -1){
			process[running_index].exe_time--;
		}		
	}
}

void SJF(int nprocess, Child process[]){
	int running_pid = -1;
	int running_index = -1;
	int finished = 0;
	qsort(process, nprocess, sizeof(Child), sort_exe);
	for (int time = 0; time >= 0; time++){
		if (time % 500 == 0)
			fprintf(stderr, "Scheduler execute round %d\n", time);
		if (running_pid != -1 && process[running_index].exe_time == 0){
			//printf("waiting for %d to terminate\n", running_pid);
			waitpid(running_pid, NULL, 0);
			//set_priority(running_pid, SCHED_FIFO, 10); use in preemptive
			running_pid = -1;
			running_index = -1;
			finished++;	
		}

		if (finished == nprocess){
			//printf("finished\n");
			return;
		}

		for (int i = 0; i < nprocess; i++){
			if (process[i].ready_time == time){
				//printf("%s is ready: ", process[i].name);
				fflush(stdout);
				process[i].pid = fork_process(&(process[i]));
				assign_cpu(process[i].pid, 1);
				printf("%s %d\n", process[i].name, process[i].pid);
				//printf("%s created with pid %d\n", process[i].name, process[i].pid);
				set_priority(process[i].pid, SCHED_IDLE, 0);
			}
			if (running_pid == -1 && process[i].pid != -1 && process[i].exe_time != 0){
				//printf("%s is next\n", process[i].name);
				running_pid = process[i].pid;
				running_index = i;
				set_priority(running_pid, SCHED_FIFO, 99);
			}						
		}

		unit_time();
		//long t = syscall(335);
		//printf("timestamp %ld.%09ld: ", t/1000000000, t%1000000000);

		if (running_index != -1){
			process[running_index].exe_time--;
		}		
	}
}

void PSJF(int nprocess, Child process[]){
	int running_pid = -1;
	int running_index = -1;
	int finished = 0;
	qsort(process, nprocess, sizeof(Child), sort_exe);
	for (int time = 0; time >= 0; time++){
		if (time % 500 == 0)
			fprintf(stderr, "Scheduler execute round %d\n", time);
		if (running_pid != -1 && process[running_index].exe_time == 0){
			//printf("waiting for %d to terminate\n", running_pid);
			waitpid(running_pid, NULL, 0);
			running_pid = -1;
			running_index = -1;
			finished++;	
		}
		if (finished == nprocess){
			return;
		}

		for (int i = 0; i < nprocess; i++){
			if (process[i].ready_time == time){
				//printf("%s is ready: ", process[i].name);
				fflush(stdout);
				process[i].pid = fork_process(&(process[i]));
				assign_cpu(process[i].pid, 1);
				printf("%s %d\n", process[i].name, process[i].pid);
				set_priority(process[i].pid, SCHED_IDLE, 0);
			}
			if (running_pid == -1 && process[i].pid != -1 && process[i].exe_time != 0){
				//printf("%s is next\n", process[i].name);
				running_pid = process[i].pid;
				running_index = i;
				set_priority(running_pid, SCHED_FIFO, 99);
			}
			else if (running_pid != -1 && process[i].pid != -1 && process[i].exe_time != 0 \
							&& process[i].exe_time < process[running_index].exe_time){
				set_priority(running_pid, SCHED_FIFO, 10);
				running_pid = process[i].pid;
				running_index = i;
				set_priority(running_pid, SCHED_FIFO, 99);
			}					
		}

		unit_time();
		//long t = syscall(335);
		//printf("timestamp %ld.%09ld: ", t/1000000000, t%1000000000);

		if (running_index != -1){
			process[running_index].exe_time--;
		}		
	}
}

void RR(int nprocess, Child process[]){
	int running_pid = -1;
	int running_index = -1;
	int finished = 0;
	qsort(process, nprocess, sizeof(Child), sort_ready);
	for (int time = 0; time >= 0; time++){
		if (time % 500 == 0)
			fprintf(stderr, "Scheduler execute round %d\n", time);
		if (running_pid != -1 && process[running_index].exe_time == 0){
			//printf("waiting for %d to terminate\n", running_pid);
			waitpid(running_pid, NULL, 0);
			running_pid = -1;
			running_index = -1;
			finished++;	
		}

		if (finished == nprocess){
			//printf("finished\n");
			return;
		}

		for (int i = 0; i < nprocess; i++){
			if (process[i].ready_time == time){
				//printf("%s is ready: ", process[i].name);
				fflush(stdout);
				process[i].pid = fork_process(&(process[i]));
				assign_cpu(process[i].pid, 1);
				//printf("%s created with pid %d\n", process[i].name, process[i].pid);
				printf("%s %d\n", process[i].name, process[i].pid);
				set_priority(process[i].pid, SCHED_FIFO, 50);
			}
			if (running_pid == -1 && process[i].pid != -1 && process[i].exe_time != 0){
				//printf("%s is next\n", process[i].name);
				running_pid = process[i].pid;
				running_index = i;
				set_priority(running_pid, SCHED_FIFO, 99);
			}
			else if (running_pid != -1 && process[i].pid != -1 && process[i].exe_time != 0 \
							&& time % 500 == 0){
				set_priority(running_pid, SCHED_FIFO, 10);
				running_pid = process[i].pid;
				running_index = i;
				set_priority(running_pid, SCHED_FIFO, 99);
			}									
		}

		unit_time();
		//long t = syscall(335);
		//printf("timestamp %ld.%09ld: ", t/1000000000, t%1000000000);
		
		if (running_index != -1){
			process[running_index].exe_time--;
		}		
	}
}


