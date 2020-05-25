#include <stdio.h>  
#include <pthread.h>  
#include <unistd.h>  
#include <stdlib.h>
  
int i;  
pthread_mutex_t mutex1;  
  
void* readfunction(void*);  
void* writefunction(void*);  
  
int main()  
{  
	pthread_t thread1,thread2;  
  
	i = 0;  
	pthread_mutex_init(&mutex1, NULL);
	pthread_create(&thread1,NULL,readfunction,NULL);
	pthread_create(&thread2,NULL,writefunction,NULL);  
	pthread_join(thread1,NULL);  
	pthread_join(thread2,NULL);
	pthread_mutex_destroy(&mutex1);
	return 0;  
}  
  
void* readfunction(void* args)  
{  
	while(1)  
	{  
		usleep(1500);
		if(i >= 10)  
			break;  
		pthread_mutex_lock(&mutex1);  
		printf("%d\n",i);  
		pthread_mutex_unlock(&mutex1);  
	}  
}  
  
void* writefunction(void* args)  
{  
	while(i < 10)  
	{  
		pthread_mutex_lock(&mutex1);  
		i++;  
		pthread_mutex_unlock(&mutex1);  
		usleep(2000);  
	}  
}
