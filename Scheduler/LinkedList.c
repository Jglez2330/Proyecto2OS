//
// Created by leahycarlos21 on 6/5/21.
//

#include "LinkedList.h"



void push_t(struct listNode_t **start_ref, dataItem  * dataInfo)
{
    struct listNode_t *ptr1 = (struct listNode_t*)malloc(sizeof(struct listNode_t));
    ptr1->dataInfo = dataInfo;
    ptr1->next = *start_ref;
    *start_ref = ptr1;
}
void append(struct listNode_t** head_ref, dataItem  * dataInfo)
{
    struct listNode_t* new_node = (struct listNode_t*) malloc(sizeof(struct listNode_t));
    struct listNode_t *last = *head_ref;
    new_node->dataInfo  = dataInfo;
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

void deleteNodeAntId_t(struct listNode_t** head_ref,int antId)
{
    int count = 0;

    struct listNode_t* temp = *head_ref;
    while (temp->dataInfo->antId!= antId && temp!=NULL)
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

dataItem * getNode_t(struct listNode_t* head, int index){


    struct listNode_t* current = head;

    int count = 0;
    while (current != NULL) {
        if (count == index)
            return (current->dataInfo);
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
        printf("||| Prioridad %d - ANT ID %i  - Time SJF %i - PERIOD %f||| ", temp->dataInfo->priority, temp->dataInfo->antId, temp->dataInfo->var_SJF, temp->dataInfo->rms_P);
        temp = temp->next;
    }
    printf("\n");

}

void bubbleSort_t(struct listNode_t *start, int typeSort)
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
            if (((ptr1->dataInfo->priority > ptr1->next->dataInfo->priority) && (PRIORITY == typeSort)) ||
                ((ptr1->dataInfo->var_SJF > ptr1->next->dataInfo->var_SJF) && (SJF == typeSort))
                ||((ptr1->dataInfo->rms_P > ptr1->next->dataInfo->rms_P) && (PERIOD == typeSort)))
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

    struct Ant *temp   = a->dataInfo;
    a->dataInfo = b->dataInfo;
    b->dataInfo = temp;
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
/*void listCycle_t(struct listNode_t** head){
    CEThread_treadInfo* listItem= getNode_t(*head ,  0);

    append(head,listItem);
    deleteNodePosition(head,0);
}*/

void listCycle_t(struct listNode_t** head){
    int k = 1;
    struct listNode_t* current = *head;

    int count = 1;
    while (count < k && current != NULL) {
        current = current->next;
        count++;
    }

    if (current == NULL)
        return;

    struct listNode_t* kthNode = current;

    while (current->next != NULL)
        current = current->next;

    current->next = *head;

    *head = kthNode->next;

    kthNode->next = NULL;
}


dataItem * getFront_t(struct listNode_t* head){


    if(head == NULL){
        return NULL;
    }
    return getNode_t(head,0);

}

listNode_t* copyList(struct listNode_t* head)
{
    if (head == NULL) {
        return NULL;
    }
    else {
        struct listNode_t* newNode
                = (struct listNode_t*)malloc(
                        sizeof(struct listNode_t));
        newNode->dataInfo = head->dataInfo;

        newNode->next = copyList(head->next);

        return newNode;
    }
}