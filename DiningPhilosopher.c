#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define totalPhilosophers 5
sem_t sem_fork[totalPhilosophers], sem_room;

void *Philosopher(void *arg)
{
	int *ph_id = (int *)arg;
	while(1)
	{
		sem_wait(&sem_room);
		printf("\nPhilosopher : %d thinking\n", *ph_id);
		sem_wait(&sem_fork[*ph_id - 1]);
		printf("Philosopher : %d  grabed left fork\n", *ph_id);
		sem_wait(&sem_fork[(*ph_id)%totalPhilosophers]);
		printf("Philosopher : %d : grabed both fork and ready to eat\n", *ph_id);
		sem_post(&sem_fork[(*ph_id)%totalPhilosophers]);
		printf("Philosopher : %d : relesed right fork\n", *ph_id);
		sem_post(&sem_fork[*ph_id - 1]);
		printf("Philosopher : %d : relesed both forks\n", *ph_id);
		sem_post(&sem_room);
		sleep(1);
	}
}

void main()
{
	pthread_t phi_thread[totalPhilosophers];
	int i, ph_id[totalPhilosophers];
	sem_init(&sem_room,0, totalPhilosophers-1);

	for(i = 0 ; i < totalPhilosophers ; i++)
	{
		sem_init(&sem_fork[i], 0, 1);
	}
	for(i = 0 ; i < totalPhilosophers ; i++)
	{
		ph_id[i] = i + 1;
		pthread_create(&phi_thread[i], NULL, Philosopher, (void*)&ph_id[i]);
	}
	for(i = 0 ; i < totalPhilosophers ; i++)
	{
		pthread_join(phi_thread[i], NULL);
	}
}
