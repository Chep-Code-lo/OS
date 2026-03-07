#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

#define NUM_WRITERS 4
#define NUM_READERS 4
#define NAP_TIME    5

struct ReadWriteLock {
    sem_t readLock;
    sem_t writeLock;
    int   readCount;
};

void nap() {
    sleep(rand() % NAP_TIME);
}

void initReadWriteLock(ReadWriteLock* rw) {
    sem_init(&rw->readLock,  0, 1);
    sem_init(&rw->writeLock, 0, 1);
    rw->readCount = 0;
}

void destroyReadWriteLock(ReadWriteLock* rw) {
    sem_destroy(&rw->readLock);
    sem_destroy(&rw->writeLock);
}

void readLockFunc(ReadWriteLock* rw) {
    sem_wait(&rw->readLock);
    rw->readCount++;
    if (rw->readCount == 1)
        sem_wait(&rw->writeLock);
    std::cout << "Thread " << (unsigned long)pthread_self() << " is READING.\n";
    sem_post(&rw->readLock);
}

void readUnlockFunc(ReadWriteLock* rw) {
    sem_wait(&rw->readLock);
    rw->readCount--;
    if (rw->readCount == 0)
        sem_post(&rw->writeLock);
    std::cout << "Thread " << (unsigned long)pthread_self() << " is DONE READING.\n";
    sem_post(&rw->readLock);
}

void writeLockFunc(ReadWriteLock* rw) {
    sem_wait(&rw->writeLock);
    std::cout << "Thread " << (unsigned long)pthread_self() << " is WRITING.\n";
}

void writeUnlockFunc(ReadWriteLock* rw) {
    std::cout << "Thread " << (unsigned long)pthread_self() << " is DONE WRITING.\n";
    sem_post(&rw->writeLock);
}

void* writer(void* arg) {
    auto* rw = static_cast<ReadWriteLock*>(arg);
    while (true) {
        nap();
        writeLockFunc(rw);
        nap();
        writeUnlockFunc(rw);
    }
    return nullptr;
}

void* reader(void* arg) {
    auto* rw = static_cast<ReadWriteLock*>(arg);
    while (true) {
        nap();
        readLockFunc(rw);
        nap();
        readUnlockFunc(rw);
    }
    return nullptr;
}

int main() {
    pthread_t writers[NUM_WRITERS];
    pthread_t readers[NUM_READERS];
    ReadWriteLock rw;

    srand(static_cast<unsigned>(time(nullptr)));
    initReadWriteLock(&rw);

    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_create(&writers[i], nullptr, writer, &rw);
    for (int i = 0; i < NUM_READERS; i++)
        pthread_create(&readers[i], nullptr, reader, &rw);

    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], nullptr);
    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], nullptr);

    destroyReadWriteLock(&rw);
    return 0;
}
