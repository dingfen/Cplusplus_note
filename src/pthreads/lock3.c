#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define        READER_MAX    3        /*最多允许多少人一起读取数据*/
#define        WRITER_MAX    2        /*最多允许多少人写数据*/

pthread_rwlock_t    rw_lock;        /*读写锁*/

void* reader_thread(void *arg)
{
    while(1) {
		int t;
		t = pthread_rwlock_tryrdlock(&rw_lock);
        if(t) {
            printf("reader %u can't read data.\n", (unsigned int)pthread_self());
            sleep(1);
        } else {
            printf("reader %u is reading data.\n", (unsigned int)pthread_self());
            sleep(1);
            printf("reader %u reads data over.\n", (unsigned int)pthread_self());
            pthread_rwlock_unlock(&rw_lock); 
            sleep(2);   
        }    
    }
}

void* writer_thread(void *arg)
{
    while(1) {
		int t;
		t = pthread_rwlock_trywrlock(&rw_lock);
        if(t) {
            printf("writer %u can't write data.\n", (unsigned int)pthread_self());
            sleep(2);
        } else {
            printf("writer %u is writing data.\n", (unsigned int)pthread_self());
            sleep(2); 
            printf("writer %u writes data over.\n", (unsigned int)pthread_self());
            pthread_rwlock_unlock(&rw_lock);
            sleep(3); 
        }
    }
}

int main(int argc, char* argv[])
{
    pthread_t reader, writer;
    int i = 0; 
    pthread_rwlock_init(&rw_lock, NULL);
    for(i = 0; i < READER_MAX; i++)    
        pthread_create(&reader, NULL, reader_thread, NULL);
    for(i = 0; i < WRITER_MAX; i++)
        pthread_create(&writer, NULL, writer_thread, NULL);
    sleep(3);
    
    return 0;
}
