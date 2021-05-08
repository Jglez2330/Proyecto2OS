//
// Created by leahycarlos21 on 6/5/21.
//

#include <assert.h>
#include "LinkedList.h"



void push_t(struct listNode_t **start_ref, CEThread_treadInfo * threadInfo)
{
struct listNode_t *ptr1 = (struct listNode_t*)malloc(sizeof(struct listNode_t));
ptr1->threadInfo = threadInfo;
ptr1->next = *start_ref;
*start_ref = ptr1;
}
void append(struct listNode_t** head_ref, CEThread_treadInfo * threadInfo)
{
    struct listNode_t* new_node = (struct listNode_t*) malloc(sizeof(struct listNode_t));
    struct listNode_t *last = *head_ref;
    new_node->threadInfo  = threadInfo;
    new_node->next = NULL;

    if (*head_ref == NULL)
    {
        *head_ref = new_node;
        return;
    }

    while (last->next != NULL)
        last = last->next;

    last->next = new_node;
    return;
}
void deleteNodePosition(struct listNode_t **head_ref, int position)
{
    if (*head_ref == NULL)
        return;

    struct listNode_t* temp = *head_ref;
    if (position == 0){
        *head_ref = temp->next;
        free(temp);
        return;
    }

    for (int i=0; temp!=NULL && i<position-1; i++)
        temp = temp->next;

    if (temp == NULL || temp->next == NULL)
        return;

    struct listNode_t *next = temp->next->next;

    free(temp->next);

    temp->next = next;
}

void deleteNodeValue(struct listNode_t** head_ref, CEThread_treadInfo * threadInfo)
{
    struct listNode_t *temp = *head_ref, *prev;

    if (temp != NULL && temp->threadInfo == threadInfo) {
        printf("Entro");
        *head_ref = temp->next; // Changed head
        free(temp);
        return;
    }

    while (temp != NULL && temp->threadInfo != threadInfo) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return;

    prev->next = temp->next;
    free(temp);
}

void deleteList(struct listNode_t** head_ref){
    struct listNode_t* current = *head_ref;
    struct listNode_t* next;
    while (current != NULL){
        next = current->next;
        free(current);
        current = next;
    }

    *head_ref = NULL;
}

CEThread_treadInfo* getNode_t(struct listNode_t* head, int index){

    struct listNode_t* current = head;

    int count = 0;
    while (current != NULL) {
        if (count == index)
            return (current->threadInfo);
        count++;
        current = current->next;
    }
    return NULL;
}


void printList_t(struct listNode_t *start)
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
void bubbleSort_t(struct listNode_t *start)
{
    int swapped, i;
    struct listNode_t *ptr1;
    struct listNode_t *lptr = NULL;

    if (start == NULL)
        return;
    do{
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

void swap(struct listNode_t *a, struct listNode_t *b)
{
    int temp = a->threadInfo->priority;
    a->threadInfo->priority = b->threadInfo->priority;
    b->threadInfo->priority= temp;
}