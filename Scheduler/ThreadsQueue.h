//
// Created by leahycarlos21 on 2/5/21.
//

#ifndef SCHEDULER_THREADSQUEUE_H
#define SCHEDULER_THREADSQUEUE_H

typedef void* queue_item;

typedef struct queue_node_t{
    queue_item item;
    struct queue_node_t* next;
} queue_node_t;

typedef struct {
    queue_node_t* front;
    queue_node_t* back;
    int N;
}queue_t;


//---------Initializes
void queueInit(queue_t* this);

//----------Return 1 if is empty
int queueIsempty(queue_t* this);

//-----------return queue size
int queueSize(queue_t* this);

//----------- Adds an item to the "back" of the queue
void queueAddBackItem(queue_t* this, queue_item item);

//------------ Adds an element to the "front" of the queue
void queueAddFrontItem(queue_t* this, queue_item item);

//------------ Removes an element to the "front" of the queue
queue_item queueRemoveFrontItem(queue_t* this);

//------------- Removes the element on the "front" to the "back" of the queue
void queue_cycle(queue_t* this);

// ------------Returns the element at the "front" of the steque without removing it
queue_item queue_Getfront(queue_t* this);

// ------------Empties the queue and performs any necessary memory cleanup
void queue_destroy(queue_t* this);



#endif //SCHEDULER_THREADSQUEUE_H
