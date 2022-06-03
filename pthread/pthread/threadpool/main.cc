#include"threadpool.hpp"
#include"Task.hpp"
#include<unistd.h>
#include<cstdlib>
#include<ctime>
using namespace ns_task;
using namespace ns_threadpool;

//线程池就是一个单例模式，只有一个线程池就够了

int main()
{
    ThreadPool<Task>* tp=new ThreadPool<Task>();
    tp->InitThreadPool();
    //我们希望是主线程不断的向线程池里面push任务，线程池里面竞争任务，处理这些任务
    //外部可能存在一个或者多个线程向里面塞任务
    srand((long long)time(nullptr));
    while(true)
    {
        //以后就是从网络里面来
        //主线程就是把任务放到线程池里面去
        
        //有的时候访问网站，挂掉了，OS受不了了，杀掉

        Task t(rand()%20+1,rand()%10+1,"+-*/%"[rand()%5]);
        tp->PushTask(t);//放进去让他进行处理工作
        

        
    }

    return 0;
}