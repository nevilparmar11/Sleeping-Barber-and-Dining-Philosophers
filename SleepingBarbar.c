#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


const int NO_CUSTOMERS = 100;
const int NO_BARBER = 1;
const int CAPACITY = 5;


sem_t sem_wait_count, sem_cust, sem_barb;
int waiting = 0;

void *Customer(void *arg)
{
	int * custId=(int *)arg;
	sem_wait(&sem_wait_count);
	if(waiting < CAPACITY)
	{
		waiting++;
		printf("Customer-%d Waiting \n", *custId);
		sem_post(&sem_cust);
		sem_post(&sem_wait_count);
		sem_wait(&sem_barb);
		printf("Customer-%d Got Haircut\n", *custId);
	}
	else
	{
		sem_post(&sem_wait_count);
	}
}


void *Barber(void *arg)
{
	while(1)
	{
		sem_wait(&sem_cust);
		sem_wait(&sem_wait_count);
		waiting--;
		printf("Barber :  Started cutting hair \t Total number of customers in waiting :- %d \n", waiting);
		sem_post(&sem_barb);
		sem_post(&sem_wait_count);
	}
}

void main()
{
	int i, customerIds[NO_CUSTOMERS];
	pthread_t barberThreads[NO_BARBER], customerThreads[NO_CUSTOMERS];
	sem_init(&sem_barb, 0, 0);
	sem_init(&sem_cust, 0, 0);
	sem_init(&sem_wait_count, 0, 1);

	for(i = 0 ; i < NO_BARBER ; i++)
	{
		pthread_create(&barberThreads[i], NULL,Barber, (void*)NULL);
	}
	for(i = 0 ; i < NO_CUSTOMERS ; i++)
	{
		customerIds[i] = i + 1;
		pthread_create(&customerThreads[i], NULL,Customer, (void*)&customerIds[i]);
	}
	for(i = 0 ; i < NO_BARBER ; i++)
	{
		pthread_join(barberThreads[i], NULL);
	}
	for(i = 0 ; i < NO_CUSTOMERS ; i++)
	{
		pthread_join(customerThreads[i], NULL);
	}
}
