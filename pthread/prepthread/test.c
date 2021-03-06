
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

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
//线程创建

// int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);
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

/*

int val=0;//全局变量会被共享使用
void* Routine(void *arg)
{
    while(1)
    {
        printf("I am %s....val:%d id:%lld\n",(char*)arg,val,pthread_self());
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

    //在linux调度的时候，线程与LWP是一一对应的，操作系统调度的时候采用的是LWP ，而不是PID，只不过我们之前都是单线程进程
    //所以者PID 和LWP 是一样的
     //tid
    return 0;
}
*/

/*
    线程id


    先有进程再有线程


    LWP(内核中的id)
    pthread_creat会给第一个参数传递线程 的id，但是这个线程ID 和我们前面说的LWP 不是一个概念，对应的数值也不一样
    LWP，线程是一个轻量级的进程，是操作系统调度的最小单位，所以我们需要一个数值来唯一标识该线程
    在linux系统层面，有LWP 会与线程对应，但是对于用户来说只需要知道
    在共享区还有TCB（线程控制块），用于用户操作线程

    我们查看的是线程库里面的线程id，不是linux内核的LWP
    pthread里面的线程id：内存地址（虚拟地址）

    pthread_t pthread_self(void),获得线程id，
    可以类比getpid(void)，获得进程id

    获取线程自身的ID，
    这个ID 本质上就是进程地址空间上的一个地址

*/

/*
    线程终止
    1.在线程函数中return（在main函数中return，代表主线程和进程退出，其他的退出就只是一个线程退出）
    2.线程可以调用pthread_exit来终止自己
    3.线程可以调用pthread_cancel终止同一进程的另一个线程或者自己
    //强调：无论在那里使用exit或return都是终止整个进程


    * void pthread_exit(void * value_ptr)
    1.功能：线程终止
    2.参数：value_ptr 线程退出传出的数据（不要去指向一个局部变量）,类比exit(1)
    3.返回值：void，
    注意：pthread_exit或者return返回的指针必须是全局的，或者malloc出来的，因为如果在线程函数上开辟的，线程退出，这个返回指针也退出了

    exit是终止进程，不能在线程中调用，在线程使用的时候，exit把进程直接全部都死掉了

    int pthread_cancel(pthread_t thread)
    但是我们一般不使用这个方法

    1.功能：取消执行中的一个线程
    2.thread表示要被执行的线程ID，
    3.返回值：成功返回0，失败返回错误码，和create一样
    pthread_cancel函数具有延迟性，不会被立即处理，
    不建议线程被创建后立即进行cancel（线程创建不会被立即调度）；也不建议在线程退出前执行cancel
    应该要在线程执行中进行cancel取消
    其他线程也可以干掉主线程，但是主线程被干掉了不影响其他线程继续运行，但是不建议用新线程取消主线程
    被取消的话，他的退出结果就是PTHREAD_CANCELED(-1)


    线程等待
    1.对于已经退出的线程，其空间没有被释放，还在进程地址空间李里面，我们新创建的线程不会复用刚才退出的线程的地址，就会造成资源浪费，内存泄漏
    可以类比父进程要wait子进程，
    2.都是为了获取线程的退出信息

    int pthread_join(pthread_t thread,voi d** value_ptr)

    1.功能：等待线程的结束
    2.参数：thread：指定要等待的线程的ID，value_ptr是一个输出型参数，获取指定线程退出的返回值
    3.返回值，成功返回0，失败返回错误码

    注意
    1.调用该函数的线程将会挂起等待，知道id为thread的线程终止为止
    2.进程等待收到的信息，并不会有终止信号的信息，因为终止信号是对于整个进程而言的，无论谁收到这个信号都是会崩溃的
    3.thread线程以不同的方式终止，通过pthread_join获取的终止状态是不一样的

    1.return返回，里面会存放thread线程函数的返回值
    2.pthread_cancel异常退出，会存放PTHREAD_CANCELED
    3.pthread_exit会有pthread_exit里面的参数
    不感兴趣的话传NULL 也可以

*/

/*
void* thread1(void * arg)
{
    printf("%s returning.....\n",(char*)arg);
    int *p =(int*)malloc(sizeof(int));
    *p=10;
    return (void*)p;//返回值//使用return进行执行线程退出，这里使用的是malloc开辟出来的空间

}

void* thread2(void* arg)
{
    printf("%s exiting.....\n",(char* )arg);
    int* p=(int*)malloc(sizeof(int));
    *p=2;
    pthread_exit((void*)p);

}

void* thread3(void* arg)
{
    while(1)
    {
        printf("%s is running ...\n",(char*)arg);
        sleep(1);
    }
    return NULL;
}
int main()
{
    pthread_t tid;
    void* ret;
    //第一次我们使用return退出

    pthread_create(&tid,NULL,thread1,(void*)"thread 1");
    //线程执行完之后要进行等待
    pthread_join(tid,&ret);//ret里面存放的就是tid1线程里面对应的退出信息
    printf("thread 1 return,thread id %x,return code;%d\n",tid,*(int*)ret);//打印出
    free(ret);//因为ret是一个输出性参数，所以在底层一定是对它进行malloc的，所以在没用的情况下要把它free掉
    //同时我们之后也都能用到这两个变量

    //第二次我们使用pthread_exit退出
    pthread_create(&tid,NULL,thread2,(void*)"thread 2");
    pthread_join(tid,&ret);
    printf("thread 2 exiting ,thread id:%x,return code:%d\n",tid,*(int*)ret);//我们要先强制类型转换，对其进行解引用
    free(ret);

    //第3次，我们使用pthread_cancel进行退出
    pthread_create(&tid,NULL,thread3,(void*)"thread 3");
    sleep(3);//因为不建议线程创建后立即cancel，所以我们等上一会，让它在线程执行中被取消
    pthread_cancel(tid);
    pthread_join(tid,&ret);
    if(ret==PTHREAD_CANCELED)
    {
        printf("thread 3 cancel ,thread id %x ,return code : PTHREAD_CANCELED-->%d\n",tid,ret);
    }
    else
    {
        printf("thread return ,thread id %x ,return code :%d\n",tid,ret);
    }
    pthread_join(tid,&ret);
    return 0;
}

*/


/*


int val = 0;
struct Ret
{
    int exitno;
    int exittime;
    //
};

void *Routine(void *arg)
{
    int cnt = 1;
    while (1)
    {
        printf("I am %s..val:%d\n", (char *)arg, val);
        sleep(1);
        cnt++;
        if (cnt == 3)
        {
            struct Ret *p = (struct Ret *)malloc(sizeof(struct Ret));
            p->exitno = 0;
            p->exittime = 6666;
            pthread_exit((void *)p); //线程退出，
        }
    }
}

int main()
{
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, Routine, (void *)"thread 1"); //执行同一个函数
    pthread_create(&tid2, NULL, Routine, (void *)"thread 2");//数据共享，基本同时执行，所以第一次打印出来的值都一样
    pthread_create(&tid3, NULL, Routine, (void *)"thread 3");
    int cnt = 0;
    while (1)
    {
        printf("I am main pthread..val: %d\n", val++);
        sleep(1);
        cnt++;
        if (cnt == 3)
        {
            break;
        }
    }
    printf("wait for pthread...\n");
    void *ret;
    pthread_join(tid1, &ret);
    printf("pthread id: %x ,exitno:%d,exittime:%d\n", tid1, ((struct Ret *)ret)->exitno, ((struct Ret *)ret)->exittime);
    pthread_join(tid1, &ret);
    printf("pthread id: %x ,exitno:%d,exittime:%d\n", tid2, ((struct Ret *)ret)->exitno, ((struct Ret *)ret)->exittime);
    pthread_join(tid1, &ret);
    printf("pthread id: %x ,exitno:%d,exittime:%d\n", tid3, ((struct Ret *)ret)->exitno, ((struct Ret *)ret)->exittime);
    return 0;
}


*/


/*
    线程分离（我们不想等待了）
    分离之后的线程不需要join，运行完毕之后，会自动释放Z，pcb
    ===signal(SIG_CHILD,SIG_IGN)
    相当于同一个屋檐下的陌生人

    1.默认情况下，新创建的线程是joinable的，线程退出的时候，需要对他进行pthread_join操作
    2.如果不关心线程的返回值，那么join是一个负担，可以告诉系统 ，线程退出的时候，自动释放线程资源

    int pthread_detach(pthread_t thread)
    成功返回0，失败返回错误码
    一般都是自己分离自己
    设置为detach的状态，让os来回收它的资源，会自动释放它的空间

    1.可以其他线程对目标线程进行分离，也可以自己分离自己
    2.join和分离是冲突的，一个线程要么分离要么joinable，分离了就不要join了 
    3.线程分离具有一定的延迟性，分离后如果再进行等待，那么得到的结果未定义
    4.如果分离的线程程序崩溃，同样整个线程也会崩溃，线程分离之后只是在回收的时候进行自动回收，如果主线程退出，那么整个线程也会退出

    一般主线程不退出。，新线程处理安业务了就退出
    默认线程都是joinable的
    
*/




void* Routine(void* arg)
{
    pthread_detach(pthread_self());//我们在这里就给它注册上，让它实行线程分离
    printf("%s detach success!\n");
    int cnt=0;
    while(cnt<5)
    {
        cnt++;
        printf("%s running ...cnt:%d\n",(char*)arg,cnt);
        sleep(1);
    }
    printf("%s return ...\n");
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,Routine,(void*)"thread");
    sleep(3);//执行3秒之后就会自动实现线程分离，就退出了，没有等待的话就会直接出错，所以要等待一会，让线程先分离
    void* ret;
    if(pthread_join(tid,&ret)==0)
    {
        printf("thread join success ! ret:%d",*(int*)ret);
    }
    else 
    printf("thread join fail.. ret:%d\n",*(int*)ret);
    return 0;
}

//我们一般不使用程序替换在线程里面，程序替换是和进程强相关的 