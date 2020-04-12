
all: scheduler

scheduler: 
	gcc -O2 main.c process.c policy.c -o scheduler

clean:
	rm scheduler