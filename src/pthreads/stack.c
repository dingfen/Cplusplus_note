#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h>
#define NUMTHREADS 4 
#define N 1000 
#define MEGEXTRA 1000000 
	
pthread_attr_t attr; 
	
void *dowork(void *threadid) 
{
	//The 2-dim array takes up sizeof(double)*N*N bytes.
	double A[N][N]; 
	int i,j,tid; 
	size_t mystacksize; 
 
	tid = *(int*)threadid; 
	pthread_attr_getstacksize (&attr, &mystacksize); 
	printf("Thread %d: stack size = %li bytes \n", tid, mystacksize); 
	for (i=0; i<N; i++) 
		for (j=0; j<N; j++) 
			A[i][j] = ((i*j)/3.452) + (N-i); 
	
	free(threadid);
	threadid = NULL;
	pthread_exit(NULL); 
} 
	
int main(int argc, char *argv[]) 
{ 
	pthread_t threads[NUMTHREADS]; 
	size_t stacksize; 
	int rc, t; 
	
	pthread_attr_init(&attr);
	
	pthread_attr_getstacksize(&attr, &stacksize); 
	
	printf("Default stack size = %li\n", stacksize); 
	
	stacksize = sizeof(double)*N*N+MEGEXTRA; 
	printf("Amount of stack needed per thread = %li\n",stacksize); 
	
	pthread_attr_setstacksize(&attr, stacksize); 
	
	printf("Creating threads with stack size = %li bytes\n",stacksize); 
	
	for(t=0; t<NUMTHREADS; t++){ 
		int* id = (int*)malloc(sizeof(int));
		*id = t;
		rc = pthread_create(&threads[t], &attr, dowork, (void *)id); 
		if (rc){ 
			printf("ERROR; return code from pthread_create() is %d\n", rc); 
			exit(-1); 
		} 
	} 
	printf("Created %d threads.\n", t); 
	pthread_exit(NULL); 
} 