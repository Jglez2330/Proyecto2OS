//
// Created by jglez2330 on 17/5/21.
//

#include <slcurses.h>
#include "CEThread.h"
#define QUANTUM 7500
#define Channels 4
long globalTID = 0;
int current_channel;
CEThread_treadInfo* main_thread;
listNode_t_thread* thread_list;
listNode_t_thread* zombie_list;

CEThread_treadInfo* current_running_thread;
sigset_t context_switching_alarm;
static struct itimerval timer_context;

CEThread_treadInfo *get_next_thread();


bool is_thread_inside(listNode_t_thread *pNode, CEThread_treadInfo *pThread);

void CEThread_get_main_thread_context() {
    struct sigaction act;


    main_thread = malloc(sizeof(CEThread_treadInfo));
    memset(main_thread, 0, sizeof(CEThread_treadInfo));

    main_thread->tid = globalTID++;
    main_thread->state = RUNNING_thread;
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
    thread_list = NULL;
    zombie_list = NULL;
    append_thread(&thread_list, main_thread);
}

int CEThread_create(CEThread_t* thread_id,CEThread_attr_t* attr ,void *(*start_routine)(void*), void *arg){

    if (globalTID++ == 0){
        CEThread_get_main_thread_context();
    }
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);

    CEThread_treadInfo* new_thread = (CEThread_treadInfo* ) malloc(sizeof(CEThread_treadInfo));
    new_thread->tid = globalTID;
    *thread_id = new_thread->tid;
    new_thread->state = READY_thread;
    new_thread->pFunction = start_routine;
    new_thread->arg = arg;
    new_thread->joining = 0;
    new_thread->detach = 0;

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

    append_thread(&thread_list, new_thread);


    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
}

void CEThread_start(void* (*start_routine)(void*), void* args){
    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);

    void * return_value = (*start_routine)(args);

    CEThread_end(return_value);
}

int CEThread_yield(){
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);
    context_switching(SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
    return 0;
}

CEThread_treadInfo *get_next_thread() {
    CEThread_treadInfo* result;

    listCycle_t_thread(&thread_list);
    result = getFront_t_thread(thread_list);

    if (result == NULL){
        return main_thread;
    } else{
        return result;
    }

}
void context_switching(int sig){
    /* block the signal */
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);

    CEThread_treadInfo *next;
    do {
        next = get_next_thread();
    } while (next->state != READY_thread);

    CEThread_treadInfo * prev = current_running_thread;
    if (prev->state == RUNNING_thread){
        prev->state = READY_thread;
    }


    next->state = RUNNING_thread;

    current_running_thread = next;

    /* unblock the signal */
    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
    swapcontext(prev->thread_context, current_running_thread->thread_context);
}

void CEThread_end(void* return_value){
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);

    CEThread_treadInfo * prev = getFront_t_thread(thread_list);
    deleteNodePosition_thread(&thread_list, 0);

    current_running_thread = get_next_thread();
    current_running_thread->state = RUNNING_thread;

    if(prev->detach == 1 ){
        free(prev->thread_context->uc_stack.ss_sp);
        free(prev->thread_context);
        prev->thread_context = NULL;

        free(prev);
        sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
        setcontext(current_running_thread->thread_context);
        return;
    }

    if (current_running_thread->tid == prev->tid){
        sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
        setcontext(current_running_thread->thread_context);
        return;
    }else {

        free(prev->thread_context->uc_stack.ss_sp);
        free(prev->thread_context);
        prev->thread_context = NULL;

        prev->state = TERMINATED_thread;
        prev->retval = return_value;
        prev->joining = 0;

        append_thread(&zombie_list, prev);


        sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
        setcontext(current_running_thread->thread_context);
    }
}

int CEThread_detach(CEThread_t thread){
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);
    CEThread_treadInfo* thread_to_detach;

    thread_to_detach = get_thread_by_tid_zombie(thread);
    if (thread_to_detach != NULL) {
        deleteNodeTID_t_thread(&zombie_list, thread);
        free(thread_to_detach->thread_context->uc_stack.ss_sp);
        free(thread_to_detach->thread_context);
        thread_to_detach->thread_context = NULL;

        free(thread_to_detach);
        sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
        return 0;
    }
    thread_to_detach = get_thread_by_tid(thread);

    if (thread_to_detach == NULL){

        perror("Thread to detach doesn't exists\n");
        sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
        return -1;
    }
    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
    thread_to_detach->detach = 1;
    return 0;
}
int CEThread_join(CEThread_t thread, void ** return_value){
    CEThread_treadInfo* thread_to_join;

    thread_to_join = get_thread_by_tid(thread);

    if (thread_to_join == NULL){
        thread_to_join = get_thread_by_tid_zombie(thread);
        if (thread_to_join != NULL) {
            return 0;
        }
        perror("Thread to join doesn't exists\n");
        return -1;
    }
    if (thread_to_join->tid == current_running_thread->tid){

        perror("Thread unable to join itself\n");
        return -1;
    }
    if (thread_to_join->joining == current_running_thread->tid){
        perror("Thread already joining me\n");
        return -1;
    }
    if (thread_to_join->detach == 1){
        perror("Thread is detached, unable to join it\n");
        return -1;
    }

    thread_to_join->joining = current_running_thread->tid;

    while (thread_to_join->state != TERMINATED_thread){
        sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
        context_switching(SIGVTALRM);
        sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);
    }
    if (return_value == NULL){
        return 0;
    } else{
        *return_value = thread_to_join->retval;
        return 0;
    }
}

CEThread_treadInfo* get_thread_by_tid_zombie(CEThread_t thread) {
    CEThread_treadInfo* result = NULL;
    for (int i = 0; i < getCount_t_thread(zombie_list); i++) {
        if (getNode_t_thread(zombie_list, i)->tid == thread){
            result = getNode_t_thread(zombie_list, i);
            break;
        }
    }
    return result;
}

CEThread_treadInfo* get_thread_by_tid(CEThread_t tid){
    CEThread_treadInfo* result = NULL;
    for (int i = 0; i < getCount_t_thread(thread_list); i++) {
        if (getNode_t_thread(thread_list, i)->tid == tid){
            result = getNode_t_thread(thread_list, i);
            break;
        }
    }
    return result;
}

int CEThread_mutex_init(CEThread_mutex_t* mutex){
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);
    mutex->blocked_list = NULL;
    mutex->owner_thread = 0;
    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
    return 0;
}
int CEThread_mutex_lock(CEThread_mutex_t *mutex) {
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);
    if (mutex->owner_thread != current_running_thread->tid && mutex->owner_thread != 0) {
        if (!is_thread_inside(mutex->blocked_list, current_running_thread)) {
            append_thread(&mutex->blocked_list, current_running_thread);
            current_running_thread->state = BLOCKED_thread;
            //deleteNodePosition_thread(&thread_list, 0);
            while (current_running_thread->state == BLOCKED_thread){
                CEThread_yield();

            };
            CEThread_mutex_lock(mutex);
        }
    } else {
        mutex->owner_thread = current_running_thread->tid;
    }
    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
    return 0;
}

bool is_thread_inside(listNode_t_thread *pNode, CEThread_treadInfo *pThread) {
    bool result = FALSE;
    for (int i = 0; i < getCount_t_thread(pNode); i++) {
        if(getNode_t_thread(pNode, i)->tid == pThread->tid){
            result = TRUE;
            break;
        }

    }
    return result;
};

int CEThread_mutex_unlock(CEThread_mutex_t *mutex) {
    sigprocmask(SIG_BLOCK, &context_switching_alarm, NULL);
    if (mutex->owner_thread != current_running_thread->tid && mutex->owner_thread != 0) {
        printf("Only the lock owner can free the mutex");
    } else {
        unblock_threads_from_list(mutex->blocked_list);
        mutex->owner_thread = 0;
        mutex->blocked_list = NULL;
    }
    sigprocmask(SIG_UNBLOCK, &context_switching_alarm, NULL);
    return 0;
}
void unblock_threads_from_list(listNode_t_thread* list) {
    while (getFront_t_thread(list)!= NULL) {
        CEThread_treadInfo *r = getFront_t_thread(list);
        r->state = READY_thread;
        //append_thread(&thread_list, r);
        deleteNodePosition_thread(&list, 0);
    }
};