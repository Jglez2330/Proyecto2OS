#include <stdio.h>
#include <malloc.h>
#include "ThreadsQueue.h"



typedef struct Thread_t
{
    //gtthread_t tid;
    //gtthread_t joining;
    int value;
    int state;
    void* (*proc)(void*);
    void* arg;
    void* retval;
    //ucontext_t* ucp;
} thread_t;

static queue_t  basic_queue;


int main() {
    printf("Hello, World!\n");

    queueInit(&basic_queue);

    thread_t* t1 = malloc(sizeof(thread_t));

    t1->value = 0;
    t1->state = 0;

    queueAddFrontItem(&basic_queue,t1);


    thread_t* t2 = malloc(sizeof(thread_t));

    t2->value = 1;
    t2->state = 1;

    queueAddFrontItem(&basic_queue,t2);

    struct Thread_t * queueItem =(struct Thread_t*) queue_Getfront(&basic_queue);

    printf("\n%i",queueItem->value);

    queue_cycle(&basic_queue);

    struct Thread_t * queueItem2 =(struct Thread_t*) queue_Getfront(&basic_queue);


    printf("\n%i",queueItem2->value);



    return 0;
}
