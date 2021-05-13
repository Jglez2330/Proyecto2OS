//
// Created by leahycarlos21 on 6/5/21.
//

#include <stdio.h>
#include "Scheduler.h"
#define SIDE_FLAG 0

listNode_t* receiveThreads(scheduler_t * scheduler){

    int scheduler_Selected = 4;
    listNode_t* listResult;

    switch (scheduler_Selected)
    {
        case 0:
            printf("\nRR");
            listResult = roundRobin(scheduler);


            // statements
            break;

        case 1:
            printf("Prioridad");
            listResult = priority(scheduler);

            // statements
            break;
        case 2:
            printf("SJF");
            listResult = shortJobFirst(scheduler);
            break;
        case 3:
            printf("FCFS");
            listResult = FCFS(scheduler);
            break;
        case 4:
            printf("\nTiempo Real");
            listResult = tiempoReal(scheduler);
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

    } else{
        listCycle_t(&selectedReady);
    }
    if(selectedReady != NULL) {
        CEThread_treadInfo* next;
        do {
            listCycle_t(&selectedReady);
            next = getFront_t(selectedReady);
        } while (next->state > RUNNING);
    }
    resultQueue = selectedReady;

    if(SIDE_FLAG ==0){
        scheduler->ant_list_ready_a = selectedReady;
        scheduler->zombie_ants_a = selectedZombie;
    }
    else{
        scheduler->ant_list_ready_b = selectedReady;
        scheduler->zombie_ants_b = selectedZombie;
    }

    return resultQueue;
}

listNode_t* priority (scheduler_t * scheduler){
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
    bubbleSort_t(selectedReady,PRIORITY);

    CEThread_treadInfo* listItem= getFront_t(selectedReady);
    selectedReady->threadInfo->priority++;

    if(listItem->state == TERMINATED){
        push_t(&selectedZombie,listItem);
        deleteNodeTID_t(&selectedReady,listItem->tid);


    } else{
    }

    if(selectedReady != NULL) {
        CEThread_treadInfo* next;
        do {
            listCycle_t(&selectedReady);
            next = getFront_t(selectedReady);
        } while (next->state > RUNNING);
    }
    resultQueue = selectedReady;

    if(SIDE_FLAG ==0){
        scheduler->ant_list_ready_a = selectedReady;
        scheduler->zombie_ants_a = selectedZombie;
    }
    else{
        scheduler->ant_list_ready_b = selectedReady;
        scheduler->zombie_ants_b = selectedZombie;

    }
    return resultQueue;

}

listNode_t* shortJobFirst (scheduler_t * scheduler){
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
    if(listItem->flag_SJF != 1){
        printf("\nENTRO  UNICA VEZ");
        bubbleSort_t(selectedReady,SJF);
        selectedReady->threadInfo->flag_SJF = 1;
    }
    else{
        printf("\nOCUPADO");
    }

    if(listItem->state == TERMINATED){
        push_t(&selectedZombie,listItem);
        deleteNodeTID_t(&selectedReady,listItem->tid);
        if (selectedReady != NULL) {
            bubbleSort_t(selectedReady,SJF);
            selectedReady->threadInfo->flag_SJF = 1;
        }


    } else{
    }
    if(selectedReady != NULL) {
        CEThread_treadInfo* next;
        do {
            listCycle_t(&selectedReady);
            next = getFront_t(selectedReady);
        } while (next->state > RUNNING);
    }
    resultQueue = selectedReady;

    if(SIDE_FLAG ==0){
        scheduler->ant_list_ready_a = selectedReady;
        scheduler->zombie_ants_a = selectedZombie;
    }
    else{
        scheduler->ant_list_ready_b = selectedReady;
        scheduler->zombie_ants_b = selectedZombie;

    }
    return resultQueue;
}

listNode_t* FCFS(scheduler_t * scheduler){
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
    } else{
    }
    if(selectedReady != NULL) {
        CEThread_treadInfo* next;

        do {
            listCycle_t(&selectedReady);
            next = getFront_t(selectedReady);
        } while (next->state > RUNNING);
    }
    resultQueue = selectedReady;

    if(SIDE_FLAG ==0){
        scheduler->ant_list_ready_a = selectedReady;
        scheduler->zombie_ants_a = selectedZombie;
    }
    else{
        scheduler->ant_list_ready_b = selectedReady;
        scheduler->zombie_ants_b = selectedZombie;

    }
    return resultQueue;
}

listNode_t* tiempoReal(scheduler_t * scheduler){
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

    if(selectedReady->threadInfo->rms_Status == 0){

        bubbleSort_t(selectedReady,PERIOD);
        float timeInCPU_SUM;
        double condicion;

        int n = getCount_t(selectedReady) +1;
        timeInCPU_SUM = 0;
        int i;
        struct Thread_t * node;
        for (i = 0; i < n; i++) {
            node = getNode_t(selectedReady,i);
            timeInCPU_SUM += (node->rms_C / node->rms_P); //Sumar los valores de cada nodo
            node->rms_Status = 1; //Cambia el estado para que no vuelva a entrar
        }
        condicion = n * (pow(2, 1/(float)n) - 1);

        while(timeInCPU_SUM> condicion){
            deleteNodePosition(&selectedReady,n-1);
            printList_t(selectedReady);

            n = getCount_t(selectedReady) + 1;
            timeInCPU_SUM = 0;
            for (i = 0; i < n; i++) {
                node = getNode_t(selectedReady,i);
                timeInCPU_SUM += (node->rms_C / node->rms_P); //Sumar los valores de cada nodo
            }
            condicion = n * (pow(2, 1/(float)n) - 1);
        }
    }

    CEThread_treadInfo* listItem= getFront_t(selectedReady);

    if(listItem->state == TERMINATED){
        push_t(&selectedZombie,listItem);
        deleteNodeTID_t(&selectedReady,listItem->tid);
    } else{
    }
    if(selectedReady != NULL) {
        CEThread_treadInfo* next;
        do {
            listCycle_t(&selectedReady);
            next = getFront_t(selectedReady);
        } while (next->state > RUNNING);
    }
    resultQueue = selectedReady;
    if(SIDE_FLAG ==0){

        scheduler->ant_list_ready_a = selectedReady;
        scheduler->zombie_ants_a = selectedZombie;
    }
    else{
        scheduler->ant_list_ready_b = selectedReady;
        scheduler->zombie_ants_b = selectedZombie;

    }
    return resultQueue;
}

