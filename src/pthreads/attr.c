#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h>
#define NUM_THREADS	 4 
 
void *Print(void *threadid) 
{ 
	printf("Thread %d is running\n", *(int*)threadid); 
	free(threadid);
	threadid = NULL;
	pthread_exit(NULL); 
} 
 
int main (int argc, char *argv[]) 
{ 
	pthread_t thread[NUM_THREADS]; 
	pthread_attr_t attr; 
	int rc, t; 
 
	pthread_attr_init(&attr); 
	
	//your code here
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); 
	//end of your code
	
	for(t=0; t<NUM_THREADS; t++) 
	{ 
		int* id = (int*)malloc(sizeof(int));
		*id = t;
		printf("Creating thread %d\n", t); 
		rc = pthread_create(&thread[t], &attr, Print, (void*)id);	
		if (rc) 
		{ 
			printf("ERROR; return code from pthread_create() is %d\n", rc); 
			exit(-1); 
		} 
	} 
	
	pthread_attr_destroy(&attr); 
	for(t=0; t<NUM_THREADS; t++) 
	{ 
		//your code here
		rc = pthread_join(thread[t], NULL); 
		//end of your code
		
		if (rc) 
		{ 
			printf("ERROR; return code from pthread_join() is %d\n", rc); 
			exit(-1); 
		} 
		printf("Completed join with thread %d\n", t); 
	} 
 
	pthread_exit(NULL); 
} 