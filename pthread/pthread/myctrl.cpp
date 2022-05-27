#include <iostream>
#include <pthread.h>
#include <string>
#include<unistd.h>
using namespace std;

pthread_cond_t cond;
pthread_mutex_t mutex;


//ctrl thread控制work线程，让他定期运行




void* ctrl(void* args)
{
    string name=(char*)args;
    while(true)
    {
        //唤醒在条件变量下等待的一个线程，哪一个呢
        // pthread_cond_signal(&cond);//一个线程原来唤醒，一次唤醒一个，在condition等待的第一个线程
        pthread_cond_broadcast(&cond);
        cout<<"master say begin work"<<endl;
        sleep(1);
    }
}

void* work(void* args)//这里的线程原来执行工作流程
{
    int number=*(int*)args;
    delete args;
    while(true)
    {
        //每个线程跑完了，又继续在循环里面等待
        //条件变量内部一定存在一个等待队列，以及评判这个是否被唤醒的标志位
        pthread_cond_wait(&cond,&mutex);//z在该条件变量下进行等待
        cout<<"worker::"<<number<<"is working"<<endl;
        //我们会发现打印出来是有顺序的
    }
}




int main()
{
    pthread_t worker[5];
    pthread_t master;
    pthread_mutex_init(&mutex,nullptr);
    pthread_cond_init(&cond,nullptr);

    pthread_create(&master, nullptr, ctrl, (void *)"master");
    for (int i = 0; i < 5; i++)
    {
        int *number = new int(i);

        pthread_create(worker + i, nullptr, work, (void *)number);
    }
    for (int i = 0; i < 5; i++)
    {

        pthread_join(worker[i], nullptr);
    }
    pthread_join(master,nullptr);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    return 0;
}
