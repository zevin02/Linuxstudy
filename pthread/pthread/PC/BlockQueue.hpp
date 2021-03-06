//.hpp 是 c++里面 在开源软件里面使用，声明和定义都可以放在一个文件里面

#pragma once
#include <iostream>
#include <queue>
#include <pthread.h>
#include<unistd.h>
namespace ns_blockqueue//使用一个自己的命名空间
{
    const int default_cap = 5;
    template <class T>//定义一个类模板

    class BlockQueue
    {
    private:
        std::queue<T> _bq;    //队列,使用了一个类模板
        int _cap;               //队列的元素上限
        pthread_mutex_t _mutex; //保证数据安全
        //当生产满了的时候，就不要生产了，不要生产锁了，让消费者来消费
        //当消费空了，就不要消费了，让生产了
        //这就要有两个条件变量
        pthread_cond_t _full;  //_bq满的，就要让消费者来消费，空了就要在这个条件变量等待
        pthread_cond_t _empty; //_bq空了，要让生产者来生产，生产者在该条件变量进行等待

    private:
        bool IsFull()
        {
            return _bq.size() == _cap;
        }

        void LockQueue()
        {
            pthread_mutex_lock(&_mutex);
        }
        void UnLockQueue()
        {
            pthread_mutex_unlock(&_mutex);
        }
        void WaitProduct()
        {
            //因为条件变量和互斥锁都搭配使用，我们进入临界资源，我们就是持有锁的
            // 1.调用的时候，会首先自动释放锁资源
            // 2.然后再挂起自己，这样别人就可以去申请锁
            // 3.返回的时候，会首先竞争锁，获取到锁之后，才能返回

            pthread_cond_wait(&_full, &_mutex);
        }

        bool IsEmpty()
        {
            return _bq.empty();
        }

        void WaitConsume()
        {
            //如果一直抱着锁被挂起的话，就会被永远挂起，死锁

            pthread_cond_wait(&_empty, &_mutex);
        }
        void WakeupConsumer()
        {
            pthread_cond_signal(&_empty);
        }
        void WakeupProducer()
        {
            pthread_cond_signal(&_full);
        }

    public:
        BlockQueue(const int cap = default_cap) //带一个缺省参数
            : _cap(cap)
        {
            pthread_mutex_init(&_mutex, nullptr);
            pthread_cond_init(&_full, nullptr);
            pthread_cond_init(&_empty, nullptr);
        }
        ~BlockQueue()
        {
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_full);
            pthread_cond_destroy(&_empty);
        }
        // const & 输入型参数
        //* 输出型参数
        //& 输入输出型参数

        //只有消费者知道，生产者什么时候可以生产
        void Push(const T &in) //向我们的队列中放数据
        {
            //在访问临界资源的时候，就应该把数据锁住
            LockQueue();
            //因为生产者消费者之间都是看同一个队列，所以这一把锁就已经够用了



            //临界区
            // if (IsFull())
            //我们需要进行条件检测的时候，这里需要使用循环的方式
            //来保证退出循环一定是条件不满足导致的
            while (IsFull())
            {
                //等待，把线程挂起，我们当前是持有锁的，
                //如果队列是空的话就不应该生产了，而是在那里等待

                //1. 如果我挂起失败呢，因为函数调用有成功有失败
                //函数调用失败


                //2. 如果我被伪唤醒呢（条件还没有就绪）
                //如果是多核多CPU的，很多线程都在条件变量下等待



                WaitProduct();
                //我醒来之后要再进行一次判断，判断是否为满，判断成功就往下走，
            }
            //用if判断，有可能当前队列还是满的，再向下走的话，就会插入一个不应该插入的数据

            //生产函数
            _bq.push(in);

            //唤醒消费者,这里我们制定的策略是，有一个就唤醒一个
            if(_bq.size()>_cap/2)//改变策略
            WakeupConsumer();

            UnLockQueue(); //解锁
        }
        //只有生产者知道，消费者什么时候可以生产
        void Pop(T *out) //向队列里面拿数据
        {
            LockQueue();
            // if(IsEmpty())
            while(IsEmpty())
            {
                //无法消费
                WaitConsume();
            }
            //消费函数
            *out = _bq.front(); //拿到队头的元素
            _bq.pop();          //删除数据
            if(_bq.size()<_cap/2)
            WakeupProducer();//唤醒生产者,这里的wakeup放在里面和外面都是可以的
            UnLockQueue();
        }
    };
}
