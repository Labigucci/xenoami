#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <alchemy/task.h>

//Real time task data structure
RT_TASK demo_task;

void demo(void *arg) {


RT_TASK *curtask;
//Return some information about a task
RT_TASK_INFO curtaskinfo;
//Printing hello world
printf("Hello world\n");
//returning the current task descriptor address
curtask = rt_task_self();
//Inquire current task
rt_task_inquire(curtask,&curtaskinfo);
//Print task name
printf("task name: %s\n",curtaskinfo.name);

}


int main()
{
    char str[10];
    printf("Start task!\n");
    //Passing the value into that string
    sprintf(str,"Demo");
// Creating task arguements: task reference,name,stack size,priority and mode
    rt_task_create(&demo_task,str,0,50,0);
// Starting the task: reference to task,function and function arguement
    rt_task_start(&demo_task,&demo,0);
    return 0;
}

