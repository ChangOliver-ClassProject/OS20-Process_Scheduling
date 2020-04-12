#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "process.h"
#include <time.h>
#define _GNU_SOURCE

void unit_time(){
	volatile unsigned long i; 
	for(i=0; i<1000000UL; i++);
}

void fork_process(Child *process){
	pid_t pid = fork();

	if (pid == 0) {

		process->pid = getpid();
		printf("%s %d\n", process->name, process->pid);
		
		long start = syscall(335);
		for (int i=0; i < process->exe_time; i++)
			unit_time();
		long end = syscall(335);

		syscall(334, process->pid, start, end);

		exit(0);
	}
	else if (pid > 0){
		return;
	}
	else if (pid < 0){
		perror("fork() error");
		exit(1);
	}
}
