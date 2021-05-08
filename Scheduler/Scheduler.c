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
    return listResult;
}

listNode_t* roundRobin (scheduler_t * scheduler){

  //  struct Thread_t * queueItem =(struct Thread_t*) queue_Getfront(scheduler->ant_list_ready_a);
    listNode_t * resultQueue;

    //printf("\n HELLO %i",queueItem->state);
    listNode_t * selectedReady;
    listNode_t * selectedZombie;
   // CEThread_treadInfo* result= getNode_t(schedulerMain->zombie_ants_a,  1);
    //    //printf("\nValor %i",result->priority);

    // Se setea dependiende el flujo
    if(SIDE_FLAG ==0){
        selectedReady = scheduler->ant_list_ready_a;
        selectedZombie = scheduler->zombie_ants_a;
    }
    else{
        selectedReady = scheduler->ant_list_ready_b;
        selectedZombie = scheduler->zombie_ants_b;
    }
    CEThread_treadInfo* listItem= getNode_t(selectedReady,  0);
    printf("\nValor %i",listItem->priority);

    if(listItem->state == TERMINATED){
       // struct Thread_t * zombiItem = (struct Thread_t *) queue_Getfront(selectedReady);
        push_t(&selectedZombie,listItem);

        //queueRemoveFrontItem(selectedReady);

        //queueAddFrontItem(selectedZombie, zombiItem);
        resultQueue = selectedReady;

    } else{
       // queue_cycle(selectedReady);
        resultQueue = selectedZombie;
    }
    return resultQueue;
}
/*
listNode_t* roundRobin (scheduler_t * scheduler){

    printf("\n SIZEEE%i",queueSize(scheduler->ant_list_ready_a));
    struct Thread_t * queueItem =(struct Thread_t*) queue_Getfront(scheduler->ant_list_ready_a);
    queue_t * resultQueue;

    printf("\n HELLO %i",queueItem->state);
    queue_t * selectedReady;
    queue_t * selectedZombie;


    // Se setea dependiende el flujo
    if(SIDE_FLAG ==0){
        selectedReady = scheduler->ant_list_ready_a;
        selectedZombie = scheduler->zombie_ants_a;
    }
    else{
        selectedReady = scheduler->ant_list_ready_b;
        selectedZombie = scheduler->zombie_ants_b;
    }


     if(queueItem->state == TERMINATED){
         struct Thread_t * zombiItem = (struct Thread_t *) queue_Getfront(selectedReady);
         queueRemoveFrontItem(selectedReady);

         queueAddFrontItem(selectedZombie, zombiItem);
         resultQueue = selectedReady;

     } else{
         queue_cycle(selectedReady);
         resultQueue = selectedZombie;
     }
    return resultQueue;
}

queue_t* prioridad (scheduler_t * scheduler) {




}*/