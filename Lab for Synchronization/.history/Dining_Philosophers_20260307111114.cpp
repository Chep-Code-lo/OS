#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>

#define NUMPHIL 5
#define LEFT    (((id) - 1 + NUMPHIL) % NUMPHIL)
#define RIGHT   (((id) + 1) % NUMPHIL)

enum State { THINKING, HUNGRY, EATING };

State          state[NUMPHIL];
int            identity[NUMPHIL] = {0, 1, 2, 3, 4};
pthread_mutex_t lock;
pthread_cond_t  cond[NUMPHIL];
pthread_t       phil[NUMPHIL];

void think(int id) {
    int thinkTime = (rand() % 3) + 1;
    std::cout << "Philosopher " << id << " is thinking for " << thinkTime << " seconds\n";
    sleep(thinkTime);
    std::cout << "Philosopher " << id << " reappears from sleep from thinking\n";
}

void pickup_forks(int id) {
    int left  = LEFT;
    int right = RIGHT;

    pthread_mutex_lock(&lock);
    state[id] = HUNGRY;

    while (state[id] == HUNGRY && (state[left] == EATING || state[right] == EATING)) {
        std::cout << "Philosopher " << id << " is hungry and waiting to pickup forks to eat\n";
        pthread_cond_wait(&cond[id], &lock);
    }

    state[id] = EATING;
    std::cout << "Philosopher " << id << " is allowed to eat now\n";
    pthread_mutex_unlock(&lock);
}

void eat(int id) {
    int eatingTime = (rand() % 3) + 1;
    std::cout << "Philosopher " << id << " is eating for " << eatingTime << " seconds\n";
    sleep(eatingTime);
    std::cout << "Philosopher " << id << " reappears from sleep from eating\n";
}

void return_forks(int id) {
    int left  = LEFT;
    int right = RIGHT;

    pthread_mutex_lock(&lock);
    state[id] = THINKING;
    std::cout << "Philosopher " << id << " has put down forks\n";

    pthread_cond_signal(&cond[left]);
    std::cout << "Philosopher " << id << " signaled philosopher " << left  << " to see if it can eat\n";

    pthread_cond_signal(&cond[right]);
    std::cout << "Philosopher " << id << " signaled philosopher " << right << " to see if it can eat\n";

    pthread_mutex_unlock(&lock);
}

void* philosopher(void* num) {
    int id = *static_cast<int*>(num);
    while (true) {
        think(id);
        pickup_forks(id);
        eat(id);
        return_forks(id);
    }
    return nullptr;
}

int main() {
    if (pthread_mutex_init(&lock, nullptr) != 0) {
        std::cerr << "mutex init has failed\n";
        return 1;
    }
    for (int i = 0; i < NUMPHIL; i++) {
        if (pthread_cond_init(&cond[i], nullptr) != 0) {
            std::cerr << "cond init has failed\n";
            return 1;
        }
    }

    for (int i = 0; i < NUMPHIL; i++)
        pthread_create(&phil[i], nullptr, philosopher, &identity[i]);
    for (int i = 0; i < NUMPHIL; i++)
        pthread_join(phil[i], nullptr);

    pthread_mutex_destroy(&lock);
    for (int i = 0; i < NUMPHIL; i++)
        pthread_cond_destroy(&cond[i]);

    return 0;
}
