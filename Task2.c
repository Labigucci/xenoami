#ifndef TASK2_H_INCLUDED
#define TASK2_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <alchemy/task.h>
#include <alchemy/mutex.h>
#define number_of_threads 2

//Shared variable and mutex declaration
int shared = 0;
RT_MUTEX mutex;

//Method that increments the variable and guarded by the mutex
void increment(void* arg) {
    for (int i = 0; i < 10; i++) {
        rt_mutex_acquire(&mutex, TM_INFINITE);
        shared++;
        rt_mutex_release(&mutex);
    }
}

int task2() {
    //Creating a mutex and number of tasks
    rt_mutex_create(&mutex, "Mutex");
    RT_TASK tasks[number_of_threads];
    //Thread creation and spawnning

    for (int i = 0; i < number_of_threads; i++) {
        rt_task_spawn(&tasks[i], "task2", 0, 99, T_JOINABLE, increment, NULL);
    }

    for (int i = 0; i < number_of_threads; i++) {
        rt_task_join(&tasks[i]);
    }
    //Printing and deleting the mutex in the end
    printf("Shared variable value: %d\n", shared);
    rt_mutex_delete(&mutex);

    return 0;
}



#endif // TASK2_H_INCLUDED
