#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10 // Total threads to be spawned
#define WRITE_FLAG 0 // For Readabliity
#define READ_FLAG 1 // For Readabliity
#define SHARED_ARR_SIZE 5 // Size of shared array

int sharedArr[SHARED_ARR_SIZE];
pthread_mutex_t mutex; // For locking

void *read(void *argn) {
	printf("In read\n");
	int x, i;
	for(i = 0; i<SHARED_ARR_SIZE; i++)
		x = sharedArr[i]; // Reading shared array
	pthread_exit(NULL); // Terminate thread
}

void *write(void *arg) {
	pthread_mutex_lock(&mutex); // Acquire mutex
	printf("in write\n");
	int i;
	for(i = 0; i<SHARED_ARR_SIZE; i++)
		sharedArr[i] = rand() % 100; //Writing shared array
	pthread_mutex_unlock(&mutex); // Release Mutex
	pthread_exit(NULL); // Terminate pthread
}

void *readCreater(void *arg_readCount){
	int i;
	pthread_mutex_lock(&mutex); // Acquire mutex
	long readCount = (long)arg_readCount; // Number of read threads to be spawned
	pthread_t readThreads[readCount]; // Array of read threads
	for(i = 0; i < readCount; i++)
		pthread_create(&readThreads[i], NULL, read, (void*)0); // Spawn read threads
	for(i = 0; i < readCount; i++)
		pthread_join(readThreads[i], NULL); // Joined spawned read threads
	pthread_mutex_unlock(&mutex); // Release Mutex
	pthread_exit(NULL); // Terminate pthread
}

void main () {
	pthread_t thread[NUM_THREADS]; // array of readCreater and write threads
	long action, i, readCount = 0;
	/*
		action: Denotes read or write. It is either 0(Write) or 1(Read)
		readCount: counts number of adjacent read threads to be spawned
	*/
	for(i = 0; i < NUM_THREADS; i++) {
		action = rand() > RAND_MAX/2; // Randomly generate read(1) or write(0) tasks
		if(action == READ_FLAG)
			readCount++; //increment number of adjacent read threads to be spawned
		else{
			if(readCount != 0){
				/*
					readCount holds the number of adjacent read threads to be spawned.
					the function readCreater is spawned in it's own thread which spawns
					"readCount" number of read threads while holding the lock.
				*/
				pthread_create(&thread[i], NULL, readCreater, (void*)readCount);
				readCount = 0; // Empties the read thread count
			}
			pthread_create(&thread[i], NULL, write, (void*)0); // Spawns a write thread.
		}
	}
	if(readCount != 0)
		pthread_create(&thread[i], NULL, readCreater, (void*)readCount); // Compensates if read task at last.
	sleep(10);
}