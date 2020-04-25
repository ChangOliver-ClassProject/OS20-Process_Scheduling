#define _GNU_SOURCE
#include "process.h"


void unit_time(){
	volatile unsigned long i; 
	for(i=0; i<1000000UL; i++);
}

int fork_process(Child *process){
	pid_t pid = fork();

	if (pid == 0) {

		process->pid = getpid();
		//printf("%s %d\n", process->name, process->pid);
		
		long start = syscall(335);
		for (int i=0; i < process->exe_time; i++){
			//printf("Child %s execute round %d\n", process->name, i);
			//long t = syscall(335);
			//printf("timestamp %ld.%09ld: ", t/1000000000, t%1000000000);
			unit_time();
		}
		long end = syscall(335);

		syscall(334, process->pid, start, end);

		exit(0);
	}
	else if (pid > 0){
		return pid;
	}
	else if (pid < 0){
		perror("fork()");
		exit(1);
	}
}
