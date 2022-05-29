#pragma once

#include <iostream>
#include <vector>
#include<semaphore.h>
namespace ns_ring_queue
{
    const int g_cap_defualt=10;
    template <class T>
    class RingQueue
    {
    private:
        std::vector<T> _ring_queue;//用一个vector来做容器
        int _cap;//最大容量
        //生产者关系空位置资源
        sem_t _blank;
        sem_t _data;//初始化信号量
        int _c_step;
        int _p_step;
        pthread_mutex_t _c_mtx,_p_mtx;

    public:
        RingQueue(int cap=g_cap_defualt)
        :_cap(cap)
        ,_ring_queue(cap)//我们不初始化也问题不大，这里默认了这个vector就是有cap的大小个数
        ,_c_step(0)//初始化pc的位置
        ,_p_step(0)
         {
             sem_init(&_blank,0,cap);//一共有cap个空格子
             sem_init(&_data,0,0);//空格子数为0
             pthread_mutex_init(&_c_mtx,nullptr);
             pthread_mutex_init(&_p_mtx,nullptr);
         }
        ~RingQueue() {
            sem_destroy(&_data);
            sem_destroy(&_blank);
            pthread_mutex_destroy(&_c_mtx);
            pthread_mutex_destroy(&_p_mtx);
        }
        void Push(const T& in)
        {
            //多生产者之间要互斥

            //生产接口          
            //先预定资源
            //如果锁在前面，不是我们想要的那个样子，效率低，只允许一个执行流可以申请
            sem_wait(&_blank);//要成功申请一个格子资源，p(空位置)
            //能进来的人一定是把信号量申请成功了
            //多个生产者，可以预先分配给生产者，有锁进来，更新就可以了,这样的话，有人把锁释放了就可以立刻进行操作，
            pthread_mutex_lock(&_p_mtx);
            //可以生产了，可是往哪个位置生产呢
            _ring_queue[_p_step]=in;
            //数据放进来，空位子少了，数据多了
            _p_step++;
            _p_step%=_cap;//不能超过总数
            pthread_mutex_unlock(&_p_mtx);//保证互斥关系，生产者和生产者之间的互斥关系
            sem_post(&_data);
            

        }
        void Pop(T* out)
        {
            //多消费者之间要互斥c_step就是临界资源
                        
            //消费接口
            sem_wait(&_data);//如果这里一直为0，就一直等着，等到p进来
            pthread_mutex_lock(&_c_mtx);
            
            *out=_ring_queue[_c_step];
            //消费者取走了数据，这个地方就可以继续放数据了

            _c_step++;
            _c_step%=_cap;
            //不能超过总数
            pthread_mutex_unlock(&_c_mtx);//维护了消费者和消费者之间的互斥关系
            sem_post(&_blank);

        }
    };
}