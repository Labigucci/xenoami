#ifndef TASK3_H_INCLUDED
#define TASK3_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <alchemy/task.h>
#include <alchemy/queue.h>

#define NUM_POINTS 1000000
#define QUEUE_SIZE 10

//Creation of a queue variable
RT_QUEUE Q;

//Pi estimation based on the known algorithm
void monteC(void* arg) {
    unsigned int inside = 0;

    for (int i = 0; i < NUM_POINTS; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        double distance = x * x + y * y;

        if (distance <= 1.0) {
            inside++;
        }
    }

    double estimated_pi = 4.0 * inside / NUM_POINTS;
    //Writing to the queue-Descriptor,what to be written,size,mode Q_NORMAL ensures FIFO
    rt_queue_write(&Q, &estimated_pi, sizeof(double), Q_NORMAL);
}

int task3() {
    //Queue creation-descriptor, name,size,limit and mode
    rt_queue_create(&Q, "PiQUEUE", QUEUE_SIZE, Q_UNLIMITED, Q_FIFO);
    //Xenomai task creation
    RT_TASK task;
    rt_task_create(&task, "Pi", 0, 99, T_JOINABLE);
    rt_task_start(&task, monteC, NULL);
    rt_task_join(&task);

    double estimation;
    //Reading-descriptor,value we read,size,number of clock ticks
    rt_queue_read(&Q, &estimation, sizeof(double), TM_INFINITE);
    printf("Estimated value of pi: %f\n", estimation);
    //Deletion to release the tasks
    rt_queue_delete(&Q);

    return 0;
}







#endif // TASK3_H_INCLUDED
