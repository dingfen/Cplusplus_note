#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS   4

struct thread_data{ 
	int  thread_id; 
	const char *message; 
}thread_data_array[NUM_THREADS]; 
 
void *Print(void *threadarg) 
{  
	struct thread_data *my_data; 
	my_data = (struct thread_data *) threadarg; 
	int taskid;
	const char* msg;
	taskid = my_data->thread_id; 
	msg = my_data->message;
	printf("msg: %s on thread %d\n", msg, taskid);
	pthread_exit(NULL);
} 
 
int main (int argc, char *argv[]) 
{
	pthread_t threads[NUM_THREADS]; 
	int t, rc;
	const char* messages[NUM_THREADS] = {"apple", "banana", "cat", "dog"};
	for(t = 0; t < NUM_THREADS; t++) {
		thread_data_array[t].thread_id = t; 
		thread_data_array[t].message = messages[t]; 
        rc = pthread_create(&threads[t], NULL, Print, (void *)&thread_data_array[t]);
		if (rc) { 
			printf("ERROR; return code from pthread_create() is %d", rc); 
			exit(-1); 
		}
	}
	pthread_exit(NULL); 
}
