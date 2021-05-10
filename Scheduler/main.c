#include <stdio.h>
#include <malloc.h>
#include "Scheduler.h"
#include "LinkedList.h"




//static queue_t  basic_queue;
/*
typedef struct scheduler_t {
    queue_t* (*funcion_calendarizador)(scheduler_t*);
    queue_t* ant_list_ready_a;
    queue_t* ant_list_ready_b;
    queue_t* zombie_ants_a;
    queue_t* zombie_ants_b;
}scheduler_t;*/




int main() {

    scheduler_t * schedulerMain = malloc(sizeof (scheduler_t));
   /* schedulerMain->funcion_calendarizador = receiveThreads;
    schedulerMain->ant_list_ready_a = malloc(sizeof (queue_t));
    schedulerMain->ant_list_ready_b = malloc(sizeof (queue_t));
    schedulerMain->zombie_ants_a = malloc(sizeof (queue_t));
    schedulerMain->zombie_ants_b = malloc(sizeof (queue_t));*/
    schedulerMain->funcion_calendarizador = receiveThreads;
    schedulerMain->ant_list_ready_a = NULL;
    schedulerMain->ant_list_ready_b = NULL;
    schedulerMain->zombie_ants_a = NULL;
    schedulerMain->zombie_ants_b =NULL;



    unsigned long int tid0 =888;
    unsigned long int tid1 =1242;
    unsigned long int tid2 =283;
    unsigned long int tid3 =34343;

    CEThread_treadInfo* pos0 = malloc(sizeof(CEThread_treadInfo));
    pos0->state = 1;
    pos0->priority= 6;
    pos0->tid = tid0;
    pos0->var_SJF = 4;
    CEThread_treadInfo* pos1 = malloc(sizeof(CEThread_treadInfo));

    pos1->state = 1;
    pos1->priority= 10;
    pos1->tid = tid1;
    pos1->var_SJF = 3;


    CEThread_treadInfo* pos2 = malloc(sizeof(CEThread_treadInfo));

    pos2->state = 1;
    pos2->priority= 2;
    pos2->tid = tid2;
    pos2->tid = 2;


    CEThread_treadInfo* pos3 = malloc(sizeof(CEThread_treadInfo));

    pos3->state = 1;
    pos3->priority= 10;
    pos3->tid = tid3;
    pos3->var_SJF = 1;



    push_t(&schedulerMain->ant_list_ready_a,pos0);
    push_t(&schedulerMain->ant_list_ready_a,pos1);
    push_t(&schedulerMain->ant_list_ready_a,pos2);
    append(&schedulerMain->ant_list_ready_a,pos3);

    printList_t(schedulerMain->ant_list_ready_a);
    printf("\n--BUBBLE SORT SJF--");
    bubbleSort_t(schedulerMain->ant_list_ready_a, SJF);
    printList_t(schedulerMain->ant_list_ready_a);


    printf("\n--BUBBLE SORT PRIORIDAD--");
    bubbleSort_t(schedulerMain->ant_list_ready_a, PRIORITY);
    printList_t(schedulerMain->ant_list_ready_a);




    printList_t(schedulerMain->ant_list_ready_a);
    printf("\nEliminar por TID %li", tid1);
    deleteNodeTID_t(&schedulerMain->ant_list_ready_a,tid1);



    printList_t(schedulerMain->ant_list_ready_a);


   printf("\n Cycle");
   listCycle_t(&schedulerMain->ant_list_ready_a);
   printList_t(schedulerMain->ant_list_ready_a);

   printf("\n Get Front");
     CEThread_treadInfo* result= getFront_t(schedulerMain->ant_list_ready_a);
     printf("\nTID FRONT %li",result->tid);


     printf("\n \n--------NUEVA PARTE--------\n");

    schedulerMain->funcion_calendarizador(schedulerMain);
    printList_t(schedulerMain->ant_list_ready_a);
    schedulerMain->funcion_calendarizador(schedulerMain);
    printList_t(schedulerMain->ant_list_ready_a);
    schedulerMain->funcion_calendarizador(schedulerMain);
    printList_t(schedulerMain->ant_list_ready_a);
    schedulerMain->funcion_calendarizador(schedulerMain);
    printList_t(schedulerMain->ant_list_ready_a);
    schedulerMain->funcion_calendarizador(schedulerMain);
    printList_t(schedulerMain->ant_list_ready_a);
    schedulerMain->funcion_calendarizador(schedulerMain);
    schedulerMain->ant_list_ready_a->threadInfo->state=TERMINATED;

    schedulerMain->funcion_calendarizador(schedulerMain);
    printList_t(schedulerMain->ant_list_ready_a);



    listNode_t* resultCalendarizador =schedulerMain->funcion_calendarizador(schedulerMain);
    printf("\nResultadoCalendarizador  con front state == 3");
    printList_t(resultCalendarizador);
    printf("\nDatos mostrados por A");
    printList_t(schedulerMain->ant_list_ready_a);

    printList_t(schedulerMain->zombie_ants_a);

    /*listNode_t* resultCalendarizador2 =schedulerMain->funcion_calendarizador(schedulerMain);
    printf("\nResultadoCalendarizador con front sate !=3");
    printList_t(resultCalendarizador2);
    printList_t(schedulerMain->ant_list_ready_a);*/


    return 0;
}
