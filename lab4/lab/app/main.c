#include "lib.h"
#include "types.h"

int data = 0;

void producer(int arg, sem_t* mutex, sem_t* buffer) {
    int pid = getpid();
    for (int k = 1; k <= 8; ++k) {
        sleep(64);
        printf("pid %d, producer %d, produce, product %d\n", pid, arg, k);
        printf("pid %d, producer %d, try lock, product %d\n", pid, arg, k);
        sem_wait(mutex);
        printf("pid %d, producer %d, locked\n", pid, arg);
        sem_post(mutex);
        printf("pid %d, producer %d, unlock\n", pid, arg);
        sem_post(buffer);
    }
}

void consumer(int arg, sem_t* mutex, sem_t* buffer) {
    int pid = getpid();
    for (int k = 1; k <= 4; ++k) {
        printf("pid %d, consumer %d, try consume, product %d\n", pid, arg, k);
        sem_wait(buffer);
        printf("pid %d, consumer %d, try lock, product %d\n", pid, arg, k);
        sem_wait(mutex);
        printf("pid %d, consumer %d, locked\n", pid, arg);
        sem_post(mutex);
        printf("pid %d, consumer %d, unlock\n", pid, arg);
        sleep(64);
        printf("pid %d, consumer %d, consumed, product %d\n", pid, arg, k);
    }
}

int uEntry(void) {
    
    int i = 4;
    int ret = 0;
    sem_t sem;
    sem_t mutex;
    sem_t buffer;
    printf("Father Process: Semaphore Initializing.\n");
    ret = sem_init(&sem, 2);
    if (ret == -1) {
        printf("Father Process: Semaphore Initializing Failed.\n");
        exit();
    }

    ret = fork();
    if (ret == 0) {
        while( i != 0) {
            i --;
            printf("Child Process: Semaphore Waiting.\n");
            sem_wait(&sem);
            printf("Child Process: In Critical Area.\n");
        }
        printf("Child Process: Semaphore Destroying.\n");
        sem_destroy(&sem);
        exit();
    }
    else if (ret != -1) {
        while( i != 0) {
            i --;
            printf("Father Process: Sleeping.\n");
            sleep(128);
            printf("Father Process: Semaphore Posting.\n");
            sem_post(&sem);
        }
        printf("Father Process: Semaphore Destroying.\n");
        sem_destroy(&sem);
        //exit();
    }
    
    sem_init(&mutex, 1);
    sem_init(&buffer, 0);
    for (int i = 0; i < 6; ++i) {
        if (fork() == 0) {
            if (i < 2)
                producer(i + 1, &mutex, &buffer);
            else
                consumer(i - 1, &mutex, &buffer);
            break;
        }    
    }
    
    while (1);
    sem_destroy(&buffer);
    sem_destroy(&mutex);
    return 0;
}
