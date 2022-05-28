#include"BlockQueue.hpp"
#include"Task.hpp"

using namespace ns_blockqueue;
#include<cstdlib>
#include<ctime>

using namespace ns_task;

void* consumer(void* args)
{
    //两个角色已经是有了
    BlockQueue<Task>* bq=(BlockQueue<Task>*)args;//拿到了了阻塞队列

    while(true)
    {
        // sleep(2);
        // int data=0;//我们想把数据拿出来
        // bq->Pop(&data);
        // //data就是一个输出型参数
        // std::cout<<"消费者消费了一个数据："<<data<<std::endl;

        //消费者这里需要获得任务，无参构造就行了
        Task t;
        bq->Pop(&t);//任务拿出来了，消费的第一步
        //拿出来之后我们就要对任务进行处理 
        // t.Run();
        t();//拿到任务，直接返回,消费的第二步

    }
}
void* producer(void* args)
{
    BlockQueue<Task>* bq=(BlockQueue<Task>*)args;//拿到了了阻塞队列
    std::string ops="+-*/%";

    while(true)
    {
        // sleep(2);//按照生产者的节奏来走
        //我生产一条他消费一条
        //制造数据

        //生产者的数据（task）哪里来呢
        // int data=rand()%20+1;
        // std::cout<<"生产者生产数据："<<data<<std::endl;
        // bq->Push(data);
        
        //1. 生产一个任务
        sleep(1);
        int x=rand()%20+1;
        int y=rand()%10+1;
        char op=ops[rand()%5];//0-4之间,进行操作运算
        Task t(x,y,op);
        std::cout<<"生产派发了一个任务"<<x<<op<<y<<"="<<"?"<<std::endl;
        
        //2.把数据放到队列里面
        
        bq->Push(t);//把任务塞进去了


    }
}

int main()
{
    srand((long long)time(nullptr));//种了一个随机数种子
    BlockQueue<Task>* bq=new BlockQueue<Task>(6);//我们动态开辟的一个空间，因为引入了模板，所以这里我们对他进行实例化一下顺便初始化一下
    pthread_t c,p;
    pthread_t c1,c2,c3,c4;
    pthread_create(&c,nullptr,consumer,(void*)bq);//把阻塞队列传进去就可以看到同一个阻塞队列了
    pthread_create(&c1,nullptr,consumer,(void*)bq);//把阻塞队列传进去就可以看到同一个阻塞队列了
    pthread_create(&c2,nullptr,consumer,(void*)bq);//把阻塞队列传进去就可以看到同一个阻塞队列了
    pthread_create(&c3,nullptr,consumer,(void*)bq);//把阻塞队列传进去就可以看到同一个阻塞队列了
    pthread_create(&c4,nullptr,consumer,(void*)bq);//把阻塞队列传进去就可以看到同一个阻塞队列了
    pthread_create(&p,nullptr,producer,(void*)bq);
    pthread_join(c,nullptr);
    pthread_join(p,nullptr);
    delete bq;
    return 0;
}