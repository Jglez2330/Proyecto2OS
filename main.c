#include <stdio.h>
#include <sched.h>
#include "CEThread.h"
#include <malloc.h>
#include <unistd.h>
#include "Synchronizer/synchronizer.h"

int counter = 0;
CEThread_mutex_t mutex;
void* increaseCounter(void* idStruct);
int main() {

    scheduler_t * schedulerMain = malloc(sizeof (scheduler_t));
    CEThread_t t1, t2,t3,t4,t5;
    schedulerMain->funcion_calendarizador = receiveThreads;
    schedulerMain->ant_list_ready_a = NULL;
    schedulerMain->ant_list_ready_b = NULL;
    schedulerMain->zombie_ants_a = NULL;
    schedulerMain->zombie_ants_b =NULL;


    /*   int* id1 = malloc(sizeof(int));
       int* id2 = malloc(sizeof(int));
       int* id3 = malloc(sizeof(int));
       int* id4 = malloc(sizeof(int));
       int* id5 = malloc(sizeof(int));

       *id1 = 1;
       *id2 = 2;
       *id3 = 3;
       *id4 = 4;
       *id5 = 5;

       CEThread_create(&t1, increaseCounter, id1, schedulerMain, 1);
       CEThread_create(&t2, increaseCounter, id2, schedulerMain, 1);
       CEThread_create(&t3, increaseCounter, id3, schedulerMain, 1);
       CEThread_create(&t4, increaseCounter, id4, schedulerMain, 1);
       CEThread_create(&t5, increaseCounter, id5, schedulerMain, 1);

       CEThread_join(t1, NULL);
       CEThread_join(t2, NULL);
       CEThread_join(t3, NULL);
       CEThread_join(t4, NULL);
       CEThread_join(t5, NULL);
       printf("Resultado %i\n", counter);*/

    printf("\n---Synchronizer\n");
    schedulerMain->canalNumber=0;

    synchronizerInit();
    //equidad_Init(schedulerMain);
    //letrero_Init(schedulerMain);
    tico_Init(schedulerMain);
    printf("\n scheduler_Selected %li", schedulerMain->scheduler_Selected);
    printf("\n flowControl %li", schedulerMain->flowControl);
    printf("\n side_Flag %i", schedulerMain->side_Flag);
    printf("\n parameterW_A %li", schedulerMain->parameterW_A);
    printf("\n parameterW_B %li", schedulerMain->parameterW_B);
    printf("\n timeCambioLetrero %li", schedulerMain->timeCambioLetrero);


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

