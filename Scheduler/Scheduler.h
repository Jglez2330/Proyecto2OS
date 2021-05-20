//
// Created by leahycarlos21 on 6/5/21.
//

#ifndef SCHEDULER_SCHEDULER_H
#define SCHEDULER_SCHEDULER_H
#include <sys/ucontext.h>
#include "LinkedList.h"
#include <math.h>

/*
typedef struct scheduler_t {
    queue_t* (*funcion_calendarizador)(struct scheduler_t*);
    queue_t* ant_list_ready_a;
    queue_t* ant_list_ready_b;
    queue_t* zombie_ants_a;
    queue_t* zombie_ants_b;
}scheduler_t;*/
/*
typedef struct scheduler_t {
    int canalNumber;
    int side_Flag;
    listNode_t* (*funcion_calendarizador)(struct scheduler_t*);
    listNode_t* ant_list_ready_a;
    listNode_t* ant_list_ready_b;
    listNode_t* zombie_ants_a;
    listNode_t* zombie_ants_b;
    long scheduler_Selected;
    long flowControl;
    long parameterW_A;
    long parameterW_B;
    long timeCambioLetrero;

}scheduler_t;*/




enum TYPESORT {READY = 0, RUNNING = 1, BLOCKED = 2, TERMINATED = 3};
/*
typedef struct Thread_t{
    ///CEThread_t tid;
    //CEThread_t joining;
    int state;
    int priority;
    void* (*pFunction)(void*);
    void* arg;
    void* retval;
    //ucontext_t* thread_context;
} CEThread_treadInfo;*/


listNode_t* schedulerSort(listNode_t* listThreads);

//listNode_t* receiveThreads(listNode_t* listResult);
listNode_t* roundRobin (listNode_t* listResult);
listNode_t* priority (listNode_t* listResult);
listNode_t* shortJobFirst (listNode_t* listResult);
listNode_t* FCFS(listNode_t* listResult);
listNode_t* tiempoReal_Init(listNode_t* listResult);



#endif //SCHEDULER_SCHEDULER_H
