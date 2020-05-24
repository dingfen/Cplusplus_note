#include <stdio.h>  
#include <pthread.h>  
#include <unistd.h> 
#include <stdlib.h>

pthread_once_t once = PTHREAD_ONCE_INIT;  

void once_run(void)  
{  
	printf("once_run in thread %u\n", (unsigned int)pthread_self());
}  

void * child1(void * arg)  
{  
    pthread_t tid = pthread_self();  
    printf("thread %u enters\n", (unsigned int)tid);
	//your code here
    pthread_once(&once,once_run);  
	//end of your code
    printf("thread %u returns\n", (unsigned int)tid);
}  


void * child2(void * arg)  
{  
    pthread_t tid = pthread_self();  
    printf("thread %u enters\n", (unsigned int)tid);
	//your code here
    pthread_once(&once,once_run);  
	//end of your code
    printf("thread %u returns\n", (unsigned int)tid);
}  

int main()  
{  
    pthread_t tid1,tid2;  
    pthread_create(&tid1,NULL,child1,NULL);  
    pthread_create(&tid2,NULL,child2,NULL);  
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    return 0;  
}  