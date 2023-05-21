#ifndef TASK1_H_INCLUDED
#define TASK1_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <alchemy/task.h>
#include </home/xeno/sources/xenomai-3.1/include/trank/native/task.h>
#include </home/xeno/sources/xenomai-3.1/include/trank/native/timer.h>
#include </home/xeno/sources/xenomai-3.1/include/trank/rtdk.h>

long long sleeping = 1000000000000;
//Task creation
RT_TASK task;
//Task method-reading the interrupt timer value for 100 times
void periodic(void* arg) {
    RTIME time;
    int i = 0;
    while (i < 100) {
        //Reading from the timer
        time = rt_timer_read();
        //printf("Current timer:");
        printf("Current timer: %llu\n", time);
        rt_task_wait_period(NULL);
        i++;
    }
}

int task1() {
    //Task creation and running
    rt_task_create(&task, "Periodic interrupt", 0, 50, 0);
    rt_task_start(&task, &periodic, NULL);
    //Setting the periodicity
    rt_task_set_periodic(NULL, TM_NOW, sleeping);

    return 0;
}

#endif // TASK1_H_INCLUDED
