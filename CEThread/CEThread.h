//
// Created by jglez2330 on 17/5/21.
//

#ifndef PROYECTO2OS_CETHREAD_H
#define PROYECTO2OS_CETHREAD_H
#include "../Scheduler/Scheduler.h"
#include <memory.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
typedef struct {
    CEThread_t owner_thread;
}CEThread_mutex_t;
typedef struct {
    CEThread_t owner_thread;
}CEThread_attr_t;
void CEThread_get_main_thread_context();
int CEThread_create(CEThread_t* thread,CEThread_attr_t* attr ,void *(*start_routine)(void*), void *arg, scheduler_t* scheduler);
void CEThread_end(void *);
int CEThread_yield();
int CEThread_join(CEThread_t thread, void **);
int CEThread_detach(CEThread_t thread);
int CEThread_mutex_init(CEThread_mutex_t* mutex);
int CEThread_mutex_destroy(CEThread_mutex_t* mutex);
int CEThread_mutex_lock(CEThread_mutex_t* mutex);
int CEThread_mutex_unlock(CEThread_mutex_t* mutex);
void CEThread_start(void* (*start_routine)(void*), void* args);
void context_switching(int sig);

#endif //PROYECTO2OS_CETHREAD_H
