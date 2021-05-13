//
// Created by jglez2330 on 2/5/21.
//
#define QUANTUM 100000
#define Channels 4
#include "CEThread.h"


int globalTID = 0;
int current_channel = 0;
scheduler_t schedulers[Channels];
void free_thread(CEThread_treadInfo *pThread);

CEThread_treadInfo *get_next_thread();

int CEThread_create(CEThread_t *thread, void *rutine, void *arg, scheduler_t* scheduler, int channel) {

    if (globalTID++ == 0) {
        struct sigaction act;
        //TODO: inicializar la lista de listos
        CEThread_treadInfo *main_thread_info = (CEThread_treadInfo *) malloc(sizeof(CEThread_treadInfo));
        main_thread_info->tid = globalTID;
        main_thread_info->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
        main_thread_info->arg = NULL;
        main_thread_info->state = RUNNING;
        main_thread_info->joining = 0;
        memset(main_thread_info->thread_context, '\0', sizeof(ucontext_t));

        if (getcontext(main_thread_info->thread_context) == -1) {
            printf("Unable to get program context for main thread");
            exit(-1);
        }
        current_thread_running = main_thread_info;

        sigemptyset(&alarm_timeout_thread);
        sigaddset(&alarm_timeout_thread, SIGVTALRM);
        sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);


        memset(&thread_timer, 0, sizeof(thread_timer));
        thread_timer.it_interval.tv_usec = QUANTUM;
        thread_timer.it_value.tv_usec = QUANTUM;

        if (setitimer(ITIMER_VIRTUAL, &thread_timer, NULL) < 0) {
            printf("Unable to set quantum for thread process");
            exit(-1);
        }

        memset(&act, '\0', sizeof(act));
        act.sa_handler = default_algo;

        if (sigaction(SIGVTALRM, &act, NULL) < 0) {
            printf("Unable to plant action to timer");
            exit(-1);
        }

        scheduler_t * scheduler_temp = malloc(sizeof(scheduler_t)*4);

        for (int i = 0; i < Channels; ++i) {
            scheduler_temp[i].ant_list_ready_a = NULL;
            scheduler_temp[i].ant_list_ready_b = NULL;
            scheduler_temp[i].zombie_ants_a = NULL;
            scheduler_temp[i].zombie_ants_b = NULL;
        }


        append(&schedulers[0].ant_list_ready_a, main_thread_info);


        globalTID++;
    }
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    //Creates the thread information and fills it
    CEThread_treadInfo *ceThreadTreadInfo = malloc(sizeof(CEThread_treadInfo));
    *thread = globalTID;
    ceThreadTreadInfo->tid = globalTID;
    ceThreadTreadInfo->state = READY;
    ceThreadTreadInfo->pFunction = rutine;
    ceThreadTreadInfo->arg = arg;
    ceThreadTreadInfo->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
    ceThreadTreadInfo->joining = 0;
    schedulers[channel] = *scheduler;
    memset(ceThreadTreadInfo->thread_context, '\0', sizeof(ucontext_t));


    //Get program context
    if (getcontext(ceThreadTreadInfo->thread_context) == -1) {
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

    append(&schedulers[channel].ant_list_ready_a, ceThreadTreadInfo);
    schedulers[channel].funcion_calendarizador = scheduler->funcion_calendarizador;
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    return 0;

}

void CEThread_start(void *(*start_routine)(void *), void *args) {
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    void *r = (*start_routine)(args);
    CEThread_end(r);
}

CEThread_attr_t *CEThread_default_attr() {
    CEThread_attr_t *attr = (CEThread_attr_t *) malloc(sizeof(CEThread_attr_t));
    attr->quantum = 2;
    attr->priority = 1;
    attr->alarm_timer_handler = default_algo;


    return attr;
}

void default_algo(int sig) {
    /* block the signal */
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    current_channel++;
    current_channel = current_channel % 2;
    CEThread_treadInfo *prev;
    CEThread_treadInfo *next;
    if (current_channel == 0){
        //TODO: Run main thread
        prev = current_thread_running;
        if (current_thread_running->state == RUNNING) {
            current_thread_running->state = READY;

        }
        next = getFront_t(schedulers[0].ant_list_ready_a);
        next->state = RUNNING;
        current_thread_running = next;
        sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
        swapcontext(prev->thread_context, current_thread_running->thread_context);
        return;
    }
    /* if no thread in the ready queue, resume execution */
    listNode_t * list_threads = (*schedulers[current_channel].funcion_calendarizador)(&schedulers[current_channel]);
    if (getFront_t(list_threads) == NULL)
        return;

    /* get the next runnable thread and use preemptive scheduling */
    prev = current_thread_running;
    if (current_thread_running->state == RUNNING) {
        current_thread_running->state = READY;
    }
    //TODO: LLamar su fucnion de calendarizador
    next = getFront_t(list_threads);

    next->state = RUNNING;
    current_thread_running = next;

    /* unblock the signal */
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    swapcontext(prev->thread_context, current_thread_running->thread_context);

}

void CEThread_end(void *pVoid) {
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    listNode_t * list_threads = (*schedulers[current_channel].funcion_calendarizador)(&schedulers[current_channel]);
    if (getFront_t(list_threads) == NULL) {
        sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
        exit((int) pVoid);
    }

    /* if the main thread call CEThread_end */
    if (current_thread_running->tid == 1) {
        while (!(getFront_t(list_threads) == NULL)) {
            sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
            default_algo(SIGVTALRM);
            sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
        }
        sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
        exit((long) pVoid);
    }

    if (current_thread_running->detach == 1) {
        //TODO: Free resources
    }

    CEThread_treadInfo *prev = current_thread_running;
    prev->state = TERMINATED;


    current_thread_running = get_next_thread();
    current_thread_running->state = RUNNING;

    /* free up memory allocated for exit thread */
    free(prev->thread_context->uc_stack.ss_sp);
    free(prev->thread_context);
    prev->thread_context = NULL;

    /* mark the exit thread as DONE and add to zombie_queue */
    prev->retval = pVoid;
    prev->joining = 0;

    /* unblock alarm signal and setcontext for next thread */
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    setcontext(current_thread_running->thread_context);
    return;
}

CEThread_treadInfo *get_next_thread() {
    current_channel++;
    current_channel = current_channel % 2;
    CEThread_treadInfo *next = NULL;
    if (current_channel == 0){
        //TODO: Run main thread
        next = getFront_t(schedulers[0].ant_list_ready_a);
        return next;
    }
    /* if no thread in the ready queue, resume execution */
    listNode_t * list_threads = (*schedulers[current_channel].funcion_calendarizador)(&schedulers[current_channel]);
    if (getFront_t(list_threads) == NULL)
        return NULL;
    next = getFront_t(list_threads);

    return next;
}

int CEThread_join(CEThread_t thread, void **return_value) {
    if (thread == current_thread_running->tid) {
        printf("Unable to join to same thread");
        return -1;
    }
    CEThread_treadInfo *threadTreadInfo ;
    if ((threadTreadInfo = get_thread_zombie(thread)) != NULL) {

        free_thread(threadTreadInfo);
        //TODO: Delete thread
        return 0;
    }
    if ((threadTreadInfo = get_thread_ready(thread)) == NULL) {
        printf("Thread %lu doesn't exist \n", thread);
        return -1;
    }


    if (threadTreadInfo->joining == current_thread_running->tid) {
        printf("Thread already joining");
        return -1;
    }
    if (threadTreadInfo->detach == 1) {
        printf("Thread is detached and cannot be joined");
        return -1;
    }
    current_thread_running->joining = threadTreadInfo->tid;
    while (threadTreadInfo->state != TERMINATED) {
        sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
        default_algo(SIGVTALRM);
        sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    }
    if (return_value == NULL) {
        return 0;
    }

    return 0;
}

void free_thread(CEThread_treadInfo *pThread) {
    return;
}
CEThread_treadInfo * get_thread(CEThread_t thread, listNode_t* list){
    CEThread_treadInfo* result = NULL;
    for (int i = 0; i < getCount_t(list); i++) {
        if (getNode_t(list,i)->tid == thread){
            result = getNode_t(list,i);
            break;
        }

    }
    return result;
}
CEThread_treadInfo * get_thread_zombie(CEThread_t thread){
    CEThread_treadInfo *result = NULL;
    listNode_t* list;
    for (int i = 0; i < Channels; i++){
        scheduler_t selected_channel = schedulers[i];

        if ((result = get_thread(thread, selected_channel.zombie_ants_a)) != NULL){
            break;
        }
        else if ((result = get_thread(thread, selected_channel.zombie_ants_b)) != NULL){
            break;
        }

    }
    return result;
}
CEThread_treadInfo * get_thread_ready(CEThread_t thread){
    CEThread_treadInfo *result = NULL;
    listNode_t* list;
    for (int i = 0; i < Channels; i++){
        scheduler_t selected_channel = schedulers[i];

        if ((result = get_thread(thread, selected_channel.ant_list_ready_a)) != NULL){
            break;
        }
        else if ((result = get_thread(thread, selected_channel.ant_list_ready_b)) != NULL){
            break;
        }

    }
    return result;
}

int CEThread_yield() {
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    listNode_t * list_threads = (*schedulers[current_channel].funcion_calendarizador)(&schedulers[current_channel]);

    if (getFront_t(list_threads) == NULL) {
        return -1;
    }
    default_algo(SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    return 0;
};

int CEThread_detach(CEThread_t thread) {
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    CEThread_treadInfo *ceThreadTreadInfo = get_thread_ready(thread);
    ceThreadTreadInfo->detach = 1;
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    return 0;
};

void CEThread_mutex_init(CEThread_mutex_t *mutex, CEThread_mutex_attr_t *attr) {
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    mutex->blocked_list = malloc(sizeof(listNode_t));
    mutex->blocked_list = NULL;
    //TODO:Cahbge list
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
};

void CEThread_mutex_lock(CEThread_mutex_t *mutex) {
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    if (mutex->owner_thread != current_thread_running->tid && mutex->owner_thread != 0) {
        append(&mutex->blocked_list, current_thread_running);
        current_thread_running->state = BLOCKED;
        CEThread_yield();
    } else {
        mutex->owner_thread = current_thread_running->tid;
    }
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
};

void CEThread_mutex_unlock(CEThread_mutex_t *mutex) {
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    if (mutex->owner_thread != current_thread_running->tid && mutex->owner_thread != 0) {
        printf("Only the lock owner can free the mutex");
    } else {
        unblock_threads_from_list(mutex->blocked_list);
        mutex->owner_thread = 0;
    }
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
}

int CEThread_mutex_destroy(CEThread_mutex_t* mutex){
    if (mutex->owner_thread != 0){
        printf("Lock has not been unlocked");
        return -1;
    } else{

    }

    return 0;
}

void unblock_threads_from_list(listNode_t* list) {
    while (getFront_t(list)!= NULL) {
        CEThread_treadInfo *r = getFront_t(list);
        r->state = READY;
        deleteNodePosition(&list, 0);
    }
};