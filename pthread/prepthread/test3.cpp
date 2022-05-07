#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
using namespace std;

/*

    信号量：该信号量允许继承和线程同步对共享资源进行返回（本质就是一个临界资源），也可以实现线程间同步
    ：其本质上是一个计数器，描述临界资源的个数，申请一个资源就对信号量+1（p操作），释放一个就对临界资源-1（v操作）
    ；临界资源可以看成多份，而且互不冲突高效
*/


/*
    信号量的接口都在semaphore.h里面
    信号量是一个sem_t 类型变量

    * 初始化信号量
    int sem_init(sem_t * sem,int pshared,unsigned int value)
     参数：
     sem信号量
     pshared ：0为线程间，非0为进程之间的
     value :信号量初始值

     返回值
     成功返回0，失败返回-1

    * 销毁信号量
    int sem_destroy(sem_t * sem)
    sem为信号量
    成功返回0，失败返回-1

    * 等待信号量
    int sem_wait（sem_t *sem）
    功能：
    等待信号量，会将信号量的值-1
    如果信号量为0就一直等待，如果信号量不为0就-1


    成功返回0，失败返回-1


    * 发布信号量

    int sem_post(sem_t *sem)
    功能：发布信号量，表示资源使用完毕，可以归还资源了，将信号量+1

    成功返回0 ，失败返回-1

    
    * 获取信号量的值
    int sem_getvalue(sem_t* sem,int* sval)
    sval也是一个输出型参数

    * 




*/

sem_t sem;

void* run1(void* arg)
{
    char* naem=(char*)arg;
    while(true)
    {
        sem_wait(&sem);//等待信号量，如果信号量的值为0就等待，相当于加锁，如果大于0就做-1操作
    
        cout<<"run1 is running ..."<<endl;
        sem_post(&sem);//发布信号量+1，开锁，其他的线程就可以得到这个锁
        cout<<"run1 unlock the lock"<<endl;
        // sleep(1);//sleep会出现优先唤醒的问题，线程1占者锁
    }
}

void* run2(void* arg)
{
    while(true)
    {
        sem_wait(&sem);
        cout<<"run2 is running .."<<endl;
        sem_post(&sem);
        cout<<"run2 unlock the lock"<<endl;
        // sleep(1);
    }
}

//信号量可以类似互斥锁的功能
//1表示开，0表示关
int main()
{
    sem_init(&sem,0,1);//初始化信号量，用0表示在线程间，1表示初始化为1，表示开
    int val;
    sem_getvalue(&sem,&val);
    cout<<"semaphore="<<val<<endl;
    pthread_t t1,t2;
    pthread_create(&t1,nullptr,run1,(void*)1);
    pthread_create(&t2,nullptr,run2,(void*)2);
    sem_destroy(&sem);
    pthread_join(t1,nullptr);
    pthread_join(t2,nullptr);


    return 0;
}