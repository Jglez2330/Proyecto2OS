//
// Created by jglez2330 on 17/5/21.
//

#include "CEThread.h"
#define QUANTUM 1000
#define Channels 4
long globalTID = 0;
int current_channel;
CEThread_treadInfo* main_thread;
CEThread_treadInfo* current_running_thread;
sigset_t context_switching_alarm;
static struct itimerval timer_context;

CEThread_treadInfo *get_next_thread();

scheduler_t* schedulers[Channels];

void CEThread_get_main_thread_context() {
    struct sigaction act;


    main_thread = malloc(sizeof(CEThread_treadInfo));
    memset(main_thread, 0, sizeof(CEThread_treadInfo));

    main_thread->tid = globalTID++;
    main_thread->state = RUNNING;
    main_thread->arg = NULL;
    main_thread->joining = 0;
    main_thread->detach = 0;

    main_thread->thread_context = (ucontext_t*) malloc(sizeof(ucontext_t));
    memset(main_thread->thread_context, 0, sizeof(ucontext_t));


    if (getcontext(main_thread->thread_context) == -1)
    {
        perror("Unable to get main context");
        exit(EXIT_FAILURE);
    }

    current_running_thread = main_thread;

    sigemptyset(&context_switching_alarm);
    sigaddset(&context_switching_alarm, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);

    memset(&timer_context, 0, sizeof(struct itimerval));
    timer_context.it_interval.tv_usec = QUANTUM;
    timer_context.it_value.tv_usec = QUANTUM;

    if (setitimer(ITIMER_VIRTUAL, &timer_context, NULL) < 0)
    {
        perror("Unable to set timer alarm");
        exit(EXIT_FAILURE);
    }

    /* install signal handler for SIGVTALRM */
    memset(&act, 0, sizeof(act));
    act.sa_handler = &context_switching;
    if (sigaction(SIGVTALRM, &act, NULL) < 0)
    {
        perror ("Unable to install handler");
        exit(EXIT_FAILURE);
    }
}

int CEThread_create(CEThread_t* thread_id,CEThread_attr_t* attr ,void *(*start_routine)(void*), void *arg, scheduler_t* scheduler){

    if (globalTID == 0){
        CEThread_get_main_thread_context();
        globalTID++;
        current_channel = 0;
        schedulers[Channels - 1] = malloc(sizeof(scheduler_t));
        schedulers[Channels - 1]->ant_list_ready_a = NULL;
        schedulers[Channels - 1]->zombie_ants_a = NULL;
    }
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);

    CEThread_treadInfo* new_thread = (CEThread_treadInfo* ) malloc(sizeof(CEThread_treadInfo));
    new_thread->tid = globalTID++;
    *thread_id = new_thread->tid;
    new_thread->state = READY;
    new_thread->pFunction = start_routine;
    new_thread->arg = arg;
    new_thread->joining = 0;

    new_thread->thread_context = (ucontext_t*) malloc(sizeof(ucontext_t));
    memset(new_thread->thread_context, 0, sizeof(ucontext_t));

    if (getcontext(new_thread->thread_context) == -1){
        perror("Unable to get context for thread");
        exit(EXIT_FAILURE);
    }

    new_thread->thread_context->uc_stack.ss_sp = malloc(SIGSTKSZ);
    new_thread->thread_context->uc_stack.ss_size = SIGSTKSZ;
    new_thread->thread_context->uc_stack.ss_flags = 0;
    new_thread->thread_context->uc_link = NULL;


    makecontext(new_thread->thread_context, (void (*)(void)) CEThread_start, 2, start_routine, arg);

    append(&scheduler->ant_list_ready_a, new_thread);

    schedulers[scheduler->canalNumber] = scheduler;

    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
}

void CEThread_start(void* (*start_routine)(void*), void* args){
    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);

    void * return_value = (*start_routine)(args);

    CEThread_end(return_value);
}

int CEThread_yield(){
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);
    CEThread_treadInfo* next = get_next_thread();
    CEThread_treadInfo* prev = current_running_thread;
    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
    if (prev->thread_context == NULL){
        setcontext(main_thread->thread_context);
    } else{
        swapcontext(prev->thread_context, next->thread_context);
    }


    return 0;
}

CEThread_treadInfo *get_next_thread() {
    CEThread_treadInfo* result;
    do{
        current_channel++;
        current_channel = current_channel % Channels;
    } while (schedulers[current_channel] == NULL && current_channel != 0);
    if (current_channel == 0){
        return main_thread;
    } else{
        listNode_t * list_threads = (*schedulers[current_channel]->funcion_calendarizador)(schedulers[current_channel]);
        if (getFront_t(list_threads) == NULL){
            return main_thread;
        }
        return getFront_t(list_threads);
    }
}
void context_switching(int sig){
    /* block the signal */
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);



    CEThread_treadInfo * next = get_next_thread();

    CEThread_treadInfo * prev = current_running_thread;
    if (prev->state == RUNNING){
        prev->state = READY;
    }

    next->state = RUNNING;
    current_running_thread = next;

    /* unblock the signal */
    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
    swapcontext(prev->thread_context, current_running_thread->thread_context);
}

void CEThread_end(void* return_value){
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);



    CEThread_treadInfo * prev = current_running_thread;
    current_running_thread = get_next_thread();
    current_running_thread->state = RUNNING;
    if (current_running_thread->tid == prev->tid){
        sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
        setcontext(current_running_thread->thread_context);
        return;
    }else {

        free(prev->thread_context->uc_stack.ss_sp);
        free(prev->thread_context);
        prev->thread_context = NULL;

        prev->state = TERMINATED;
        prev->retval = return_value;
        prev->joining = 0;

        append(&schedulers[current_channel]->zombie_ants_a, prev);

        sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
        setcontext(current_running_thread->thread_context);
    }
}

