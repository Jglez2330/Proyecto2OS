//
// Created by jglez2330 on 2/5/21.
//

#include <malloc.h>
#include <memory.h>
#include <bits/sigstack.h>
#include "CEThread.h"
int globalTID = 0;

void gtthread_exit(void *pVoid);

int CEThread_create(CEThread_t* thread, CEThread_attr_t *attr, void* rutine, void* arg){

    //Creates the thread information and fills it
    CEThread_treadInfo* ceThreadTreadInfo = malloc(sizeof(CEThread_treadInfo));
    *thread = globalTID;
    ceThreadTreadInfo->tid = globalTID++;
    ceThreadTreadInfo->state = READY;
    ceThreadTreadInfo->pFunction = rutine;
    ceThreadTreadInfo->arg = arg;
    ceThreadTreadInfo->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
    ceThreadTreadInfo->joining = 0;
    ceThreadTreadInfo->attributes = attr;
    memset(ceThreadTreadInfo->thread_context, "\0", sizeof(ucontext_t));


    //Get program context
    if (getcontext(ceThreadTreadInfo->thread_context) == -1){
        printf("Unable to get program context");
        return -1;
    }

    //Sets program context
    ceThreadTreadInfo->thread_context->uc_stack.ss_sp = malloc(SIGSTKSZ);
    ceThreadTreadInfo->thread_context->uc_stack.ss_size = SIGSTKSZ;
    ceThreadTreadInfo->thread_context->uc_stack.ss_flags = 0;
    ceThreadTreadInfo->thread_context->uc_link = NULL;

    //makecontext(context, thread_starting_function, arguments_quantities, argument1, argument2, argument3)
    makecontext(ceThreadTreadInfo->thread_context, (void (*)(void)) CEThread_start, 2, rutine, arg);

    //TODO: place thread into the ready list

    return 0;

}

void CEThread_start(void* (*start_routine)(void*), void* args){
    void* r = (*start_routine)(args);
    gtthread_exit(r);
}

void gtthread_exit(void *pVoid) {

}

int main(){

}