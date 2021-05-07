#include <stdio.h>
#include <malloc.h>
#include "ThreadsQueue.h"
#include "Scheduler.h"





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
    schedulerMain->funcion_calendarizador = receiveThreads;
    schedulerMain->ant_list_ready_a = malloc(sizeof (queue_t));
    schedulerMain->ant_list_ready_b = malloc(sizeof (queue_t));
    schedulerMain->zombie_ants_a = malloc(sizeof (queue_t));
    schedulerMain->zombie_ants_b = malloc(sizeof (queue_t));

    queueInit(schedulerMain->ant_list_ready_a);
    queueInit(schedulerMain->ant_list_ready_b);
    queueInit(schedulerMain->zombie_ants_a);
    queueInit(schedulerMain->zombie_ants_b);


    printf("Hello, World!\n");
    // ----------------THREADS de A-------

    CEThread_treadInfo* t1A = malloc(sizeof(CEThread_treadInfo));

    t1A->state = 3;

    queueAddFrontItem(schedulerMain->ant_list_ready_a,t1A);


    CEThread_treadInfo* t2A = malloc(sizeof(CEThread_treadInfo));

    t2A->state = 1;

    queueAddFrontItem(schedulerMain->ant_list_ready_a,t2A);

    // ----------------THREADS de B-------
    CEThread_treadInfo* t1B = malloc(sizeof(CEThread_treadInfo));

    t1B->state = 3;

    queueAddFrontItem(schedulerMain->ant_list_ready_b,t1B);


    CEThread_treadInfo* t2B = malloc(sizeof(CEThread_treadInfo));

    t2B->state = 1;

    queueAddFrontItem(schedulerMain->ant_list_ready_b,t2B);

    // ----------------FIN de THREADS-------



    //struct Thread_t * queueItem =(struct Thread_t*) queue_Getfront(schedulerMain->ant_list_ready_a);



    //queue_cycle(schedulerMain->ant_list_ready_a);
    //queue_cycle(schedulerMain->ant_list_ready_b);


    //struct Thread_t * queueItem2 =(struct Thread_t*) queue_Getfront(schedulerMain->ant_list_ready_a);


    //printf("\n HELLO %i",queueItem2->state);

    queue_t * result = schedulerMain->funcion_calendarizador(schedulerMain);

    printf("\n Tamaño  final  %i",queueSize(result));

    printf("\n Tamaño  ant list ready A  %i",queueSize(schedulerMain->ant_list_ready_a));
    printf("\n Tamaño  ant zombie A  %i",queueSize(schedulerMain->zombie_ants_a));

    printf("\n Tamaño  ant list ready B  %i",queueSize(schedulerMain->ant_list_ready_b));
    printf("\n Tamaño  ant zombie B  %i",queueSize(schedulerMain->zombie_ants_b));





    return 0;
}
