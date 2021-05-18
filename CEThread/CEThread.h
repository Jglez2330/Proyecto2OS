//
// Created by jglez2330 on 17/5/21.
//

#ifndef PROYECTO2OS_CETHREAD_H
#define PROYECTO2OS_CETHREAD_H
#include "CEThread_List.h"
#include <memory.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
enum process_state {READY = 0, RUNNING = 1, BLOCKED = 2, TERMINATED = 3};

typedef struct {
    CEThread_t owner_thread;
    listNode_t* blocked_list;
}CEThread_mutex_t;
typedef struct {
    CEThread_t owner_thread;

}CEThread_attr_t;
void CEThread_get_main_thread_context();
int CEThread_create(CEThread_t* thread,CEThread_attr_t* attr ,void *(*start_routine)(void*), void *arg);
void CEThread_end(void *);
int CEThread_yield();
int CEThread_join(CEThread_t thread, void ** return_value);
int CEThread_detach(CEThread_t thread);
int CEThread_mutex_init(CEThread_mutex_t* mutex);
int CEThread_mutex_destroy(CEThread_mutex_t* mutex);
int CEThread_mutex_lock(CEThread_mutex_t* mutex);
int CEThread_mutex_unlock(CEThread_mutex_t* mutex);
void CEThread_start(void* (*start_routine)(void*), void* args);
void context_switching(int sig);
CEThread_treadInfo* get_thread_by_tid(CEThread_t tid);
void unblock_threads_from_list(listNode_t* list);
CEThread_treadInfo* get_thread_by_tid_zombie(CEThread_t thread);
#endif //PROYECTO2OS_CETHREAD_H
