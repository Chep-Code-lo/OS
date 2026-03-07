#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUF_SIZE 5

static int buf[BUF_SIZE];
static int in_idx = 0, out_idx = 0;
static int next_num = 0;

static pthread_mutex_t mtx;
static sem_t empty_slots;
static sem_t full_slots;

static void put(int x) {
    buf[in_idx] = x;
    in_idx = (in_idx + 1) % BUF_SIZE;
}

static int get_item() {
    int x = buf[out_idx];
    out_idx = (out_idx + 1) % BUF_SIZE;
    return x;
}

static void* producer(void*) {
    while (true) {
        sleep(1);
        sem_wait(&empty_slots);
        pthread_mutex_lock(&mtx);
        int x = next_num++;
        put(x);
        std::cout << "P " << (unsigned long)pthread_self() << " -> " << x << "\n";
        pthread_mutex_unlock(&mtx);
        sem_post(&full_slots);
    }
    return nullptr;
}

static void* consumer(void*) {
    while (true) {
        sleep(1);
        sem_wait(&full_slots);
        pthread_mutex_lock(&mtx);
        int x = get_item();
        std::cout << "C " << (unsigned long)pthread_self() << " <- " << x << "\n";
        pthread_mutex_unlock(&mtx);
        sem_post(&empty_slots);
    }
    return nullptr;
}

int main() {
    std::ifstream fin("bounded_buffer_input.txt");
    if (!fin) {
        std::cerr << "Cannot open bounded_buffer_input.txt\n";
        return 1;
    }

    int run_seconds, P, C;
    fin >> run_seconds >> P >> C >> next_num;
    fin.close();

    pthread_mutex_init(&mtx, nullptr);
    sem_init(&empty_slots, 0, BUF_SIZE);
    sem_init(&full_slots, 0, 0);

    std::vector<pthread_t> pt(P), ct(C);
    for (int i = 0; i < P; i++)
        pthread_create(&pt[i], nullptr, producer, nullptr);
    for (int i = 0; i < C; i++)
        pthread_create(&ct[i], nullptr, consumer, nullptr);

    sleep(run_seconds);

    pthread_mutex_destroy(&mtx);
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    return 0;
}
