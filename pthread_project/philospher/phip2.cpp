#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
using namespace std;

//2.把5个筷子当作5把锁

pthread_mutex_t chopstick[5];

/*

pthread_mutex_trylock(&mutex)
如果我们要的锁没有被锁住，那么就会上锁，如果这个锁已经上锁了，就会返回一个消息
成功返回0，说明这个锁没有被锁住，调用了之后，这个锁就已经上锁了




*/
void * php(void* arg)
{
    while(true)
    {
        long who=(long )arg;
        int left=who;
        int right=(who+1)%5;
        cout<<"pthilospher "<<who<<" is thinking"<<endl; 
        usleep(100);
        //先拿起来左边筷子,尝试一下拿起来右边筷子
        //如果这个右边的筷子已经被人上锁了的话，那么左边的筷子也就没有必要让我们拿起来了，可以解锁让别人拿起来
        pthread_mutex_lock(&chopstick[left]);
        cout<<"pthilosper "<<who<< " try to eat"<<endl;

        if(pthread_mutex_trylock(&chopstick[right])!=0)
        {
            //说明右边的筷子已经被锁了
            cout<<"philosper "<<who<<" can not eating"<<endl;
            pthread_mutex_unlock(&chopstick[left]);
            continue;
        }
        //走到这里说明没有被锁住
        //因为使用了trylock 所以已经上锁了

        cout<<"philospher "<<who<<" is eating"<<endl;
        usleep(100);
        pthread_mutex_unlock(&chopstick[left]);
        pthread_mutex_unlock(&chopstick[right]);
        cout<<"pthilospher "<<who<<" is continue thinking"<<endl; 
        usleep(100);

    }
    return nullptr;
}



int main()
{
    pthread_t phip[5];
    for(int i=0;i<5;i++)

    {
        pthread_mutex_init(&chopstick[i],nullptr);
    }
    for(int i=0;i<5;i++)

    {
        pthread_create(&phip[i],nullptr,php,(void*)i);
    }
    for(int i=0;i<5;i++)

    {
        pthread_join(phip[i],nullptr);
    }
    for(int i=0;i<5;i++)

    {
        pthread_mutex_destroy(&chopstick[i]);
    }

    return 0;
}