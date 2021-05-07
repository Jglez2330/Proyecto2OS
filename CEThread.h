//
// Created by jglez2330 on 2/5/21.
//
#include <ucontext.h>
#include <malloc.h>
#include <memory.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include "Scheduler/ThreadsQueue.h"


#ifndef PROYECTO2OS_CETHREAD_H
#define PROYECTO2OS_CETHREAD_H

typedef unsigned long int CEThread_t;



typedef struct {
    long priority;
    long quantum;
    void (*alarm_timer_handler)(int);
    queue_t* thread_canal;
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


static struct itimerval thread_timer;
sigset_t alarm_timeout_thread;
sigset_t alarm_main_thread;
static CEThread_treadInfo * current_thread_running;

void default_algo (int sig);



void gtthread_exit(void *pVoid);


int CEThread_create(CEThread_t* thread, CEThread_attr_t *attr, void* rutine, void* arg);

void CEThread_start(void* (*start_routine)(void*), void* args);
CEThread_attr_t* CEThread_default_attr();
void swap_context_algorithm(int sig);
int CEThread_join(CEThread_t thread, void** return_value);
CEThread_treadInfo* get_thread(CEThread_t thread, queue_t* thread_list_local);

#endif //PROYECTO2OS_CETHREAD_H
