//
// Created by leahycarlos21 on 6/5/21.
//

#include <stdio.h>
#include "Scheduler.h"



listNode_t* schedulerSort(listNode_t* listResult){

    if (getCount_t(listResult)  == -1){
        return NULL;
    }

    switch (listResult->dataInfo->scheduler_Selected)
    {
        case 0:
            //printf("\nRR");

            break;

        case 1:
            //printf("Prioridad");
            bubbleSort_t(listResult,PRIORITY);

            break;
        case 2:
            //printf("SJF");
            bubbleSort_t(listResult,SJF);

            break;
        case 3:
            //printf("FCFS");
            //listResult = FCFS(listResult);
            break;
        case 4:
            //printf("\nTiempo Real");
            tiempoReal_Init(listResult);
            break;

    }



    return listResult;

}


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

int tiempoReal_Check(listNode_t* listResult, dataItem * dataItem1){
    float timeInCPU_SUM = 0;
    int n = 0;

    if(listResult == NULL){
        return 1;
    }
    if (listResult != NULL){
        int i;
        n = getCount_t(listResult) +1; // 1 por el offset de la funcion

        dataItem * node;
        for (i = 0; i < n; i++) {
            node = getNode_t(listResult,i);
            timeInCPU_SUM += (node->rms_C / node->rms_P); //Sumar los valores de cada nodo
        }

    }
    double condicion;

    timeInCPU_SUM +=(dataItem1->rms_C / dataItem1->rms_P); //Sumar el valor del nuevo

    n++;

    condicion = (n) * (pow(2, 1/(float)(n)) - 1);
    printf("\nCondicion RMS %f \n", condicion);
    printf("\ntime in CPUE %f \n", timeInCPU_SUM);

    if(timeInCPU_SUM<= condicion){
        return 1; // Si se puede ingresar
    }
    return 0; // No se puede ingresar
}
