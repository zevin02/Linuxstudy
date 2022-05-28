#include"BlockQueue.hpp"
using namespace ns_blockqueue;
#include<time.h>
#include<cstdlib>



void* consumer(void* args)
{
    //两个角色已经是有了
    BlockQueue<int>* bq=(BlockQueue<int>*)args;//拿到了了阻塞队列

    while(true)
    {
        sleep(2);
        int data=0;//我们想把数据拿出来

        bq->Pop(&data);
        std::cout<<"消费者消费了一个数据："<<data<<std::endl;
    }
}
void* producer(void* args)
{
    BlockQueue<int>* bq=(BlockQueue<int>*)args;//拿到了了阻塞队列
    while(true)
    {
        // sleep(2);//按照生产者的节奏来走
        //我生产一条他消费一条
        //制造数据

        //生产者的数据（task）哪里来呢
        int data=rand()%20+1;
        std::cout<<"生产者生产数据："<<data<<std::endl;
        bq->Push(data);

    }
}

int main()
{
    srand((long long)time(nullptr));//种了一个随机数种子
    BlockQueue<int>* bq=new BlockQueue<int>(6);//我们动态开辟的一个空间，因为引入了模板，所以这里我们对他进行实例化一下顺便初始化一下
    pthread_t c,p;
    pthread_create(&c,nullptr,consumer,(void*)bq);//把阻塞队列传进去就可以看到同一个阻塞队列了
    pthread_create(&p,nullptr,producer,(void*)bq);
    pthread_join(c,nullptr);
    pthread_join(p,nullptr);
    return 0;
}