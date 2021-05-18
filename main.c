#include <stdio.h>
#include <sched.h>
#include "CEThread/CEThread.h"
#include <malloc.h>
#include <unistd.h>


int counter = 0;
CEThread_mutex_t mutex;
void* increaseCounter(void* idStruct);
int main() {



    CEThread_t t1, t2,t3,t4,t5;

    CEThread_mutex_init(&mutex);

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

    CEThread_create(&t1, NULL,increaseCounter, id1);
    CEThread_create(&t2, NULL,increaseCounter, id2);
    CEThread_create(&t3,NULL, increaseCounter, id3);
    CEThread_create(&t4,NULL, increaseCounter, id4);
    CEThread_create(&t5,NULL, increaseCounter, id5);

    CEThread_join(t1, NULL);
    CEThread_join(t2, NULL);
    CEThread_join(t3, NULL);
    CEThread_join(t4, NULL);
    CEThread_join(t5, NULL);


    printf("Resultado %i\n", counter);





    return 0;
}

void* increaseCounter(void* idStruct){
    int* threadNumber = idStruct;
    int result = 0;

    for (int i = 0; i < 100000; i++) {
        CEThread_mutex_lock(&mutex);
        printf("El hilo numero %d tiene el control \n", *threadNumber);
        printf("Iteracion numero %i\n", i);
        if (*threadNumber%2 == 0) {
            counter++;
        } else{
            counter--;
        }
        CEThread_mutex_unlock(&mutex);
    }
    *threadNumber = result;

    return NULL;
}

