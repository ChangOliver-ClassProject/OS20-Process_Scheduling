#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/timer.h>
//335
asmlinkage long sys_my_getnstimeofday(){
    struct timespec t;
    getnstimeofday(&t);
    return t.tv_sec * 1000000000 + t.tv_nsec;
}