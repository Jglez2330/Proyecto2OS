//
// Created by leahycarlos21 on 6/5/21.
//

#include "LinkedList.h"



void insertAtTheBegin(struct listNode_t **start_ref, CEThread_treadInfo * threadInfo)
{
struct listNode_t *ptr1 = (struct listNode_t*)malloc(sizeof(struct listNode_t));
ptr1->threadInfo = threadInfo;
ptr1->next = *start_ref;
*start_ref = ptr1;
}
void printList(struct listNode_t *start)
{
    struct listNode_t *temp = start;
    printf("\n");
    while (temp!=NULL)
    {
        printf("%d ", temp->threadInfo->priority);
        temp = temp->next;
    }
}

/* Bubble sort the given linked list */
void bubbleSort(struct listNode_t *start)
{
    int swapped, i;
    struct listNode_t *ptr1;
    struct listNode_t *lptr = NULL;

    /* Checking for empty list */
    if (start == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = start;

        while (ptr1->next != lptr)
        {
            if (ptr1->threadInfo->priority > ptr1->next->threadInfo->priority)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}

/* function to swap data of two nodes a and b*/
void swap(struct listNode_t *a, struct listNode_t *b)
{
    int temp = a->threadInfo->priority;
    a->threadInfo->priority = b->threadInfo->priority;
    b->threadInfo->priority= temp;
}