#include <stdio.h>
#include <sched.h>
#include "CEThread.h"
#include <malloc.h>
#include <unistd.h>

int counter = 0;
CEThread_mutex_t mutex;
void* increaseCounter(void* idStruct);

int main() {

    scheduler_t * schedulerMain = malloc(sizeof (scheduler_t));
    CEThread_t t1;
    schedulerMain->funcion_calendarizador = receiveThreads;
    schedulerMain->ant_list_ready_a = NULL;
    schedulerMain->ant_list_ready_b = NULL;
    schedulerMain->zombie_ants_a = NULL;
    schedulerMain->zombie_ants_b =NULL;

    int* id1 = malloc(sizeof(int));
    *id1 = 1;
    CEThread_create(&t1, increaseCounter, id1, schedulerMain, 1);
    while (1){
        printf("Hola\n");
    }
    printf("Resultado %i\n", counter);
    return 0;
}

void* increaseCounter(void* idStruct){
    int* threadNumber = idStruct;
    int result = 0;
    for (int i = 0; i < 100; i++) {
        CEThread_mutex_lock(&mutex);
        printf("El hilo numero %d tiene el control \n", *threadNumber);
        printf("Iteracion numero %i\n", i);
        counter++;
        CEThread_mutex_unlock(&mutex);
    }
    *threadNumber = result;

    return NULL;
}
