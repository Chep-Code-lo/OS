#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#define NUMPHIL 5
#define LEFT ((id-1)+NUMPHIL) % 5
#define RIGHT (id+1) % 5
enum{THINKING, HUNGRY, EATING} state[NUMPHIL];
int identity[5] = {0,1,2,3,4};
pthread_mutex_t lock;
pthread_cond_t cond[NUMPHIL];
pthread_t phil[NUMPHIL];

void think(int id){
	int thinkTime = ((rand()) % 3) +1;
	printf("Philosopher %d is thinking for %d seconds\n", id, thinkTime);
	sleep(thinkTime);
	printf("Philosopher %d reappears from sleep from thinking\n", id);
}

void pickup_forks(int id){
	int left = LEFT;
	int right = RIGHT;
	pthread_mutex_lock(&lock);
	state[id] = HUNGRY;
	while((state[id] == HUNGRY) && ((state[left] == EATING) || (state[right] == EATING))){
		printf("Philosopher %i is hungry and waiting to pickup forks to eat \n", id);
		pthread_cond_wait(&cond[id], &lock);
	}
	state[id] = EATING;
	printf("Philosopher %d is allowed to eat now \n", id);
	pthread_mutex_unlock(&lock);
}

void eat(int id){
	int eatingTime = ((rand()) % 3) +1;
	printf("Philosopher %d is eating for %d seconds\n", id, eatingTime);
	sleep(eatingTime);
	printf("Philosopher %d reappears from sleep from eating\n", id);
}

void return_forks(int id){
	int left = LEFT;
	int right = RIGHT;
	pthread_mutex_lock(&lock);
	state[id]= THINKING;
	printf("Philosopher %d has put down forks\n", id);
	pthread_cond_signal(&cond[left]);
	printf("Philosopher %d signaled philosopher %d to see if it can eat\n", id, left);
	pthread_cond_signal(&cond[right]);
	printf("Philosopher %d signaled philosopher %d to see if it can eat\n", id, right);
	pthread_mutex_unlock(&lock);
}

void* philosopher(void* num){
	int id = *((int *) num);
	while(1){
		think(id);
		pickup_forks(id);
		eat(id);
		return_forks(id);
	}
}

int main(){
	int i=0;
	if (pthread_mutex_init(&lock,NULL) != 0) {
		printf("\n mutex init has failed\n");
		return 1;
	}
	for (i = 0; i < NUMPHIL; i++) {
		if(pthread_cond_init(&cond[i], NULL)!=0){
			printf("\n cond init has failed\n");
			return 1;
		}
	}
	for (i = 0; i < NUMPHIL; i++) 
		pthread_create(&phil[i], NULL, philosopher, &identity[i]);
	for (i = 0; i < NUMPHIL; i++)
		pthread_join(phil[i], NULL);

	pthread_mutex_destroy(&lock);
	for (i = 0; i < NUMPHIL; i++)
		pthread_cond_destroy(&cond[i]);
	return 0;
}