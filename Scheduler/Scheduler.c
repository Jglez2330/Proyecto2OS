//
// Created by leahycarlos21 on 6/5/21.
//

#include <stdio.h>
#include "Scheduler.h"
#define SIDE_FLAG 0

listNode_t* receiveThreads(scheduler_t * scheduler){

    int scheduler_Selected = 0;
    listNode_t* listResult;

    switch (scheduler_Selected)
    {
        case 0:
            printf("\nRR");
            listResult = roundRobin(scheduler);


            // statements
            break;

        case 1:
            printf("SFG");
            // statements
            break;

        default:
            printf("Doesnt found");

        // default statements
    }

    /*if(this->back == NULL)
        return;

    this->back->next = this->front;
    this->back = this->front;
    this->front = this->front->next;
    this->back->next = NULL;*/
    printf("\nSALIO");
    return listResult;
}

listNode_t* roundRobin (scheduler_t * scheduler){

    listNode_t * resultQueue;

    listNode_t * selectedReady;
    listNode_t * selectedZombie;

    if(SIDE_FLAG ==0){
        selectedReady = scheduler->ant_list_ready_a;
        selectedZombie = scheduler->zombie_ants_a;
    }
    else{
        selectedReady = scheduler->ant_list_ready_b;
        selectedZombie = scheduler->zombie_ants_b;
    }
    CEThread_treadInfo* listItem= getFront_t(selectedReady);

    if(listItem->state == TERMINATED){

        push_t(&selectedZombie,listItem);
        deleteNodeTID_t(&selectedReady,listItem->tid);
        resultQueue = selectedReady;

    } else{
        printf("VERGA");
        printList_t(selectedReady);
        listCycle_t(&selectedReady);
        resultQueue = selectedReady;
    }

    if(SIDE_FLAG ==0){
        scheduler->ant_list_ready_a = selectedReady;
    }
    else{
        scheduler->ant_list_ready_b = selectedReady;

    }
    return resultQueue;
}
