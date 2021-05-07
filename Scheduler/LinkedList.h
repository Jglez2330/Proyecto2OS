//
// Created by leahycarlos21 on 6/5/21.
//

#ifndef SCHEDULER_LINKEDLIST_H
#define SCHEDULER_LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Thread_t{
    ///CEThread_t tid;
    //CEThread_t joining;
    int state;
    int priority;
    void* (*pFunction)(void*);
    void* arg;
    void* retval;
    //ucontext_t* thread_context;
} CEThread_treadInfo;

typedef struct listNode_t{
    CEThread_treadInfo * threadInfo;
    struct listNode_t *next;
}listNode_t;

/* Function to insert a node at the beginning of a linked list */
void insertAtTheBegin(struct listNode_t **start_ref, CEThread_treadInfo * threadInfo);

/* Function to bubble sort the given linked list */
void bubbleSort(struct listNode_t *start);

/* Function to swap data of two nodes a and b*/
void swap(struct listNode_t *a, struct listNode_t *b);

/* Function to print nodes in a given linked list */
void printList(struct listNode_t *start);

#endif //SCHEDULER_LINKEDLIST_H
