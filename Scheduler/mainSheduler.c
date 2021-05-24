//
// Created by leahycarlos21 on 10/5/21.
//

#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include "Scheduler.h"
#include "LinkedList.h"

typedef struct  ant_channel{
    listNode_t * listAIn;
    listNode_t * listAOut;
    int numberCanal;

} ant_channel;


int channelInUse = 0;
int flagA_B= 0;
pthread_mutex_t mutex;

ant_channel * canal1 ;

void* increaseCounter(void* idStruct);

int main() {

    listNode_t * list_Ant_A1 = NULL;

    int scheduler_Selected = 2;


    pthread_t *toledoAnt_1 = malloc(sizeof(pthread_t));


    dataList  * hormiga0 = malloc(sizeof (dataList));
    hormiga0->state = 0;
    hormiga0->priority= 10;
    hormiga0->var_SJF = 4;
    hormiga0->scheduler_Selected = scheduler_Selected;
    hormiga0->rms_C = 1;
    hormiga0->rms_P = 6;
    hormiga0->tid = toledoAnt_1;
    hormiga0->column = 0;
    hormiga0->row = 0;

    pthread_t *toledoAnt_2 = malloc(sizeof(pthread_t));

    unsigned long int tid1 = 200;
    dataList  * hormiga1 = malloc(sizeof (dataList));
    hormiga1->state = RUNNING;
    hormiga1->priority= 20;
    hormiga1->tid = toledoAnt_2;
    hormiga1->var_SJF = 6;
    hormiga1->scheduler_Selected = scheduler_Selected;
    hormiga1->rms_C = 2;
    hormiga1->rms_P = 9;
    hormiga1->column = 0;
    hormiga1->row = 0;


    pthread_t *toledoAnt_3 = malloc(sizeof(pthread_t));
    dataList  * hormiga2 = malloc(sizeof (dataList));
    hormiga2->state = RUNNING;
    hormiga2->priority= 30;
    hormiga2->tid = toledoAnt_3;
    hormiga2->var_SJF = 7;
    hormiga2->scheduler_Selected = scheduler_Selected;
    hormiga2->rms_C = 6;
    hormiga2->rms_P = 18;
    hormiga2->column = 0;
    hormiga2->row = 0;

    push_t(&list_Ant_A1,hormiga0);
    push_t(&list_Ant_A1,hormiga1);
    push_t(&list_Ant_A1,hormiga2);


    schedulerInit(list_Ant_A1);
    printList_t(list_Ant_A1);


    pthread_mutex_init(&mutex,NULL);


    canal1 = malloc(sizeof(ant_channel));


    canal1->listAIn = list_Ant_A1;
    canal1->listAOut = NULL;
    canal1->numberCanal = 0;


    pthread_create(toledoAnt_1,NULL,increaseCounter, canal1);
    pthread_create(toledoAnt_2,NULL,increaseCounter, canal1);
    pthread_create(toledoAnt_3,NULL,increaseCounter, canal1);

    pthread_join(*toledoAnt_1, NULL);
    pthread_join(*toledoAnt_2, NULL);
    pthread_join(*toledoAnt_3, NULL);


    //&t1, increaseCounter, id1, schedulerMain, 1);

/*
    *id5 = 5;

    pthread_create(&t1, increaseCounter, id1, schedulerMain, 1);*/






/*
    scheduler_t * schedulerMain = malloc(sizeof (scheduler_t));
   schedulerMain->funcion_calendarizador = receiveThreads;
     schedulerMain->ant_list_ready_a = malloc(sizeof (queue_t));
     schedulerMain->ant_list_ready_b = malloc(sizeof (queue_t));
     schedulerMain->zombie_ants_a = malloc(sizeof (queue_t));
     schedulerMain->zombie_ants_b = malloc(sizeof (queue_t));
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
    pos0->state = 2;
    pos0->priority= 6;
    pos0->tid = tid0;
    pos0->var_SJF = 4;
    pos0->rms_Status = 0;
    pos0->rms_C = 1;
    pos0->rms_P = 6;
    CEThread_treadInfo* pos1 = malloc(sizeof(CEThread_treadInfo));

    pos1->state = 1;
    pos1->priority= 10;
    pos1->tid = tid1;
    pos1->var_SJF = 4;
    pos1->rms_Status = 0;
    pos1->rms_C = 2;
    pos1->rms_P = 9;


    CEThread_treadInfo* pos2 = malloc(sizeof(CEThread_treadInfo));

    pos2->state = 1;
    pos2->priority= 2;
    pos2->tid = tid2;
    pos1->var_SJF = 4;
    pos2->rms_Status = 0;
    pos2->rms_C = 6;
    pos2->rms_P = 18;


    CEThread_treadInfo* pos3 = malloc(sizeof(CEThread_treadInfo));

    pos3->state = 1;
    pos3->priority= 4;
    pos3->tid = tid3;
    pos3->var_SJF = 1;
    pos3->rms_Status = 0;
    pos3->rms_C = 6;
    pos3->rms_P = 18;



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

*/

/*
    printList_t(schedulerMain->ant_list_ready_a);
    printf("\nEliminar por TID %li", tid1);
    deleteNodeTID_t(&schedulerMain->ant_list_ready_a,tid1);*/


/*
    printList_t(schedulerMain->ant_list_ready_a);


    printf("\n Cycle");
    listCycle_t(&schedulerMain->ant_list_ready_a);
    printList_t(schedulerMain->ant_list_ready_a);

    printf("\n Cycle");
    int i = 0;
    while(i<10000) {
        listCycle_t(&schedulerMain->ant_list_ready_a);
        printList_t(schedulerMain->ant_list_ready_a);
        i++;
    }
    i = 0;

    printf("\n Get Front");
    CEThread_treadInfo* result= getFront_t(schedulerMain->ant_list_ready_a);
    printf("\nTID FRONT %li",result->tid);
*/

    printf("\n \n--------NUEVA PARTE--------\n");

   /* schedulerMain->funcion_calendarizador(schedulerMain);
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

    printList_t(schedulerMain->zombie_ants_a);*/




    return 0;
}
void* increaseCounter(void* idStruct){
    //int* threadNumber = idStruct;
    ant_channel *canalData = idStruct;
    int result = 0;
/*
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        printf("El hilo numero  tiene el control \n");
        printf("Iteracion numero %i\n", i);
        counter++;
        pthread_mutex_unlock(&mutex);
    }

  */
        while(1) {
            if (canal1->listAIn == NULL) {
                break;
            }

            else if (canalData->numberCanal == 0) {

                if (flagA_B == 0) {

                    if (channelInUse == 0) {
                        pthread_mutex_lock(&mutex);

                        channelInUse = 1;
                        dataList *data1 = getNode_t(canalData->listAIn, 0);
                        printf("FUCK");
                        if(data1 != NULL){
                            struct timespec tiempo ;
                            tiempo.tv_sec = 0;
                            tiempo.tv_nsec = 100000000;
                            for (int i = 0; i < 10; i++) {
                                nanosleep(&tiempo,&tiempo);
                                data1->column += i;
                                printf("\nValor columna %i\n", getNode_t(canal1->listAIn, 0)->column);

                            }

                            push_t(&canalData->listAOut, data1);
                            deleteNodePosition(&canalData->listAIn, 0);
                            printf("\nCANAL ListaIN");
                            printList_t(canalData->listAIn);
                            printf("\nCANAL ListaOUT  ");
                            printList_t(canalData->listAOut);
                            channelInUse = 0;
                            pthread_mutex_unlock(&mutex);
                            if(canalData->listAIn == NULL){
                                return NULL;
                            }
                        }
                        else{
                            return NULL;
                        }


                    }


                }


            }
        }





    return NULL;
}


void moverse(dataList * dataListIn){
    dataListIn->column ++;

}