#include <bits/stdc++.h>
#include <pthread.h>

#define NUM_THREADS 4

using namespace std;

class Barrier
{
    int total;
    int remaining;
    pthread_mutex_t lock;
    pthread_cond_t release;
    
public:
    Barrier(int count)
    {
        this->total = count;
        this->remaining = count;
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&release, NULL);
    }
    
    ~Barrier()
    {
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&release);
    }
    
    void wait()
    {
        pthread_mutex_lock(&lock);
        if (remaining == 1)
        {
            remaining = total;
            pthread_cond_broadcast(&release);
        }
        else
        {
            remaining--;
            pthread_cond_wait(&release, &lock);
        }
        pthread_mutex_unlock(&lock);
    }
};

struct ThreadArg
{
    int id;
    Barrier *barrier;
};

void *threadFunction(void *arg)
{
    int id = ((ThreadArg*)arg)->id;
    Barrier *barrier = ((ThreadArg*)arg)->barrier;
    printf("Thread %d before the barrier\n", id);
    barrier->wait();
    printf("Thread %d after the barrier\n", id);
    pthread_exit(NULL);
}

int main()
{
    int numThreads = NUM_THREADS;
    pthread_t threads[NUM_THREADS];
    ThreadArg threadArgs[NUM_THREADS];
    Barrier barrier(numThreads);
    for (int i = 0; i < numThreads; ++i)
    {
        threadArgs[i].id = i;
        threadArgs[i].barrier = &barrier;
    }
    for (int i = 0; i < numThreads; ++i)
    {
        pthread_create(&threads[i], NULL, threadFunction, &threadArgs[i]);
    }
    for (int i = 0; i < numThreads; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
