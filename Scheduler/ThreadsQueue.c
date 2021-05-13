#include <stdlib.h>
#include <stdio.h>
#include "ThreadsQueue.h"

//---------Initializes
void queueInit(queue_t* this){
    this->front =NULL;
    this->back = NULL;
    this->N = 0;
}

//----------Return 1 if is empty
int queueIsempty(queue_t* this){
    return this->N == 0;
}

//-----------return queue size
int queueSize(queue_t* this){
    return this->N;
}

//----------- Adds an item to the "back" of the queue ---steque_enqueue
void queueAddBackItem(queue_t* this, queue_item item){
    queue_node_t* node;
    node = (queue_node_t*) malloc(sizeof(queue_node_t));
    node->item = item;
    node->next = NULL;

    if(this->back == NULL)
        this->front = node;
    else
        this->back->next = node;

    this->back = node;
    this->N++;
}

//------------ Adds an element to the "front" of the queue
void queueAddFrontItem(queue_t* this, queue_item item){
    queue_node_t* node;

    node = (queue_node_t*) malloc(sizeof(queue_node_t));
    node->item = item;
    node->next = this->front;

    if(this->back == NULL)
        this->back = node;

    this->front = node;
    this->N++;
}

//------------ Removes an element to the "front" of the queue
queue_item queueRemoveFrontItem(queue_t* this){
    queue_item ans;
    queue_node_t* node;

    if(this->front == NULL){
        fprintf(stderr, "Error: underflow in steque_pop.\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    node = this->front;
    ans = node->item;

    this->front = this->front->next;
    if (this->front == NULL) this->back = NULL;
    free(node);

    this->N--;

    return ans;
}

//------------- Removes the element on the "front" to the "back" of the queue
void queue_cycle(queue_t* this){
    if(this->back == NULL)
        return;

    this->back->next = this->front;
    this->back = this->front;
    this->front = this->front->next;
    this->back->next = NULL;
}

// ------------Returns the element at the "front" of the steque without removing it
queue_item queue_Getfront(queue_t* this){
    if(this->front == NULL){
        fprintf(stderr, "Error: underflow in steque_front.\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    return this->front->item;
}

// ------------Empties the queue and performs any necessary memory cleanup
void queue_destroy(queue_t* this){
    while(!queueIsempty(this))
        queueRemoveFrontItem(this);
}



queue_item queue_GetPosition(queue_t* this,int position){
    queue_t* actual = this;
    int num = 0;

    while(actual!= NULL){
        if(num == position)
            return actual->back->item;
        num++;
        actual->back->next = actual->back ;
    }

}
