//
// Created by leahycarlos21 on 6/5/21.
//

#ifndef SCHEDULER_SCHEDULER_H
#define SCHEDULER_SCHEDULER_H
#include <sys/ucontext.h>
#include "LinkedList.h"
#include <math.h>



int flag_RMS_MAX;


enum TYPESORT {READY = 0, RUNNING = 1, BLOCKED = 2, TERMINATED = 3};


listNode_t* schedulerSort(listNode_t* listThreads);

listNode_t* roundRobin (listNode_t* listResult);
listNode_t* priority (listNode_t* listResult);
listNode_t* shortJobFirst (listNode_t* listResult);
listNode_t* FCFS(listNode_t* listResult);
listNode_t* tiempoReal_Init(listNode_t* listResult);

int tiempoReal_Check(listNode_t* listResult, dataItem * dataItem1);



#endif //SCHEDULER_SCHEDULER_H