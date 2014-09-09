#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM_THREADS 10
#define WRITE_FLAG 0
#define READ_FLAG 1
#define SHARED_ARR_SIZE 5

int sharedArr[SHARED_ARR_SIZE];
pthread_mutex_t mutex;

void *read(void *argn) {
	printf("In read\n");
	sleep(1);
	pthread_exit(NULL);
}

void *write(void *arg) {
	pthread_mutex_lock(&mutex);
	printf("in write\n");
	sleep(1);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

void *readcreater(void *arg_readCount){
	int i;
	pthread_mutex_lock(&mutex);
	long readCount = (long)arg_readCount;
	pthread_t readThreads[readCount];
	for(i = 0; i < readCount; i++)
		pthread_create(&readThreads[i], NULL, read, (void*)0);
	for(i = 0; i < readCount; i++)
		pthread_join(readThreads[i], NULL);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

void main () {
	pthread_t thread[NUM_THREADS];
	long action, i, readCount = 0;

	for(i = 0; i < NUM_THREADS; i++) {
		action = rand() > RAND_MAX/2;
		if(action == READ_FLAG)
			readCount++;
		else{
			if(readCount != 0){
				pthread_create(&thread[i], NULL, readcreater, (void*)readCount);
				readCount = 0;
			}
			pthread_create(&thread[i], NULL, write, (void*)0);
		}
	}
	if(readCount != 0)
		pthread_create(&thread[i], NULL, readcreater, (void*)readCount);
	sleep(10);
}