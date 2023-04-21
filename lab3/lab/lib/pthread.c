#include "pthread.h"
#include "lib.h"
/*
 * pthread lib here
 * 用户态多线程写在这
 */

ThreadTable tcb[MAX_TCB_NUM];
int current;

void pthread_initial(void)
{
    int i;
    for (i = 0; i < MAX_TCB_NUM; i++)
    {
        tcb[i].state = STATE_DEAD;
        tcb[i].joinid = -1;
    }
    tcb[0].state = STATE_RUNNING;
    tcb[0].pthid = 0;
    current = 0; // main thread
    return;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
    int i;
    for (i = 0; i < MAX_TCB_NUM; ++i)
        if (tcb[i].state == STATE_DEAD)
            break;
    if (i == MAX_TCB_NUM)
        return -1;
    *thread = i;
    tcb[i].stack[MAX_STACK_SIZE - 1] = (uint32_t)arg;
    tcb[i].cont.esp = (uint32_t)&tcb[i].stack[MAX_STACK_SIZE - 2];
    tcb[i].cont.eip = (uint32_t)start_routine;
    tcb[i].pthArg = (uint32_t)arg;
    tcb[i].state = STATE_RUNNABLE;
    tcb[i].pthid = i;
    return 0;
}

int pthread_join(pthread_t thread, void **retval)
{
    while (tcb[thread].state != STATE_DEAD)
    {
        asm volatile("movl 4(%%ebp), %0":"=r"(tcb[current].cont.eip));
        asm volatile("pusha");
        asm volatile("popl %0":"=m"(tcb[current].cont.edi));
        asm volatile("popl %0":"=m"(tcb[current].cont.esi));
        asm volatile("popl %0":"=m"(tcb[current].cont.ebp));
        asm volatile("popl %0":"=m"(tcb[current].cont.esp));
        asm volatile("popl %0":"=m"(tcb[current].cont.ebx));
        asm volatile("popl %0":"=m"(tcb[current].cont.edx));
        asm volatile("popl %0":"=m"(tcb[current].cont.ecx));
        asm volatile("popl %0":"=m"(tcb[current].cont.eax));
        asm volatile("movl (%%ebp), %0":"=r"(tcb[current].cont.ebp));
        asm volatile("leal 8(%%ebp), %0":"=r"(tcb[current].cont.esp));
        tcb[current].state = STATE_RUNNABLE;
        current = thread;
        tcb[current].state = STATE_RUNNING;
        printf("");
        asm volatile("movl %0, %%esp"::"r"(&tcb[current].cont.edi));
        asm volatile("popa");
        asm volatile("movl %0, %%esp"::"m"(tcb[current].cont.esp));
        asm volatile("jmp *%0"::"m"(tcb[current].cont.eip));
    }
    return 0;
}
void pthread_exit(void *retval)
{
    tcb[current].state = STATE_DEAD;
    int i;
    for (i = (current+1)%MAX_TCB_NUM; i != current; i = (i+1)%MAX_TCB_NUM)
        if (tcb[i].state == STATE_RUNNABLE)
            break;
    current = i;
    tcb[i].state = STATE_RUNNING;
    //printf("");
    asm volatile("movl %0, %%esp"::"r"(&tcb[current].cont.edi));
    asm volatile("popa");
    asm volatile("movl %0, %%esp"::"m"(tcb[current].cont.esp));
    asm volatile("jmp *%0"::"m"(tcb[current].cont.eip));
    //printf("");
    return;
}

int pthread_yield(void)
{
    asm volatile("movl 4(%%ebp), %0":"=r"(tcb[current].cont.eip));
    asm volatile("pusha");
    asm volatile("popl %0":"=m"(tcb[current].cont.edi));
    asm volatile("popl %0":"=m"(tcb[current].cont.esi));
    asm volatile("popl %0":"=m"(tcb[current].cont.ebp));
    asm volatile("popl %0":"=m"(tcb[current].cont.esp));
    asm volatile("popl %0":"=m"(tcb[current].cont.ebx));
    asm volatile("popl %0":"=m"(tcb[current].cont.edx));
    asm volatile("popl %0":"=m"(tcb[current].cont.ecx));
    asm volatile("popl %0":"=m"(tcb[current].cont.eax));
    asm volatile("movl (%%ebp), %0":"=r"(tcb[current].cont.ebp));
    asm volatile("leal 8(%%ebp), %0":"=r"(tcb[current].cont.esp));
    printf("");
    tcb[current].state = STATE_RUNNABLE;
    int i;
    for (i = (current+1)%MAX_TCB_NUM; i != current; i = (i+1)%MAX_TCB_NUM)
        if (i != 0 && tcb[i].state == STATE_RUNNABLE)
            break;
    current = i;
    tcb[i].state = STATE_RUNNING;
    //printf("");
    asm volatile("movl %0, %%esp"::"r"(&tcb[current].cont.edi));
    asm volatile("popa");
    asm volatile("movl %0, %%esp"::"m"(tcb[current].cont.esp));
    asm volatile("jmp *%0"::"m"(tcb[current].cont.eip));
    printf("");
    return 0;
}
