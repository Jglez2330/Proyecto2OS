//
// Created by jglez2330 on 17/5/21.
//

#include "CEThread_List.h"

void push_t_thread(struct listNode_t **start_ref, CEThread_treadInfo * threadInfo)
{
    struct listNode_t *ptr1 = (struct listNode_t*)malloc(sizeof(struct listNode_t));
    ptr1->threadInfo = threadInfo;
    ptr1->next = *start_ref;
    *start_ref = ptr1;
}
void append_thread(struct listNode_t** head_ref, CEThread_treadInfo * threadInfo)
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
void deleteNodePosition_thread(struct listNode_t **head_ref, int position)
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

void deleteNodeTID_t_thread(struct listNode_t** head_ref, CEThread_t key)
{
    int count = 0;

    struct listNode_t* temp = *head_ref;
    while (temp->threadInfo->tid!= key && temp!=NULL)
    {
        temp = temp->next;
        count ++;
    }
    deleteNodePosition_thread(head_ref, count);
}

void deleteList_thread(struct listNode_t** head_ref){
    struct listNode_t* current = *head_ref;
    struct listNode_t* next;
    while (current != NULL){
        next = current->next;
        free(current);
        current = next;
    }

    *head_ref = NULL;
}

CEThread_treadInfo* getNode_t_thread(struct listNode_t* head, int index){

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





void swap_thread(struct listNode_t *a, struct listNode_t *b){

    CEThread_treadInfo *temp   = a->threadInfo;
    a->threadInfo = b->threadInfo;
    b->threadInfo = temp;
}

int getCount_t_thread(struct listNode_t* head)
{
    int count = 0;  // Initialize count
    struct listNode_t* current = head;  // Initialize current
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}
/*void listCycle_t_thread(struct listNode_t** head){
    CEThread_treadInfo* listItem= getNode_t_thread(*head ,  0);

    append_thread(head,listItem);
    deleteNodePosition_thread(head,0);
}*/

void listCycle_t_thread(struct listNode_t** head){
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


CEThread_treadInfo* getFront_t_thread(struct listNode_t* head){

    if(head == NULL){
        return NULL;
    }
    return getNode_t_thread(head, 0);

}