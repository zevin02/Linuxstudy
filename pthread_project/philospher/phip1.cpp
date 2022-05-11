#include <iostream>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

sem_t chopstick[5];    //代表了筷子
pthread_mutex_t mutex; //这把锁代表了，一旦有哲学家开始吃饭，其他哲学家都不允许吃饭

//1. 获得两只筷子才可以吃饭
void *php(void *arg)
{

    long who = (long)arg;
    int left = who;
    int right = (who + 1) % 5;

    while (true)
    {
        long who = (long)arg;
        int left = who;
        int right = (who + 1) % 5;

        // thinking
        cout << "philospher " << who << " is thinking" << endl;
        usleep(100);
        //上锁
        //规定都是从左边筷子,右边筷子的顺序开始吃饭，一个哲学家吃饭别人别吃,把桌子给锁上
        pthread_mutex_lock(&mutex);
        sem_wait(&chopstick[left]);  //信号量里面有东西才吃饭
        sem_wait(&chopstick[right]); //信号量里面有
        cout << "philospher " << who << " is eating" << endl;

        //吃完就放回去
        //放回其中一个筷子，就能把桌子解锁了，一个就能组成两个筷子
        sem_post(&chopstick[left]);//吃完就放回去，全都放回去才能把锁解开吃饭
        sem_post(&chopstick[right]);
        pthread_mutex_unlock(&mutex);

        cout << "philospher " << who << " continue thinking" << endl;
    }
    return nullptr;
}

int main()
{
    pthread_mutex_init(&mutex, nullptr);
    pthread_t philospher[5];
    for (int i = 0; i < 5; i++)
    {
        sem_init(&chopstick[i], 0, 1); //每个筷子对应一个信号量
    }
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&philospher[i], nullptr, php, (void *)(i));
    }
    for (int i = 0; i < 5; i++)
    {
        pthread_join(philospher[i], nullptr);
    }

    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < 5; i++)
    {
        sem_destroy(&chopstick[i]);
    }
}