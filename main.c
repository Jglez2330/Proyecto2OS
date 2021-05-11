#include <stdio.h>
#include <sched.h>
#include "CEThread.h"
#include <malloc.h>
#include <unistd.h>

int counter = 0;
CEThread_mutex_t mutex;
void* increaseCounter(void* idStruct);

int main() {
    CEThread_t t1, t2, t3,t4, t5, t6,t7,t8,t9,t10,t11;

    CEThread_mutex_init(&mutex, NULL);
    int* id1;
    int* id2;
    int* id3 = malloc(sizeof(int));
    int* id4 = malloc(sizeof(int));
    int* id5 = malloc(sizeof(int));
    int* id6 = malloc(sizeof(int));
    int* id7 = malloc(sizeof(int));
    int* id8 = malloc(sizeof(int));
    int* id9 = malloc(sizeof(int));
    int* id10 = malloc(sizeof(int));
    int* id11 = malloc(sizeof(int));


    id1 = malloc(sizeof(int));
    id2 = malloc(sizeof(int));

    *id1  =1;
    *id2 = 2;
    *id3 = 3;
    *id4 = 4;
    *id5 = 5;
    *id6 = 6;
    *id7 = 7;
    *id8 = 8;
    *id9 = 9;
    *id10 = 10;
    *id11 = 11;

    CEThread_create(&t1, NULL, increaseCounter, id1);
    CEThread_create(&t2, NULL, increaseCounter, id2);
    CEThread_create(&t3, NULL, increaseCounter, id3);
    CEThread_create(&t4, NULL, increaseCounter, id4);
    CEThread_create(&t5, NULL, increaseCounter, id5);
    CEThread_create(&t6, NULL, increaseCounter, id6);
    CEThread_create(&t7, NULL, increaseCounter, id7);
    CEThread_create(&t8, NULL, increaseCounter, id8);
    CEThread_create(&t9, NULL, increaseCounter, id9);
    CEThread_create(&t10, NULL, increaseCounter, id10);
    CEThread_create(&t11, NULL, increaseCounter, id11);

    CEThread_join(t1, NULL);

    CEThread_join(t2, NULL);
    CEThread_join(t3, NULL);
    CEThread_join(t4, NULL);
    CEThread_join(t5, NULL);
    CEThread_join(t6, NULL);
    CEThread_join(t7, NULL);
    CEThread_join(t8, NULL);
    CEThread_join(t9, NULL);
    CEThread_join(t10, NULL);
    CEThread_join(t11, NULL);

    printf("Resultado %i\n", counter);
    return 0;
}

void* increaseCounter(void* idStruct){
    int* threadNumber = idStruct;
    int result = 0;
    while (1){
        printf("El hilo numero %d tiene el control \n", *threadNumber);
    };
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
