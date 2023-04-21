#include "lib.h"
#include "types.h"
#include "pthread.h"

int gi = 0;

void * ping_thread_function(void *arg){
    int i;
    for (i = 0; i < 10; i++){
        gi++;
        printf("Ping@%d-%d\n", gi, *(int *)arg);
        sleep(10);
        pthread_yield();
    }
    pthread_exit(NULL);
    return NULL;
}

void * pong_thread_function(void *arg){
    int i;
    for (i = 0; i < 5; i++){
        gi++;
        printf("Pong@%d\n", gi);
        sleep(10);
        pthread_yield();
    }
    pthread_exit(NULL);
    return NULL;
}

int main(void) {
    int a = 1, b = 2;
    pthread_t pi1_thread_ID, pi2_thread_ID, po_thread_ID;
    pthread_create(&pi1_thread_ID, NULL, ping_thread_function, &a);
    pthread_create(&pi2_thread_ID, NULL, ping_thread_function, &b);
    pthread_create(&po_thread_ID, NULL, pong_thread_function, NULL);
    pthread_join(pi1_thread_ID, NULL);
    pthread_join(pi2_thread_ID, NULL);
    pthread_join(po_thread_ID, NULL);
    exit();
    return 0;
}