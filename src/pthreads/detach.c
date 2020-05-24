#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void* thread1(void *arg)
{ 
	while (1)
    {
        usleep(100 * 1000);
        printf("thread1 running...!\n");
    }
    printf("Leave thread1!\n"); 
	return NULL;
} 

int main(int argc, char** argv)
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread1, NULL);
	//your code here
    pthread_detach(tid);
	//pthread_join(tid, NULL);
    //end of your code
	sleep(1);
    printf("Leave main thread!\n"); 
	return 0;
}