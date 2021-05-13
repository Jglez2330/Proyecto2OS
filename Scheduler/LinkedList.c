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

void deleteNodeTID_t(struct listNode_t** head_ref, CEThread_t key)
{
    int count = 0;

    struct listNode_t* temp = *head_ref;
    while (temp->threadInfo->tid!= key && temp!=NULL)
    {
        temp = temp->next;
        count ++;
    }
    deleteNodePosition(head_ref,count);
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
        printf("||| Prioridad %d - TID %li  - Time SJF %i - PERIOD %f||| ", temp->threadInfo->priority, temp->threadInfo->tid, temp->threadInfo->var_SJF, temp->threadInfo->rms_P);
        temp = temp->next;
    }
    printf("\n");

}

void bubbleSort_t(struct listNode_t *start,int typeSort)
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
            if (((ptr1->threadInfo->priority > ptr1->next->threadInfo->priority) && (PRIORITY == typeSort)) ||
                    ((ptr1->threadInfo->var_SJF > ptr1->next->threadInfo->var_SJF) && (SJF == typeSort))
                     ||((ptr1->threadInfo->rms_P > ptr1->next->threadInfo->rms_P) && (PERIOD == typeSort)))
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

void swap(struct listNode_t *a, struct listNode_t *b){

    CEThread_treadInfo *temp   = a->threadInfo;
    a->threadInfo = b->threadInfo;
    b->threadInfo = temp;
}

int getCount_t(struct listNode_t* head)
{
    int count = 0;  // Initialize count
    struct listNode_t* current = head;  // Initialize current
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count -1;
}
void listCycle_t(struct listNode_t** head){
    CEThread_treadInfo* listItem= getNode_t(*head ,  0);

    append(head,listItem);
    deleteNodePosition(head,0);
}

CEThread_treadInfo* getFront_t(struct listNode_t* head){

    if(head == NULL){
        return NULL;
    }
    return getNode_t(head,0);

}
