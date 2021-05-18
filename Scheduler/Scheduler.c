//
// Created by leahycarlos21 on 6/5/21.
//

#include <stdio.h>
#include "Scheduler.h"



listNode_t* schedulerInit(listNode_t* listResult){

    if (getCount_t(listResult)  == -1){
        return NULL;
    }

    switch (listResult->dataInfo->scheduler_Selected)
    {
        case 0:
            printf("\nRR");

            break;

        case 1:
//            printf("Prioridad");
            bubbleSort_t(listResult,PRIORITY);

            break;
        case 2:
            printf("SJF");
            bubbleSort_t(listResult,SJF);

            break;
        case 3:
            printf("FCFS");
            //listResult = FCFS(listResult);
            break;
        case 4:
            printf("\nTiempo Real");
            tiempoReal_Init(listResult);
            break;

        default:
            printf("Doesnt found");

    }

//    printf("\nSALIO de schedulerINIT");


    return listResult;

}

/*

listNode_t* receiveThreads(listNode_t* listResult){

    listNode_t* listResult;

    switch (scheduler->scheduler_Selected)
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

    if(this->back == NULL)
        return;

    this->back->next = this->front;
    this->back = this->front;
    this->front = this->front->next;
    this->back->next = NULL;
    printf("\nSALIO");
    return listResult;
}*/
/*
listNode_t* roundRobin (listNode_t* listResult){

    if (listResult == NULL){
        return NULL;
    }

    return listResult;
}*/
/*
listNode_t* priority (listNode_t* listResult){

    bubbleSort_t(listResult,PRIORITY);

    return listResult;

}*/
/*
listNode_t* shortJobFirst (listNode_t* listResult){
    if(listResult == NULL){
        return NULL;
    }
    bubbleSort_t(listResult,SJF);

    return listResult;
}*/
/*
listNode_t* FCFS(listNode_t* listResult){
    if (listResult == NULL){
        return NULL;
    }

    return listResult;
}*/

listNode_t* tiempoReal_Init(listNode_t* listResult){
    if (listResult == NULL){
        return NULL;
    }

    bubbleSort_t(listResult,PERIOD);
    float timeInCPU_SUM;
    double condicion;

    int n = getCount_t(listResult) +1;
    timeInCPU_SUM = 0;
    int i;
    dataItem * node;
    for (i = 0; i < n; i++) {
        node = getNode_t(listResult,i);
        timeInCPU_SUM += (node->rms_C / node->rms_P); //Sumar los valores de cada nodo
    }
    condicion = n * (pow(2, 1/(float)n) - 1);

    while(timeInCPU_SUM> condicion){
        deleteNodePosition(&listResult,n-1);
        printList_t(listResult);

        n = getCount_t(listResult) + 1;
        timeInCPU_SUM = 0;
        for (i = 0; i < n; i++) {
            node = getNode_t(listResult,i);
            timeInCPU_SUM += (node->rms_C / node->rms_P); //Sumar los valores de cada nodo
        }
        condicion = n * (pow(2, 1/(float)n) - 1);
    }

    return listResult;
}
