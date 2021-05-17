#include <stdio.h>
#include <sched.h>
#include "CEThread//CEThread.h"
#include <malloc.h>
#include <unistd.h>
#include "Synchronizer/synchronizer.h"

int counter = 0;
CEThread_mutex_t mutex;
void* increaseCounter(void* idStruct);
int main() {

    scheduler_t * schedulerMain = malloc(sizeof (scheduler_t));
    scheduler_t * schedulerMain2 = malloc(sizeof (scheduler_t));
    scheduler_t * schedulerMain3 = malloc(sizeof (scheduler_t));

    CEThread_t t1, t2,t3,t4,t5;
    schedulerMain->funcion_calendarizador = receiveThreads;
    schedulerMain->ant_list_ready_a = NULL;
    schedulerMain->ant_list_ready_b = NULL;
    schedulerMain->zombie_ants_a = NULL;
    schedulerMain->zombie_ants_b =NULL;
    schedulerMain->canalNumber = 1;

    schedulerMain2->funcion_calendarizador = receiveThreads;
    schedulerMain2->ant_list_ready_a = NULL;
    schedulerMain2->ant_list_ready_b = NULL;
    schedulerMain2->zombie_ants_a = NULL;
    schedulerMain2->zombie_ants_b =NULL;
    schedulerMain->canalNumber = 2;


    schedulerMain3->funcion_calendarizador = receiveThreads;
    schedulerMain3->ant_list_ready_a = NULL;
    schedulerMain3->ant_list_ready_b = NULL;
    schedulerMain3->zombie_ants_a = NULL;
    schedulerMain3->zombie_ants_b =NULL;
    schedulerMain->canalNumber = 3;


      int* id1 = malloc(sizeof(int));
       int* id2 = malloc(sizeof(int));
       int* id3 = malloc(sizeof(int));
       int* id4 = malloc(sizeof(int));
       int* id5 = malloc(sizeof(int));

       *id1 = 1;
       *id2 = 2;
       *id3 = 3;
       *id4 = 4;
       *id5 = 5;

       CEThread_create(&t1, NULL,increaseCounter, id1, schedulerMain);
       CEThread_create(&t2, NULL,increaseCounter, id2, schedulerMain);
       CEThread_create(&t3,NULL, increaseCounter, id3, schedulerMain);
       CEThread_create(&t4,NULL, increaseCounter, id4, schedulerMain2);
       CEThread_create(&t5,NULL, increaseCounter, id5, schedulerMain2);



       printf("Resultado %i\n", counter);





    return 0;
}

void* increaseCounter(void* idStruct){
    int* threadNumber = idStruct;
    int result = 0;

    for (int i = 0; i < 100000; i++) {
        printf("El hilo numero %d tiene el control \n", *threadNumber);
        printf("Iteracion numero %i\n", i);
        counter++;
    }
    *threadNumber = result;

    return NULL;
}

