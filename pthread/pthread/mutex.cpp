#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include<mutex>

using namespace std;

//想完成一个抢票逻辑
//c++11里面早就已经可以支持mutex,我们不用原生线程库，
int ticket = 10; // ticket是一个临界资源

// ticket--是原子的吗？（安全的吗）
//为了让多个线程进行切换，线程什么时候可能切换（1.线程的时间片到了2.从内核态返回到用户态）

//对临界区进行加锁

class Ticket
{
public:
    Ticket() //构造函数
        : ticket(1000)
    {
        pthread_mutex_init(&mutex, nullptr);//c++不需要初始化，因为他是一个类
    }

    ~Ticket() //析构函数
    {
        pthread_mutex_destroy(&mutex);
    }

    bool GetTicket() //抢票
    {
        //加锁，安全的
        bool ret=true;//这个不会被所有线程所共享
        // pthread_mutex_lock(&mutex);
        
        //一行代码是原子的，只有一行汇编的情况下才是原子的，否则都不是原子的

        
        //这里的锁也是临界的资源
        mymtx.lock();//加锁使用c++版本的

        if (ticket > 0)
        {
            usleep(1000);
            cout << "我是"<<pthread_self()<<" 我要抢的票是 " << ticket << endl;
            ticket--;
            
        }
        else
        {
            cout<<"票已经被抢空了"<<endl;//这里因为没有同步，竞争能力强，竞争的人多了，就会
            ret=false;
        }
        // pthread_mutex_unlock(&mutex);//只有这一个执行流
        mymtx.unlock();//
        return ret;
    }

private:
    int ticket;
    pthread_mutex_t mutex;//原生线程库系统级别的
    std::mutex mymtx;//c++语言级别的,这里要加一个std::才可以不然的话没办法使用
    //mutex这里面一定封装了pthread_mutex,
    //我们现在学习的是语言底层使用的库，更靠近系统级别
    int lock;//lock--就是申请锁，lock++ 就是释放锁，if(lock>0){lock--}

};

void * threadrun(void *args)
{
    // string name=(char*)args;
    Ticket *t = (Ticket *)args;
    while (true)
    {
        if(!t->GetTicket())//进来无脑调用,
        {
            break;//抢票失败
        }
    }
}
int main()
{
    Ticket *t = new Ticket; //我们这里定义了一个票的类变量

    pthread_t tid[5];
    for (int i = 0; i < 5; i++)
    {
        int *id = new int(i); //这样的话每个线程都有一个
        //因为有可能我们其他线程会把这里的i给改掉
        pthread_create(&tid[i], nullptr, threadrun, (void *)t);
    }
    for (int i = 0; i < 5; i++)
        pthread_join(tid[i], nullptr);
    return 0;
}