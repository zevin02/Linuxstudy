#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

//偶数号先左后右，奇数号哲学家先右后左，两个人保证手里有一把筷子，只要抢夺中间共用的筷子即可
sem_t chopstick[5];
pthread_mutex_t mutex;
/*
void *phip(void *arg)
{
    while (true)
    {
        long who = (long)arg;
        int left = who;
        int right = (who + 1) % 5;
        cout << who << " is thinking " << endl;
        if (who % 2 == 0)
        {
            pthread_mutex_lock(&mutex);
            cout << who << " try to eat" << endl;
            sem_wait(&chopstick[left]);
            sem_wait(&chopstick[right]);
            cout << who << " is eating " << endl;
            pthread_mutex_unlock(&mutex);
            sem_post(&chopstick[right]);
            sem_post(&chopstick[left]);

            cout << who << " continue thinking " << endl;
        }
        else
        {
            pthread_mutex_lock(&mutex);

            cout << who << " try to eat" << endl;

            sem_wait(&chopstick[right]);
            sem_wait(&chopstick[left]);
            cout << who << " is eating " << endl;
            pthread_mutex_unlock(&mutex);

            sem_post(&chopstick[left]);
            sem_post(&chopstick[right]);

            cout << who << " continue thinking " << endl;
        }

    }

}

*/
int philosophers[5] = {0, 1, 2, 3, 4};

void *phip(void *arg)
{
    int i = *(int *)arg;
    cout << i << endl;

    int left = i;
    int right = (i + 1) % 5;
    while (1)
    {
        if (0 == i % 2)
        {
            printf("哲学家正在思考\n");
            // usleep(100);
            // sleep(rand()%4);
            printf("哲学家%d号emo了准备吃\n", i);
            

            pthread_mutex_lock(&mutex);
            sem_wait(&chopstick[left]); //拿起筷子
            printf("哲学家%d拿起了%d号筷子,现在只有一根筷子\n", i, left);
            sem_wait(&chopstick[right]);
            printf("哲学家%d拿起了%d号筷子\n", i, right);
            pthread_mutex_unlock(&mutex);

            printf("哲学家%d现在拿了两根筷子开始吃\n", i);
            // usleep(100);
            // sleep(rand()%4);
            sem_post(&chopstick[left]);
            printf("哲学家%d放下了%d号筷子\n", i, left); //归还筷子
            sem_post(&chopstick[right]);
            printf("哲学家%d放下了%d号筷子\n", i, right); //归还筷子
        }
        else
        {
            printf("哲学家正在思考\n");
            printf("哲学家%demo了准备吃\n", i);
           
            pthread_mutex_lock(&mutex);
            // sleep(rand()%4);
            // usleep(100);

            sem_wait(&chopstick[right]); //拿起筷子
            printf("哲学家%d拿起了%d号筷子,现在只有一根筷子\n", i, left);
            sem_wait(&chopstick[left]);
            printf("哲学家%d拿起了%d号筷子\n", i, right);
            pthread_mutex_unlock(&mutex);

            printf("哲学家%d现在拿了两根筷子开始吃\n", i);
            // usleep(100);
            sem_post(&chopstick[left]);
            printf("哲学家%d放下了%d号筷子\n", i, left); //归还筷子
            sem_post(&chopstick[right]);
            printf("哲学家%d放下了%d号筷子\n", i, right); //归还筷子
        }
    }
    return NULL;
}

int main()
{
    pthread_t php[5];
    pthread_mutex_init(&mutex, nullptr);
    for (int i = 0; i < 5; i++)
    {
        sem_init(&chopstick[i], 0, 1);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&php[i], nullptr, phip, &philosophers[i]);
    }
    for (int i = 0; i < 5; i++)
    {
        pthread_join(php[i], nullptr);
    }
    for (int i = 0; i < 5; i++)
    {
        sem_destroy(&chopstick[i]);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}