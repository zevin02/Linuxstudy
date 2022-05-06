#include <iostream>
using namespace std;
#include <memory>
#include <pthread.h>
#include <unistd.h>

int capacity = 5;
pthread_cond_t cond;
// pthread_cond_t condp;
pthread_mutex_t mutex;
int data = 0;
void *C_func(void *arg)
{

    while (true)
    {
        pthread_mutex_lock(&mutex);
        while(data==0)//如果消费者发现缓存里面没有数据，那么我们就应该挂起来等待
        pthread_cond_wait(&cond, &mutex); //我们调用完这个之后会解锁
        
        data--;
        cout << "consumer consume a data now data =" << data << endl;
        usleep(100);
        //操作完之后就立刻就上锁了

        pthread_mutex_unlock(&mutex); //
        pthread_cond_signal(&cond);//消费完了就把生产者唤醒，告诉它可以继续生产了，
    }
    int *p = new int;
    *p = 0;
    pthread_exit((void*)p);
    // return NULL;
}

void *P_func(void *arg)
{
    while (true)
    {
        pthread_mutex_lock(&mutex);
        while(data==capacity)//如果发现缓存里面的数据已经满了，就应该挂起来等待消费者拿数据，
        pthread_cond_wait(&cond, &mutex);
        
        data++;
        cout << "producer produce a data now data =" << data << endl;
        usleep(100);
        //操作完之后就立刻就上锁了

        pthread_mutex_unlock(&mutex); //
        pthread_cond_signal(&cond);//生产完了就把消费者唤醒告诉他里面有数据了
    }
    int *p = new int;
    *p = 0;
    pthread_exit((void*)p);
    // return NULL;
}

int main()
{

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_t consumer, producer;
    pthread_create(&consumer, NULL, C_func, (void *)"consumer consume ");
    pthread_create(&producer, NULL, P_func, (void *)"producer produce ");
    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return 0;
}
