//
// Created by jglez2330 on 2/5/21.
//
#include <ucontext.h>
#ifndef PROYECTO2OS_CETHREAD_H
#define PROYECTO2OS_CETHREAD_H
typedef unsigned long int CEThread_t;

typedef struct {
    long priority;
    long quantum;
} CEThread_attr_t;

enum State {READY = 0, RUNNING = 1, BLOCKED = 2, TERMINATED = 3};
typedef struct Thread_t{
    CEThread_t tid;
    CEThread_t joining;
    int state;
    void* (*pFunction)(void*);
    void* arg;
    void* retval;
    CEThread_attr_t* attributes;
    ucontext_t* thread_context;
} CEThread_treadInfo;

int CEThread_create(CEThread_t* thread, CEThread_attr_t *attr, void* rutine, void* arg);

void CEThread_start(void* (*start_routine)(void*), void* args);
#endif //PROYECTO2OS_CETHREAD_H
