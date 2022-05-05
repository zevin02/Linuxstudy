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

    while (1)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, NULL); //我们调用完这个之后会解锁
        if (data > 0)
        {
            pthread_cond_signal(&cond);
        }
        data--;
        cout << "consumer consume a data now data =" << data << endl;
        sleep(1);
        //操作完之后就立刻就上锁了

        pthread_mutex_unlock(&mutex); //
    }
    int *p = new int;
    *p = 0;
    pthread_exit(p);
}

void *P_func(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, NULL);
        if (data < capacity)
        {
            pthread_cond_signal(&cond);
        }
        data++;
        cout << "producer produce a data now data =" << data << endl;
        sleep(1);
        //操作完之后就立刻就上锁了

        pthread_mutex_unlock(&mutex); //
    }
    int *p = new int;
    *p = 0;
    pthread_exit(p);
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
