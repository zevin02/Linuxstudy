#include "threadpool.hpp"
#include "Task.hpp"
#include <unistd.h>
#include <cstdlib>
#include <ctime>
using namespace ns_task;
using namespace ns_threadpool;

//线程池就是一个单例模式，只有一个线程池就够了

int main()
{
    std::cout << "当前正在执行我的进程其他代码......" << std::endl;
    std::cout << "当前正在执行我的进程其他代码......" << std::endl;
    std::cout << "当前正在执行我的进程其他代码......" << std::endl;
    std::cout << "当前正在执行我的进程其他代码......" << std::endl;
    std::cout << "当前正在执行我的进程其他代码......" << std::endl;
    sleep(5);
    //前5秒是没有这个单例的
    //我们希望是主线程不断的向线程池里面push任务，线程池里面竞争任务，处理这些任务
    //外部可能存在一个或者多个线程向里面塞任务
    srand((long long)time(nullptr));
    while (true)
    {
        //以后就是从网络里面来
        //主线程就是把任务放到线程池里面去

        //有的时候访问网站，挂掉了，OS受不了了，杀掉
        sleep(1);
        Task t(rand() % 20 + 1, rand() % 10 + 1, "+-*/%"[rand() % 5]);
        ThreadPool<Task>::GetInstance()->PushTask(t); //在这里获得单例
        //即使是死循环入数据，也就只有一个对象，因为是静态的
        //只会调用一次
        //Getinstance
    }

    return 0;
}