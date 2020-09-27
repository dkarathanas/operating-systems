/*Karathanasis Dimitris 3547
csd3547*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/timeb.h>

#define KNRM "\x1B[0m"
#define KGRN "\x1B[32m"

int p;
int c;
int bufsize;
int *buffer;
int flag = 0;										//flag used to signal when producers are over

pthread_mutex_t producerLock;
pthread_mutex_t consumerLock;

int producerCounter = 0;
int consumerCounter = 0;
int consumerPrint = 1;


sem_t empty;
sem_t used;

void *producer(void *ptr){

	int i = 0;

	for(i=0; i<1000000; i++){

		int time = rand();
		int number;
		time = (time%10)+1;

		usleep(time);
		number = rand();
		number = (number%100)+1;	

		sem_wait(&empty);
		pthread_mutex_lock(&producerLock);
		
		buffer[producerCounter++] = number;
		producerCounter = producerCounter%bufsize;
		
		pthread_mutex_unlock(&producerLock);
		sem_post(&used);
	}

	return 0;
}

void *consumer(void *ptr){

	int value =0;
	int sem_value_empty = sem_getvalue(&empty,&sem_value_empty);
	while(consumerCounter!=producerCounter || flag == 0){
		
		sem_wait(&used);
		pthread_mutex_lock(&consumerLock);

		if(consumerCounter == producerCounter){
		}
		else{
			value = value + buffer[consumerCounter++];
		}
		consumerCounter = consumerCounter%bufsize;

		pthread_mutex_unlock(&consumerLock);
		sem_post(&empty);
		sem_getvalue(&used,&sem_value_empty);
	}

	printf("Consumer[%d]\t: %d\n",consumerPrint++, value );
	return 0;
}



int main(int args, char* argv[]){

	struct timeb start, end;
	int diff;
	ftime(&start);

	p = atoi(argv[2]);
	c = atoi(argv[4]);
	int i;
	int temp;

	bufsize = (p * 100000);
	buffer = malloc(bufsize * sizeof(int));

	int producerMutex = pthread_mutex_init(&producerLock,NULL);
	int consumerMutex = pthread_mutex_init(&consumerLock,NULL);

	sem_init(&empty,0,bufsize);
	sem_init(&used,0,0);

	pthread_t producerArray[p];
	pthread_t consumerArray[c];


	for(i=0; i<p; i++){
		pthread_create(&producerArray[i],NULL,producer,NULL);
	}

	for(i=0; i<c; i++){
		pthread_create(&consumerArray[i],NULL,consumer,NULL);
	}

	for(i=0; i<p; i++){
		pthread_join(producerArray[i],NULL);
	}
	flag = 1;
	for(i=0; i<(2*c); i++){
		sem_post(&used);
	}
	for(i=0; i<c; i++){
		pthread_join(consumerArray[i],NULL);
	}
	ftime(&end);

	diff = (int) (1000.0 * (end.time - start.time)
		+ (end.millitm - start.millitm));

	printf("\n");
	printf(KGRN"Completed in %.3fs !!!\n"KNRM, (float)diff/1000);
}
