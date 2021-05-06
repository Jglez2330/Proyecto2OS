//
// Created by jglez2330 on 2/5/21.
//

#include <malloc.h>
#include <memory.h>
#include <bits/sigstack.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include "CEThread.h"
int globalTID = 0;
static struct itimerval thread_timer;
static CEThread_treadInfo * current_thread_running;
sigset_t alarm_timeout_thread;
void gtthread_exit(void *pVoid);

int CEThread_create(CEThread_t* thread, CEThread_attr_t *attr, void* rutine, void* arg){
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    if (attr == NULL){
        attr = CEThread_default_attr();
    }
    if (globalTID++ == 0){
        struct sigaction act;
        //TODO: inicializar la lista de listos
        CEThread_treadInfo* main_thread_info = (CEThread_treadInfo*)malloc(sizeof(CEThread_t));
        main_thread_info->tid = globalTID;
        main_thread_info->thread_context = (ucontext_t*) malloc(sizeof(ucontext_t));
        memset(main_thread_info->thread_context, "\0", sizeof(ucontext_t));
        main_thread_info->arg = NULL;
        main_thread_info->state = RUNNING;
        main_thread_info->joining = 0;
        if (getcontext(main_thread_info->thread_context) == -1){
            printf("Unable to get program context for main context");
            exit(-1);
        }
        current_thread_running = main_thread_info;

        sigemptyset(&alarm_timeout_thread);
        sigaddset(&alarm_timeout_thread, SIGVTALRM);
        sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);

        /* set alarm signal and signal handler */
        memset(&thread_timer, 0, sizeof(thread_timer));
        thread_timer.it_interval.tv_usec = attr->quantum;
        thread_timer.it_value.tv_usec = attr->quantum;

        if(setitimer(ITIMER_VIRTUAL, &thread_timer, NULL)< 0){
            printf("Unable to set quantum for thread process");
            exit(-1);
        }
        memset(&act, "\0", sizeof(act));
        act.sa_handler = attr->alarm_timer_handler;

        if(sigaction(SIGVTALRM, &act, NULL) < 0){
            printf("Unable to plant action to timer");
            exit(-1);
        }


    }
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
        exit(-1);
    }

    //Sets program context
    ceThreadTreadInfo->thread_context->uc_stack.ss_sp = malloc(SIGSTKSZ);
    ceThreadTreadInfo->thread_context->uc_stack.ss_size = SIGSTKSZ;
    ceThreadTreadInfo->thread_context->uc_stack.ss_flags = 0;
    ceThreadTreadInfo->thread_context->uc_link = NULL;

    //makecontext(context, thread_starting_function, arguments_quantities, argument1, argument2, argument3)
    makecontext(ceThreadTreadInfo->thread_context, (void (*)(void)) CEThread_start, 2, rutine, arg);

    //TODO: place thread into the ready list

    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    return 0;

}

void CEThread_start(void* (*start_routine)(void*), void* args){
    void* r = (*start_routine)(args);
    gtthread_exit(r);
}

CEThread_attr_t* CEThread_default_attr(){
    CEThread_attr_t* attr = (CEThread_attr_t*) malloc(sizeof(CEThread_attr_t));
    attr->quantum = 200;
    attr->priority = 1;
    attr->alarm_timer_handler = ;


}


void gtthread_exit(void *pVoid) {

}

int main(){

}