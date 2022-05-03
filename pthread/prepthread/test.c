
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include<pthread.h>
#include<string.h>

/*

fork是对父进程代码和数据进行写实拷贝，在没有用的时候，子进程和父进程数据一样，在用的时候，会进行拷贝，确保两者数据不相同


vfork
1.会与父进程共享数据段
2.会保证子进程先运行，在他调用完exec（进程替换）或exit（进程退出）之后，父进程才会进行调度
所以如果子进程没有exec或exit的话，程序会导致死锁，程序是有问题的
*/


/*

int main()
{
    int val = 100;
    pid_t id = vfork();
    if (id == 0)
    {
        //子进程先被运行

        // child

        int cnt = 0;
        while (1)
        {
            printf("i am child pid :%d,val :%d,&val :%p\n", getpid(), val, &val);
            cnt++;
            sleep(1);
            if (cnt == 2)
            {
                val = 200;
            }
            if (cnt == 5)
            {
               // exit(0); //调用了exit这样才可以使用父进程，不然的话就会一直停在这里，一直执行子进程
            }
        }
    }
    else if (id > 0)
    {
        //父进程
        int cnt = 0;
        while (1)
        {
            printf("i am father pid :%d,val :%d,&val :%p\n", getpid(), val, &val);
            cnt++;
            sleep(1);
            if(cnt==3)
            {
                val=300;
            }
        }
    }
    return 0;
}

*/

//int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);
/*
    功能：创建一个新线程
    参数：
    1.thread是一个输出型参数（调用这个函数，就会给thread里面赋值），可以返回线程的id值
    2.attr，是设置线程的属性，attr为NULL 说明使用的是默认属性，我们现在什么都不知道，就直接给他设置为NULL就行了
    3.start_routine，是一个函数指针，我们创建的线程，线程要执行的操作就都在这个里面了，返回值为void*，参数是void*，
    4.arg是传给线程启动函数的参数，就是我们的参数

    返回值：成功返回0，失败返回一个错误码

    主线程创建了一个新线程，此后新线程会去执行它要执行的操作，而主线程会继续往下执行它的操作
*/  
int val=0;//全局变量会被共享使用
void* Routine(void *arg)
{
    while(1)
    {
        printf("I am %s....val:%d \n",(char*)arg,val);
        sleep(1);
    }
}
int main()
{   
    pthread_t tid1,tid2,tid3;
    int ret1=pthread_create(&tid1,NULL,Routine,(void*)"pthread 1");
    //ret1=0就代表正常返回，否则就代表错误返回,错误会返回错误码
    if(ret1!=0)
    {
        fprintf(stderr,"pthread_creat : %s\n",strerror(ret1));//stderror会以字符串的形式对错误码进行描述
        //输出给标准错误流，打印到显示器上面
        exit(1);
    }
    //因为1比2早创建，所以1比2早进行调度

    int ret2=pthread_create(&tid2,NULL,Routine,(void*)"pthread 2");
    if(ret2!=0)
    {
        fprintf(stderr,"pthread_creat2 %s\n",strerror(ret2));
        exit(1);
    }
    while(1)
    {
        printf("I am main pthread ... val;%d \n",val++);//如果没有sleep （1）的话，新线程可能还没来的及执行它的东西，线程就结束了，所以我们要让主线程多执行一会才可以
        sleep(1);
    }

    
    return 0;
}



