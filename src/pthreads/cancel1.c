#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *thr_fn1(void *arg)
{
    printf("thread 1 returning\n");
    return (void*)1;
}

void *thr_fn2(void *arg)
{
    printf("thread 2 exiting\n");
    pthread_exit((void*)2);
}

void *thr_fn3(void *arg)
{
	int time = 0;
    while(1) {
    	time++;
        printf("thread 3 writing\n");
        sleep(1);
        if(time>=4) return NULL;
    }
}

int main()
{
    pthread_t tid;
    void *retval;

    pthread_create(&tid, NULL, thr_fn1, NULL);
    pthread_join(tid, &retval);
    printf("thread 1 exit code %d\n", (int*)retval);    
    
    pthread_create(&tid, NULL, thr_fn2, NULL);
    pthread_join(tid, &retval);
    printf("thread 2 exit code %d\n", (int*)retval);

    pthread_create(&tid, NULL, thr_fn3, NULL);
    sleep(2);
    //your code here
    pthread_cancel(tid);
    //end of your code
    pthread_join(tid, &retval);
    printf("thread 3 exit code %d\n", (int*)retval);
    
    return 0;
}