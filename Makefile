
all: scheduler

scheduler: 
	gcc -O2  -w -pthread main.c process.c policy.c -o scheduler

clean:
	rm scheduler