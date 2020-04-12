#include <linux/kernel.h>
#include <linux/linkage.h>
//334
asmlinkage void sys_my_printk(int pid, long start, long end){

	printk(KERN_INFO "[Project1] %d %ld.%09ld %ld.%09ld\n", pid, start/1000000000, start%1000000000, end/1000000000, end%1000000000);

}