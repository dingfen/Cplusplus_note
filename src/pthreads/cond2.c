#include <pthread.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>
   
static pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;  
static pthread_cond_t cond=PTHREAD_COND_INITIALIZER;  
  
static void* func_1(void* arg)  
{  
    pthread_mutex_lock(&mtx);  
    printf("thread 1 waiting for signal\n");
    pthread_cond_wait(&cond, &mtx);
	printf("thread 1 receives signal\n");
    pthread_mutex_unlock(&mtx);  
      
    return NULL;  
}  
  
static void* func_2(void* arg)  
{  
      
    pthread_mutex_lock(&mtx);  
    printf("thread 2 waiting for signal\n");
    pthread_cond_wait(&cond, &mtx); 
	printf("thread 2 receives signal\n");
    pthread_mutex_unlock(&mtx);  
      
    return NULL;  
}  
  
  
int main()  
{  
    pthread_t tid1, tid2;  
  
    pthread_create(&tid1, NULL, func_1, NULL);  
    pthread_create(&tid2, NULL, func_2, NULL);  
  
  	sleep(1);
  	printf("main thread broadcasts signal\n");
    pthread_cond_broadcast(&cond);  
  
    pthread_join(tid1, NULL);  
    pthread_join(tid2, NULL);  
    
    return 0;  
}  