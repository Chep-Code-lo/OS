// producer_consumer_min.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUF_SIZE 5

static int buf[BUF_SIZE];
static int in = 0, out = 0;
static int next_num = 0;

static pthread_mutex_t mtx;
static sem_t empty_slots; 
static sem_t full_slots;  

static void put(int x) {
    buf[in] = x;
    in = (in + 1) % BUF_SIZE;
}

static int get(void) {
    int x = buf[out];
    out = (out + 1) % BUF_SIZE;
    return x;
}

static void* producer(void* arg) {
    (void)arg;
    while (1) {
        sleep(1);
        sem_wait(&empty_slots);        
        pthread_mutex_lock(&mtx);
        int x = next_num++;
        put(x);
        printf("P %lu -> %d\n", (unsigned long)pthread_self(), x);
        pthread_mutex_unlock(&mtx);
        sem_post(&full_slots);         
    }
    return NULL;
}

static void* consumer(void* arg) {
    (void)arg;
    while (1) {
        sleep(1);
        sem_wait(&full_slots);        
        pthread_mutex_lock(&mtx);
        int x = get();
        printf("C %lu <- %d\n", (unsigned long)pthread_self(), x);
        pthread_mutex_unlock(&mtx);
        sem_post(&empty_slots);        
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <run_seconds> <P> <C> <start_number>\n", argv[0]);
        return 1;
    }
    int run_seconds = atoi(argv[1]);
    int P = atoi(argv[2]);
    int C = atoi(argv[3]);
    next_num = atoi(argv[4]);

    pthread_mutex_init(&mtx, NULL);
    sem_init(&empty_slots, 0, BUF_SIZE);
    sem_init(&full_slots, 0, 0);

    pthread_t pt[P], ct[C];
    for (int i = 0; i < P; i++) pthread_create(&pt[i], NULL, producer, NULL);
    for (int i = 0; i < C; i++) pthread_create(&ct[i], NULL, consumer, NULL);
    sleep(run_seconds);
}