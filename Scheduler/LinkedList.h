//
// Created by leahycarlos21 on 6/5/21.
//

#ifndef SCHEDULER_LINKEDLIST_H
#define SCHEDULER_LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ucontext.h>


typedef struct dataItem{
    pthread_t *tid;
    int state;
    int priority;
    int var_SJF;
    int channel;
    float rms_C;
    float rms_P;
    long scheduler_Selected; // Indica el calendarizador a utilizar
    int column;
    int row;
    int antId;

} dataItem;



typedef struct listNode_t{
    dataItem * dataInfo;
    struct listNode_t *next;
    int  size;
}listNode_t;


enum State {SJF = 0, PRIORITY = 1, PERIOD = 2};

void push_t(struct listNode_t **start_ref,  dataItem  * dataInfo);
void append(struct listNode_t** head_ref, dataItem  * dataInfo);
void deleteNodePosition(struct listNode_t **head_ref, int position);
void deleteNodeAntId_t(struct listNode_t** head_ref,int antId);
void bubbleSort_t(struct listNode_t *start,int typeSort);
void deleteList(struct listNode_t** head_ref);
void swap(struct listNode_t *a, struct listNode_t *b);
dataItem * getNode_t(struct listNode_t* head, int index);
void printList_t(struct listNode_t *start);
void listCycle_t(struct listNode_t** head);
int getCount_t(struct listNode_t* head);
dataItem* getFront_t(struct listNode_t* head);

#endif //SCHEDULER_LINKEDLIST_H
