#include "ring_queue.hpp"
#include <pthread.h>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
using namespace ns_ring_queue;
using std::cout;
using std::endl;

void *consumer(void *args)
{
    RingQueue<int> *rq = (RingQueue<int> *)args;
    while (true)
    {
        int data = 0;
        rq->Pop(&data);
        cout << "消费的数据是" << data <<"我是"<<pthread_self()<< std::endl;
        sleep(1);
    }
}
void *producter(void *args)
{
    RingQueue<int> *rq = (RingQueue<int> *)args;
    while (true)
    {
        int data = rand() % 20 + 1;
        cout << "生产了数据是:" << data <<"我是"<<pthread_self()<< std::endl;
        rq->Push(data);
        sleep(1);
    }
}

int main()
{
    //如果我们想要改成多生产者多消费者模型
    srand((long long)time(nullptr)); //种下随机数种子
    pthread_t c, p;
    pthread_t c1, p1;
    RingQueue<int> *rq = new RingQueue<int>(); //看到了同一个资源
    pthread_create(&c, nullptr, consumer, (void *)rq);
    pthread_create(&p, nullptr, producter, (void *)rq);
    pthread_create(&c1, nullptr, consumer, (void *)rq);
    pthread_create(&p1, nullptr, producter, (void *)rq);
    pthread_join(c, nullptr);
    pthread_join(p, nullptr);
    pthread_join(c1, nullptr);
    pthread_join(p1, nullptr);
    return 0;
}