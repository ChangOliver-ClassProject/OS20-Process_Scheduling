#define _GNU_SOURCE
#include "process.h"


void unit_time(){
	volatile unsigned long i; 
	for(i=0; i<1000000UL; i++);
}

int fork_process(Child *process){
	pid_t pid = fork();

	if (pid == 0) {
		setvbuf(stdout, NULL, _IONBF, 0);

		process->pid = getpid();
		printf("%s %d\n", process->name, process->pid);
		
		long start;

		for (int i = 0; i < process->exe_time; i++){
			if (i == 0){
				start = syscall(335);
			}
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
