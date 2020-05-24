#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h> 
#include <signal.h>

void *func1(void* args)
{
    sleep(1);
    printf("ID 0x%x thread exits.\n",(unsigned int)pthread_self());
    pthread_exit((void *)0);
}

void *func2(void* args)
{
    sleep(3);
    printf("ID 0x%x thread exits。\n",(unsigned int)pthread_self());
    pthread_exit((void *)0);
}

void test_pthread(pthread_t tid) 
{
    int pthread_kill_err;
	
    pthread_kill_err = pthread_kill(tid, 0);

    if(pthread_kill_err == ESRCH)
        printf("ID 0x%x thread exits.\n",(unsigned int)tid);
    else if(pthread_kill_err == EINVAL)
        printf("invalid signal\n");
    else
        printf("ID 0x%x thread stays alive now.\n",(unsigned int)tid);
}

int main()
{
    int ret;
    pthread_t tid1,tid2;
    
    pthread_create(&tid1,NULL,func1,NULL);
    pthread_create(&tid2,NULL,func2,NULL);
    
    sleep(2);
    
    test_pthread(tid1);
    test_pthread(tid2);

    exit(0);
}