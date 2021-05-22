//
// Created by jglez2330 on 17/5/21.
//

#ifndef PROYECTO2OS_CETHREAD_LIST_H
#define PROYECTO2OS_CETHREAD_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/ucontext.h>
#include "../Scheduler/LinkedList.h"

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

typedef struct listNode_t_thread{
    CEThread_treadInfo * threadInfo;
    struct listNode_t_thread *next;
    int  size;
}listNode_t_thread;


void block_threads_from_list(listNode_t* listNode, long wfixed, long scheduler_type, int channel);
void unblock_threads_from_list_ants(int channel);
void push_t_thread(struct listNode_t_thread **start_ref, CEThread_treadInfo * threadInfo);
void append_thread(struct listNode_t_thread** head_ref, CEThread_treadInfo * threadInfo);
void deleteNodePosition_thread(struct listNode_t_thread **head_ref, int position);
void deleteNodeTID_t_thread(struct listNode_t_thread** head_ref, CEThread_t key);
void bubbleSort_t_thread(struct listNode_t_thread *start, int typeSort);
void deleteList_thread(struct listNode_t_thread** head_ref);
void swap_thread(struct listNode_t_thread *a, struct listNode_t_thread *b);
CEThread_treadInfo* getNode_t_thread(struct listNode_t_thread* head, int index);
void printList_t_thread(struct listNode_t_thread *start);
void listCycle_t_thread(struct listNode_t_thread** head);
int getCount_t_thread(struct listNode_t_thread* head);
CEThread_treadInfo* getFront_t_thread(struct listNode_t_thread* head);
#endif //PROYECTO2OS_CETHREAD_LIST_H
