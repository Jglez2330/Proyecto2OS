//
// Created by leahycarlos21 on 6/5/21.
//

#ifndef SCHEDULER_SCHEDULER_H
#define SCHEDULER_SCHEDULER_H

#include <sys/ucontext.h>
#include "ThreadsQueue.h"

typedef struct scheduler_t {
    queue_t* (*funcion_calendarizador)(struct scheduler_t*);
    queue_t* ant_list_ready_a;
    queue_t* ant_list_ready_b;
    queue_t* zombie_ants_a;
    queue_t* zombie_ants_b;
}scheduler_t;

enum State {READY = 0, RUNNING = 1, BLOCKED = 2, TERMINATED = 3};

typedef struct Thread_t{
    ///CEThread_t tid;
    //CEThread_t joining;
    int state;
    int priority;
    void* (*pFunction)(void*);
    void* arg;
    void* retval;
    //ucontext_t* thread_context;
} CEThread_treadInfo;

queue_t* receiveThreads(scheduler_t* scheduler);
queue_t* roundRobin (scheduler_t * scheduler);


#endif //SCHEDULER_SCHEDULER_H
