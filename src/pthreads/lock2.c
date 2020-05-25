#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t mutex;

void *thread_a(void *arg)
{
    pthread_mutex_lock(&mutex);
    sleep(1);
    pthread_mutex_unlock(&mutex);
}

void *thread_b(void *arg)
{
    while(pthread_mutex_trylock(&mutex))
    {
        printf("pthread b trylock\n");
        sleep(1);
    }
    pthread_mutex_unlock(&mutex);
}

int main(int argc,char **argv)
{
    pthread_t tid_a,tid_b;
    pthread_mutex_init(&mutex,NULL);
	pthread_create(&tid_a,NULL,thread_a,NULL);
    pthread_create(&tid_b,NULL,thread_b,NULL);
    pthread_join(tid_a, NULL);
    pthread_join(tid_b, NULL);

    printf("the main close\n");

    return 0;
}