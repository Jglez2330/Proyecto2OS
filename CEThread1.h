//
// Created by jglez2330 on 2/5/21.
//
#include <ucontext.h>
#include <malloc.h>
#include <memory.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include "Scheduler/Scheduler.h"



#ifndef PROYECTO2OS_CETHREAD_H
#define PROYECTO2OS_CETHREAD_H

typedef unsigned long int CEThread_t;

typedef struct {
    listNode_t * blocked_list;
    CEThread_t owner_thread;
}CEThread_mutex_t;
typedef struct {
    int tid;
}CEThread_mutex_attr_t;


typedef struct {
    long priority;
    long quantum;
    void (*alarm_timer_handler)(int);
    listNode_t * thread_canal;
} CEThread_attr_t;

static struct itimerval thread_timer;
sigset_t alarm_timeout_thread;
//static CEThread_treadInfo * current_thread_running;

void default_algo (int sig);



void CEThread_end(void *pVoid);

/*
int CEThread_create(CEThread_t *thread, void *rutine, void *arg, scheduler_t* scheduler, int channel);
void CEThread_start(void* (*start_routine)(void*), void* args);
CEThread_attr_t* CEThread_default_attr();
void swap_context_algorithm(int sig);
int CEThread_join(CEThread_t thread, void** return_value);
CEThread_treadInfo* get_thread(CEThread_t thread, listNode_t * thread_list_local);
int CEThread_yield();
int CEThread_detach(CEThread_t thread);
void CEThread_mutex_init(CEThread_mutex_t* mutex, CEThread_mutex_attr_t* attr);
void CEThread_mutex_lock(CEThread_mutex_t* mutex);
void CEThread_mutex_unlock(CEThread_mutex_t* mutex);
int CEThread_mutex_destroy(CEThread_mutex_t* mutex);
void unblock_threads_from_list(listNode_t* list);
CEThread_treadInfo * get_thread_zombie(CEThread_t thread);
CEThread_treadInfo * get_thread_ready(CEThread_t thread);
*/
 #endif //PROYECTO2OS_CETHREAD_H
