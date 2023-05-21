#ifndef TASK4_H_INCLUDED
#define TASK4_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <alchemy/task.h>
#include </home/xeno/sources/xenomai-3.1/include/trank/native/task.h>
#include </home/xeno/sources/xenomai-3.1/include/trank/native/timer.h>
#include </home/xeno/sources/xenomai-3.1/include/trank/rtdk.h>
#include <alchemy/mutex.h>
#include <alchemy/sem.h>
#include <alchemy/buffer.h>
#include <stdbool.h>
#define BUFFER_SIZE 1000000

//Buffer creation
RT_BUFFER buffer;
//Semaphore creation
RT_SEM writesem;
RT_SEM readsem;
char data[10000];
bool buffer_updated = false;
//Readers function
void reader(void* arg)
{
    FILE* fp = fopen("output.txt", "w");
    if (fp == NULL)
    {
        printf("Failed to open output file\n");
        return;
    }

    while (true)
    {
        //Semaphore waiting
        rt_sem_p(&readsem, TM_INFINITE);
        // Allocate memory for data
        //void *data = malloc(100 * sizeof(char));
        if (buffer_updated) {
            // Read from the shared memory buffer
            rt_buffer_read(&buffer, data, 100, TM_INFINITE);
            printf("Writing into the file from reader\n");
            //Writing to the file
            fwrite(data, sizeof(char), 100, fp);
            fflush(fp);
            //free(data);  // Free the allocated memory
            buffer_updated = false;
        }
        //Semaphore signaling
        rt_sem_v(&writesem);
    }

    printf("Reader finished writing to the file\n");
    fclose(fp);
}

void writer(void* arg)
{
    FILE* fp = fopen("input.txt", "r");
    if (fp == NULL)
    {
        printf("Failed to open input file\n");
        return;
    }

    while (!feof(fp) && !ferror(fp))
    {
        //Semaphore wait
        rt_sem_p(&writesem, TM_INFINITE);
        //Allocate memory for data
        //void *data = malloc(100 * sizeof(char));
        printf("Writing into shared memory\n");
        //Reading from the file
        fread(data, sizeof(char), 100, fp);
        fflush(stdout);
        //Writing to the buffer
        rt_buffer_write(&buffer, data, 100, TM_INFINITE);
        // free(data);
        buffer_updated = true;
        //Signal a semaphore
        rt_sem_v(&readsem);
    }

    printf("Writer finished writing to shared memory\n");
    fclose(fp);

    // Release the writer semaphore to allow the reader to exit
}

int task4()
{
    //Buffer creation
    rt_buffer_create(&buffer, "buffer", BUFFER_SIZE, B_FIFO);
    //Creating and initialising the semaphores
    rt_sem_create(&writesem, "writerSem", 1, S_FIFO);
    rt_sem_create(&readsem, "readerSem", 0, S_FIFO);
    //Task creation,starting and joining
    RT_TASK readerTask, writerTask;
    rt_task_create(&readerTask, "reader", 0, 50, T_JOINABLE);
    rt_task_create(&writerTask, "writer", 0, 50, T_JOINABLE);
    rt_task_start(&writerTask, writer, NULL);
    rt_task_start(&readerTask, reader, NULL);
    rt_task_join(&writerTask);  // Add this line to wait for the writer task to finish

}



#endif // TASK4_H_INCLUDED
