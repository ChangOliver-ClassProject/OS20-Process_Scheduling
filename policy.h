#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sched.h>

void set_priority(int pid, int policy, int priority);

void assign_cpu(int pid, int num);

int sort_ready(const void *a, const void *b);

int sort_exe(const void *a, const void *b);

void FIFO(int nprocess, Child process[]);

void RR(int nprocess, Child process[]);

void SJF(int nprocess, Child process[]);

void PSJF(int nprocess, Child process[]);