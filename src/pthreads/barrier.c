#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#define	NUM_THREADS	2

pthread_barrier_t barrier;

void *Task1(void *arg);
void *Task2(void *arg);

int main(void)
{
    int policy,inher;
    pthread_t tid;
    pthread_attr_t attr;
    struct sched_param param;

    pthread_attr_init(&attr);
    pthread_barrier_init(&barrier, NULL, NUM_THREADS + 1);

    pthread_create(&tid, &attr,Task1,NULL);
    pthread_create(&tid, &attr,Task2,NULL);
    
    printf("main process will sleep 1s.\r\n");
    sleep(1);
    pthread_barrier_wait(&barrier);

    pthread_join(tid, NULL);
    pthread_barrier_destroy(&barrier);
}

void *Task1(void *arg)
{
    printf("Task1 will be blocked.\r\n");
    pthread_barrier_wait(&barrier);
    printf("Task1 is running.\r\n");
    pthread_exit(NULL);
}

void *Task2(void *arg)
{
    printf("Task2 will be blocked.\r\n");
    pthread_barrier_wait(&barrier);
    printf("Task2 is running.\r\n");
    pthread_exit(NULL);
}