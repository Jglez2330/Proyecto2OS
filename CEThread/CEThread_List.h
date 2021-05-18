//
// Created by jglez2330 on 17/5/21.
//

#ifndef PROYECTO2OS_CETHREAD_LIST_H
#define PROYECTO2OS_CETHREAD_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/ucontext.h>
typedef unsigned long int CEThread_t;
typedef struct Thread_t{
    CEThread_t tid;
    CEThread_t joining;
    int state;
    int detach;
    void* (*pFunction)(void*);
    void* arg;
    void* retval;
    ucontext_t* thread_context;
} CEThread_treadInfo;

typedef struct listNode_t{
    CEThread_treadInfo * threadInfo;
    struct listNode_t *next;
    int  size;
}listNode_t;


enum State {SJF = 0, PRIORITY = 1, PERIOD = 2};

void push_t_thread(struct listNode_t **start_ref, CEThread_treadInfo * threadInfo);
void append_thread(struct listNode_t** head_ref, CEThread_treadInfo * threadInfo);
void deleteNodePosition_thread(struct listNode_t **head_ref, int position);
void deleteNodeTID_t_thread(struct listNode_t** head_ref, CEThread_t key);
void bubbleSort_t_thread(struct listNode_t *start, int typeSort);
void deleteList_thread(struct listNode_t** head_ref);
void swap_thread(struct listNode_t *a, struct listNode_t *b);
CEThread_treadInfo* getNode_t_thread(struct listNode_t* head, int index);
void printList_t_thread(struct listNode_t *start);
void listCycle_t_thread(struct listNode_t** head);
int getCount_t_thread(struct listNode_t* head);
CEThread_treadInfo* getFront_t_thread(struct listNode_t* head);
#endif //PROYECTO2OS_CETHREAD_LIST_H
