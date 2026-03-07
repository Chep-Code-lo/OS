#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_WRITERS 4
#define NUM_READERS 4
#define NAP_TIME 5

typedef struct {
    sem_t readLock;
    sem_t writeLock;
    int readCount;
} ReadWriteLock;

void nap() {
    int sleeptime = rand() % NAP_TIME;
    sleep(sleeptime);
}

void initReadWriteLock(ReadWriteLock *rw) {
    sem_init(&rw->readLock, 0, 1);
    sem_init(&rw->writeLock, 0, 1);
    rw->readCount = 0;
}

void destroyReadWriteLock(ReadWriteLock *rw) {
    sem_destroy(&rw->readLock);
    sem_destroy(&rw->writeLock);
}

void readLockFunc(ReadWriteLock *rw) {
    sem_wait(&rw->readLock);
    rw->readCount++;
    if (rw->readCount == 1) {
        sem_wait(&rw->writeLock);
    }
    printf("Thread %lu is READING.\n", (unsigned long)pthread_self());
    sem_post(&rw->readLock);
}

void readUnlockFunc(ReadWriteLock *rw) {
    sem_wait(&rw->readLock);

    rw->readCount--;
    if (rw->readCount == 0) {
        sem_post(&rw->writeLock);
    }
    printf("Thread %lu is DONE READING.\n", (unsigned long)pthread_self());
    sem_post(&rw->readLock);
}

void writeLockFunc(ReadWriteLock *rw) {
    sem_wait(&rw->writeLock);
    printf("Thread %lu is WRITING.\n", (unsigned long)pthread_self());
}

void writeUnlockFunc(ReadWriteLock *rw) {
    printf("Thread %lu is DONE WRITING.\n", (unsigned long)pthread_self());
    sem_post(&rw->writeLock);
}

void *writer(void *arg) {
    ReadWriteLock *rw = (ReadWriteLock *)arg;
    while (1) {
        nap();
        writeLockFunc(rw);
        nap();
        writeUnlockFunc(rw);
    }
    return NULL;
}

void *reader(void *arg) {
    ReadWriteLock *rw = (ReadWriteLock *)arg;
    while (1) {
        nap();
        readLockFunc(rw);
        nap();
        readUnlockFunc(rw);
    }
    return NULL;
}

int main() {
    pthread_t writers[NUM_WRITERS];
    pthread_t readers[NUM_READERS];
    ReadWriteLock rw;

    srand(time(NULL));
    initReadWriteLock(&rw);

    for(int i = 0; i < NUM_WRITERS; i++) 
        pthread_create(&writers[i], NULL, writer, &rw);
    
    for(int i = 0; i < NUM_READERS; i++) 
        pthread_create(&readers[i], NULL, reader, &rw);

    for(int i = 0; i < NUM_WRITERS; i++) 
        pthread_join(writers[i], NULL);

    for(int i = 0; i < NUM_READERS; i++) 
        pthread_join(readers[i], NULL);

    destroyReadWriteLock(&rw);
}