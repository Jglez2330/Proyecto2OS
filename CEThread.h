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
    queue_t* blocked_list;
    CEThread_t owner_thread;
}CEThread_mutex_t;
typedef struct {
    int tid;
}CEThread_mutex_attr_t;


typedef struct {
    long priority;
    long quantum;
    void (*alarm_timer_handler)(int);
    queue_t* thread_canal;
} CEThread_attr_t;

enum State {READY = 0, RUNNING = 1, BLOCKED = 2, TERMINATED = 3};
typedef struct{
    CEThread_t tid;
    CEThread_t joining;
    int state;
    int detach;
    void* (*pFunction)(void*);
    void* arg;
    void* retval;
    CEThread_attr_t* attributes;
    ucontext_t* thread_context;
} CEThread_treadInfo;


static struct itimerval thread_timer;
sigset_t alarm_timeout_thread;
static CEThread_treadInfo * current_thread_running;

void default_algo (int sig);



void CEThread_exit(void *pVoid);


int CEThread_create(CEThread_t* thread, CEThread_attr_t *attr, void* rutine, void* arg);

void CEThread_start(void* (*start_routine)(void*), void* args);
CEThread_attr_t* CEThread_default_attr();
void swap_context_algorithm(int sig);
int CEThread_join(CEThread_t thread, void** return_value);
CEThread_treadInfo* get_thread(CEThread_t thread, queue_t* thread_list_local);
int CEThread_yield();
int CEThread_detach(CEThread_t thread);
void CEThread_mutex_init(CEThread_mutex_t* mutex, CEThread_mutex_attr_t* attr);
void CEThread_mutex_lock(CEThread_mutex_t* mutex);
void CEThread_mutex_unlock(CEThread_mutex_t* mutex);
int CEThread_mutex_destroy(CEThread_mutex_t* mutex);
#endif //PROYECTO2OS_CETHREAD_H
