#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <stdlib.h>
  
int main(){  
    pthread_t thread_id;  
  
    thread_id = pthread_self();
    printf("Thread ID: %lu.\n",thread_id);  

	int t = 0;
	t = pthread_equal(thread_id,pthread_self());

    if (t)  
     {  
        printf("Equal!\n");  
     } else {  
        printf("Not equal!\n");  
    }  
    return 0;  
}  