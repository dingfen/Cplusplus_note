#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	4

void *PrintHello(void *threadid) 
{ 
	int tid; 
	tid = *((int*)threadid); 
	printf("Hello World! It's me, thread #%d!\n", tid); 
	free(threadid);
	threadid = NULL;
	pthread_exit(NULL); 
} 
 
int main (int argc, char *argv[]) 
{ 
	pthread_t threads[NUM_THREADS]; 
	int rc=-1, t; 
	for(t=0; t<NUM_THREADS; t++){ 
		printf("In main: creating thread %d\n", t); 
		int* id = (int*)malloc(sizeof(int));
		*id = t;
		rc = pthread_create(&threads[t], NULL, PrintHello, (void *)id); 
		if (rc){ 
			printf("ERROR; return code from pthread_create() is %d", rc); 
			exit(-1); 
		} 
	} 
	pthread_exit(NULL); 
} 