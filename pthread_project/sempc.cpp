#include <iostream>
using namespace std;
#include <memory>
#include <pthread.h>
#include <unistd.h>
#include<signal.h>
#include<semaphore.h>
#include<queue>
#include<string>
#include<cstring>
int capacity = 5;

int mesgid=1;//消息的计数器
//定义一个缓存的消息的结构体
struct st_message
{
    int mesgid;
    string messages;

};

queue<st_message> vcache;//用一个队列来保存

sem_t sem;
pthread_mutex_t mutex;
void incache(int sig)
{
    st_message stmesg;//定义一个结构体，往里面放数据
    memset(&stmesg,0,sizeof(stmesg));//初始化
    pthread_mutex_lock(&mutex);//加锁
    //生产数据放入缓存
    stmesg.mesgid=mesgid++;
    string m="producer produce a data";
    stmesg.messages=m;
    vcache.push(stmesg);
    pthread_mutex_unlock(&mutex);
    sem_post(&sem);//发布信号，信号俩+1

}

//消费者
void* outcache(void* arg)
{
    st_message stmesg;
    while(true)
    {
        while(vcache.empty())
            //如果缓存里面没有数据，就要等待
        {
            sem_wait(&sem);
        }
        //如果缓存里面有数据就不用wait了
        //wait是要把信号量-1
        //看到有数据我们还要去加锁，因为这两步不是原子操作，因为还没上锁，所以别的线程可能正在操作

        pthread_mutex_lock(&mutex);
        //还要再判断缓存里面是否有数据
        if(vcache.empty())
        {
            //没数据就不要用了
            //解锁
            pthread_mutex_unlock(&mutex);
            continue;
        }
        memcpy(&stmesg,&vcache.front(),sizeof(stmesg));
        vcache.pop();//删除数据
        pthread_mutex_unlock(&mutex);
        cout<<stmesg.mesgid<<"consume"<<stmesg.mesgid<<endl;

    }
    return nullptr;
}

int main()
{
    pthread_t t1,t2,t3;
    signal(15,incache);//接收15的信号，调用生产者函数、
    cout<<"1"<<endl;
    sem_init(&sem,0,1);//初始化一个信号量，在线程之间进行交互，信号量初始值设置为1
    
    
    pthread_mutex_init(&mutex,nullptr);
    

    pthread_create(&t1,nullptr,outcache,(void*)"t1");//消费者函数
    
    pthread_create(&t2,nullptr,outcache,(void*)"t2");
  
    pthread_create(&t3,nullptr,outcache,(void*)"t3");
    // pthread_join(t1,nullptr);
    
    
    // pthread_join(t2,nullptr);
    // pthread_join(t3,nullptr);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem);
    return 0;
}
