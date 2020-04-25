#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>

typedef struct child{
	char name[32];
	int ready_time;
	int exe_time;
	pid_t pid;
} Child;

void unit_time();

int fork_process(Child *process);