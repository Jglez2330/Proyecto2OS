//
// Created by jglez2330 on 2/5/21.
//
#define QUANTUM 100000

#include "CEThread.h"


queue_t *thread_list = NULL;
queue_t *thread_list_zombie = NULL;
int globalTID = 0;
int current_channel = 0;

void free_thread(CEThread_treadInfo *pThread);

void unblock_threads_from_list(queue_t *list);

typedef struct {
    queue_t *(*funcion_calendarizador)(queue_t *, queue_t *);

    queue_t *ant_list_ready_a;
    queue_t *ant_list_ready_b;
    queue_t *zombie_ants_a;
    queue_t *zombie_ants_b;
} scheduler_t;

int CEThread_create(CEThread_t *thread, CEThread_attr_t *attr, void *rutine, void *arg) {
    if (attr == NULL) {
        attr = CEThread_default_attr();
    }
    if (globalTID++ == 0) {
        struct sigaction act;
        //TODO: inicializar la lista de listos
        CEThread_treadInfo *main_thread_info = (CEThread_treadInfo *) malloc(sizeof(CEThread_treadInfo));
        main_thread_info->tid = globalTID;
        main_thread_info->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
        main_thread_info->arg = NULL;
        main_thread_info->state = RUNNING;
        main_thread_info->joining = 0;
        main_thread_info->attributes = attr;
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
        thread_list = (queue_t *) malloc(sizeof(queue_t));
        thread_list_zombie = (queue_t *) malloc(sizeof(queue_t));
        queueInit(thread_list);
        queueInit(thread_list_zombie);

        queueAddFrontItem(thread_list, current_thread_running);
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
    ceThreadTreadInfo->attributes = attr;
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

    queueAddBackItem(thread_list, ceThreadTreadInfo);
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    return 0;

}

void CEThread_start(void *(*start_routine)(void *), void *args) {
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    void *r = (*start_routine)(args);
    CEThread_exit(r);
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
    current_channel = current_channel % 4;

    /* if no thread in the ready queue, resume execution */
    if (queueIsempty(thread_list))
        return;

    /* get the next runnable thread and use preemptive scheduling */
    CEThread_treadInfo *prev = current_thread_running;
    if (current_thread_running->state == RUNNING) {
        current_thread_running->state = READY;
    }
    //TODO: LLamar su fucnion de calendarizador
    CEThread_treadInfo *next;
    do {
        queue_cycle(thread_list);
        next = (CEThread_treadInfo *) queue_Getfront(thread_list);
    } while (next->state != READY);


    next->state = RUNNING;
    current_thread_running = next;

    /* unblock the signal */
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    swapcontext(prev->thread_context, current_thread_running->thread_context);

}

void CEThread_exit(void *pVoid) {


    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);

    if (queueIsempty(thread_list)) {
        sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
        exit((int) pVoid);
    }

    /* if the main thread call CEThread_exit */
    if (current_thread_running->tid == 1) {
        while (!queueIsempty(thread_list)) {
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

    CEThread_treadInfo *prev = (CEThread_treadInfo *) queueRemoveFrontItem(thread_list);
    CEThread_treadInfo *next;

    do {
        queue_cycle(thread_list);
        next = (CEThread_treadInfo *) queue_Getfront(thread_list);
    } while (next->state != READY);
    current_thread_running = next;
    current_thread_running->state = RUNNING;

    /* free up memory allocated for exit thread */
    free(prev->thread_context->uc_stack.ss_sp);
    free(prev->thread_context);
    prev->thread_context = NULL;

    /* mark the exit thread as DONE and add to zombie_queue */
    prev->state = TERMINATED;
    prev->retval = pVoid;
    prev->joining = 0;
    queueAddBackItem(thread_list_zombie, prev);


    /* unblock alarm signal and setcontext for next thread */
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    setcontext(current_thread_running->thread_context);
    return;
}

int CEThread_join(CEThread_t thread, void **return_value) {
    if (thread == current_thread_running->tid) {
        printf("Unable to join to same thread");
        return -1;
    }
    CEThread_treadInfo *threadTreadInfo;
    if ((threadTreadInfo = get_thread(thread, thread_list_zombie)) != NULL) {

        free_thread(threadTreadInfo);
        //TODO: Delete thread
        return 0;
    }
    if ((threadTreadInfo = get_thread(thread, thread_list)) == NULL) {
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

CEThread_treadInfo *get_thread(CEThread_t thread, queue_t *thread_list_local) {
    CEThread_treadInfo *result = NULL;
    queue_node_t *item = thread_list_local->front;
    while (item != NULL) {
        if (((CEThread_treadInfo *) item->item)->tid == thread) {
            result = (CEThread_treadInfo *) item->item;
            break;
        }
        item = item->next;
    }
    return result;
}

int CEThread_yield() {
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    if (queueIsempty(thread_list)) {
        return -1;
    }
    default_algo(SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    return 0;
};

int CEThread_detach(CEThread_t thread) {
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    CEThread_treadInfo *ceThreadTreadInfo = get_thread(thread, thread_list);
    ceThreadTreadInfo->detach = 1;
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
    return 0;
};

void CEThread_mutex_init(CEThread_mutex_t *mutex, CEThread_mutex_attr_t *attr) {
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    mutex->blocked_list = malloc(sizeof(queue_t));
    queueInit(mutex->blocked_list);
    //TODO:Cahbge list
    sigprocmask(SIG_UNBLOCK, &alarm_timeout_thread, NULL);
};

void CEThread_mutex_lock(CEThread_mutex_t *mutex) {
    sigprocmask(SIG_BLOCK, &alarm_timeout_thread, NULL);
    if (mutex->owner_thread != current_thread_running->tid && mutex->owner_thread != 0) {
        queueAddBackItem(mutex->blocked_list, current_thread_running);
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

void unblock_threads_from_list(queue_t *list) {
    while (list->front != NULL) {
        CEThread_treadInfo *r = (CEThread_treadInfo *) queueRemoveFrontItem(list);
        r = get_thread(r->tid, thread_list);
        r->state = READY;
    }
};