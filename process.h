#include <unistd.h>

typedef struct child{
	char name[32];
	int ready_time;
	int exe_time;
	pid_t pid;
} Child;

void unit_time();

void fork_process(Child *process);