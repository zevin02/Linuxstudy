#pragma once
#include <iostream>
#include <string>
#include <queue>
#include<unistd.h>
#include<pthread.h>
// #cludine"Task.hpp"
// using namespace ns_task;
namespace ns_threadpool
{
    const int g_num=5;
    template <class T>
    class ThreadPool //线程池
    {

    private:
        int num_;                  //一个线程池里面有多少个任务
        std::queue<T> task_queue_; //任务队列,临界资源
        pthread_mutex_t mtx_;
        pthread_cond_t cond_;

    public:
        ThreadPool(int num=g_num) :num_(num)
        {
            pthread_mutex_init(&mtx_,nullptr);
            pthread_cond_init(&cond_,nullptr);
        }
        
        
        ~ThreadPool()
        {
            pthread_mutex_destroy(&mtx_);
            pthread_cond_destroy(&cond_);
        }

        //在类中，要让

        static void* Rountine(void* args)
        //也不能访问类里面非static成员
        {

            pthread_detach(pthread_self());//实现线程分离就不要再去join等待了
            ThreadPool<T>* tp=(ThreadPool<T>*)args;
            while(true)
            {
                //从任务队列里面去拿一个任务
                //执行任务，要先把这个任务队列锁主

                //每个线程他跟放任务的线程一样，都是竞争式的去拿一个任务
                tp->Lock();
                //先检测任务队列是否有一个任务
                while(tp->IsEmpty())
                {
                    //检测到任务队列为空
                    //此时线程就挂起等待
                    tp->Wait();

                }
                //该任务队列里面一定有任务了
                T t;
                tp->PopTask(&t);
                //任务就拿到了
                tp->UnLock();
                t.Run();//可能有多个线程在处理任务

                sleep(1);
            }
        }
        void InitThreadPool()
        {
            //初始化一批线程，
            //这样就不要每次用都要去开辟线程了
            pthread_t tid;//一次创建一批线程
            for(int i=0;i<num_;i++)
            {
                pthread_create(&tid,nullptr,Rountine,(void*)this);
                //在类中不能执行线程的方法，因为他都有隐藏的this指针
                //所以我们需要使用静态的函数，就没有了this指针
            }
        }
        void PopTask(T* out)
        {
            *out=task_queue_.front();
            task_queue_.pop();
        }
        void Wait()
        {
            pthread_cond_wait(&cond_,&mtx_);
        }
        bool IsEmpty()
        {
            return task_queue_.empty();
        }
        void Lock()
        {
            pthread_mutex_lock(&mtx_);
        }
        void UnLock()
        {
            pthread_mutex_unlock(&mtx_);
        }
        void Wakeup()
        {
            pthread_cond_signal(&cond_);
        }
        void PushTask(const T & in)
        {
            //塞任务，就相当于一个生产者，生产者之间要进行互斥访问
            Lock();
            task_queue_.push(in);
            UnLock();
            Wakeup();
        }

        //万一任务队列里面一个任务都没有的话，那么线程池里面的每一个线程就要处于休眠状态，挂起等待
        
    };
}