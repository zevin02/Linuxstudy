#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/*


    线程互斥：在任何时候，保证只有一个执行流进入临界区（公共的代码区），访问临界资源(能够被公共使用的部分)，对临界资源进行保护
    因为线程共享资源

    互斥量
    ：多个线程对一个共享变量进行操作的时候，会引发数据不一致的问题，就引入了互斥量的概念（互斥锁），保证数据的完整性
    在被加锁的任意时刻，临界区的代码只能被一个线程所访问


*/

//我们使用抢票程序学习一下多个线程操作共享资源产生的影响，


/*
int ticket = 100; //用一个全局变量来代表对ticket进行的操作，直到最后为0就停止操作

void *getticket(void *arg)
{
    long id = (long)arg; //对类型进行强转一下
    while (1)
    {
        if (ticket > 0)
        {
            //有票
            usleep(100); //挂起的时间以微秒为单位
            ticket--;
            printf("thread %ld get a ticket ,the number of remainint is %d\n", id, ticket);
        }
        else
        {
            //没有票
            break;
        }
    }
}

int main()
{
    pthread_t t[5]; //我们创建了5个线程
    long i = 0;
    for (i = 0; i < 5; i++)
    {
        pthread_create(&t[i], NULL, getticket, (void *)i); //创建了5个线程,5个线程同时对票进行操作
    }
    // 释放5个线程
    for (i = 0; i < 5; i++)
    {
        pthread_join(t[i], NULL); //我们不关心里面的退出情况，就可以设置为NULL
    }
    return 0;
}


*/


/*
    我们发现票最会变成了一个负数，不符合情况
    分析原因
    1.if条件为真的话，此时代码可以同时切换到其他线程
    2.usleep这操作，在这个过程之中，这个线程的ticket还没进入--，就可能会有其他的线程进入if语句当中
    3.ticket操作本省就不是一个原子操作
    当一个线程正在准备执行第三条指令的时候，有一个线程刚刚好在执行第二个指令，这个时候寄存器的值又减了一次，所以当一个线程执行完
    一整个流程的时候，ticket其实是减少了两次，所以这个操作并不是原子行为（要么不做，要么做到底）


*/

/*
    解决：
    1.代码必须要又互斥行为：当代码进入临界区执行的时候，不能允许其他的线程进入临界区
    2.如果多个线程要同时执行临界区的代码的话，并且临界区没有代码在执行，那么就只允许一个线程可以进入该临界区
    3.如果线程不再临界区执行，那么该线程不能阻止其他线程进入临界区执行

*/

/*
    互斥量是一把锁，类型为pthread_mutex_t的变量，使用前要进行初始化，使用完之后要对锁的资源进行释放

    1. 初始化互斥量
    * 静态分配: static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER
    静态或者全局的都可以，初始化，和销魂就不需要再去调用那些函数了
    * 动态分配：
    int pthread_mutex_init(pthread_mutex_t * restrict mutex,const pthread_mutexattr_t* restrict attr)

    参数1
    1.restrict mutex要初始化的锁
    2.restrict attr 不关心就NULL
    返回值L:
    成功返回0，失败返回错误码

    注意：
    * 互斥量处于未锁状态，该函数会被互斥量锁定，同时返回成功
    * 函数调用的时候，其他线程已经锁定互斥量，或者其他线程同时申请互斥量，但没有竞争到互斥量，那么pthread_lock调用会进入阻塞（执行被挂起）
    等待互斥量解锁再去申请竞争锁


    2.加锁
    int pthread_mutex_lock(pthread_mutex_t *mutex)
    参数
    mutex：要加的锁
    成功返回0，失败返回错误码

    3.解锁
    int pthread_mutex_unlock(pthread_mutex_t *mutex)
    参数
    mutex：要解的锁
    成功返回0，失败返回错误码

    4.销毁互斥量
    int pthread_mutex_destroy(pthread_mutex_t *  mutex)
    mutex：要销毁的锁


    注意：使用静态分配的不需要销毁
    * 不要销毁一个已经加锁的互斥量
    * 已经销毁的互斥量，要确保后面的线程不会再去尝试加锁
    * 加锁的力度要小    

*/


//改进抢票模型

/*

pthread_mutex_t mutex;//创建锁变量
int ticket=100;


void* getticket(void* arg)
{
    long id=(long)arg;
    while(1)
    {
        usleep(1000);//一个线程一进来就等待
        //加锁临界区只能运行一个线程在里面操作,不允许其他线程进入
        pthread_mutex_lock(&mutex);
        if(ticket>0)
        {
            //有票了
            ticket--;
            printf("thread %ld get a ticket ,the number of reamaining is %ld\n",id,ticket);
            //一个线程执行完了它的操作就要解锁，让别的线程进来操作
            pthread_mutex_unlock(&mutex);

        }
        else
        {
            //如果在这里直接break的话，这个临界区就一直都是被锁的，所以要先解锁
            pthread_mutex_unlock(&mutex);
            break;
        }
    }

}
int main()
{
    pthread_t t[5];
    //初始化锁
    pthread_mutex_init(&mutex,NULL);
    long i=0;
    //创建5个线程
    for(i=0;i<5;i++)
    {
        pthread_create(&t[i],NULL,getticket,(void*)(i+1));//这里不保证哪一个线程先执行
    }
    //释放5个线程
    for(i=0;i<5;i++)
    {
        pthread_join(t[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}



*/

/*
    锁的作用：可以对临界区进行保护，所有的执行流程都要执行这个操作
    （lock  ____>访问临界区------》unlock）

    注意：
    1.所有的线程必须要看到同一把锁，所以锁自己本省就是一个临界资源，
    所以锁保证本省安全申请的情况下不能出现中间态，保证原子性，
    2.任一线程持有锁后，其他线程如果还想申请锁，是申请不到的，要保存互斥性

    * 线程申请不到锁：
    会进入等待队列去等待，从运行队列中转移到等待队列之中，持有锁的线程unlock之后，就会唤醒等待队列的第一个线程

    * 加锁了之后，代码执行效率一般会下降
    原本并发或者并行的执行流变成了串行，一个执行完之后再等下一个，这样OS操作效率会变低
    happened before


*/

/*
    互斥量的原理
    大多数的体系结构中都提供了swap或者exchange指令，该指令的作用是把寄存器和内存单元数据进行交换
    由于只有一条指令，保证了原子性，即使是多处理器平台，访问内存的总线周期也有先后，一个处理器上的交换指令执行时，另一个处理器的交换指令只能等待总线周期

*/


/*
    线程安全和可重入
    1.线程安全：多个线程并发同一份代码的时候，不会出现不同的结果
    在对全局变量或者静态变量操作，并且没有锁保护之下就很容易出现这个问题

    2.可重入
    ：同一个函数在被不同的执行流调用，当前的一个执行流还没有执行完，就有其他的执行流再次进入，我们就称重入
    一个函数在重入的情况下，运行结果不会出现任何问题或者运行结果不会有任何区别，这个函数就叫做可重入函数


    # 常见的线程安全情况
    * 每个线程对全局变量或者静态变量，只有读权限而没有写权限的时候，这个线程就是安全的
    * 类或者接口对于线程来说都是原子操作（要么不做，要么做完，没有中间态）
    * 多个线程之间切换不会导致该接口的执行结果出现二义性（对于变量的模糊不清）

    
    # 常见的线程不安全的情况
    * 不保护共享变量的函数
    * 函数状态随着被调用，状态发生了改变的函数
    * 返回指向静态变量指针的函数
    * 调用线程不安全函数的函数
    * 
    * 

    # 常见的可重入的情况
    * 不使用全局变量和静态变量（被共享）
    * 不使用malloc和new开辟出来的空间（）,
    * 不调用不可重入的函数
    * 不返回静态或者全局数据，所有数据都有函数的调用者提供
    * 不使用本地数据，或者通过制作全局数据的本地拷贝来保护全局数据 
     
     # 常见不可重入的情况
     * 使用malloc或free  因为malloc是用全局链来管理堆的，链表问题
     * 调用表尊IO，
     * 可重入函数中使用静态的数据结构
      
     # 区别和联系
     * 区别
        * 函数可重入，线程安全
        * 函数不可重入，就不能让多个线程使用，会引起线程安全问题
        * 如果一个函数中有全局变量，那么这个函数既不是线程安全，也不可重入
        * 
     * 联系
        * 可重入函数是线程安全函数的一部分
        * 线程安全不一定是可重入的（不一定发生线程安全问题），可重入函数一定是线程安全的
        * 如果堆临界资源的访问加上锁，那这个函数一定是线程安全的，但如果这个重入函数锁没有被释放，就会产生死锁
*/


/*
    只有一个执行流，一把锁的时候----会死锁


    死锁：概念：死锁是两个或者两个以上的进程在执行过程中，由于竞争资源或者由于彼此间通信而造成的一种堵塞现象。
    若无外力作用，它将无法推进下去，此时称系统处于死锁状态，互相等待的进程叫做死锁进程
    占有了锁资源不释放，同时还伸手要别人的资源，这样就会导致死锁问题

    死锁产生的条件
    * 互斥条件，一个资源每次只能被一个执行流使用，
    * 请求和保持条件：一个执行流因申请资源而阻塞时，对以获得的资源保持不放，我不放下我的，同时我还要你的
    * 不剥夺条件：一个执行流以获得的资源，在没有使用完之前，不能强行剥夺
    * 循环等待条件：若干个执行流之间形成一种头尾相接的循环等待资源的关系、
    * 
    
    
    * 避免死锁：
    * 破坏死锁的条件（1.不使用锁 2. 让一个执行流放开资源  3.让两个执行流剥夺两个执行流的资源   4.调整申请资源的顺序）
    * 假设顺序要一致
    * 避免锁未释放的场景
    * 资源一次性分配
     
     
    
    # 避免死锁：
    * 银行家算法：为了防止银行家资金无法周转而倒闭，对每一次的贷款，必须考察是否能够限期归还。OS中研究资源分配策略
    也有类似的问题，系统中有限的资源要给很多个进程使用，必须保证资源的进程能够在有限的时间内归还，可以让其他资源使用


*/

//死锁检测法
//线程1拿着锁1运行5秒后申请锁2，线程2拿着锁2 运行5秒后申请锁1，观察现象



/*
pthread_mutex_t mutex1;//创建锁变量1
pthread_mutex_t mutex2;//创建锁变量2

int ticket=100;

void* thread_run1(void* arg)
{
    char* name=(char*)arg;
    //锁上
    pthread_mutex_lock(&mutex1);
    int count=0;
    while(1)
    {
        if(count++==5)
        {
            printf("%s is requesting a lock..\n",name);
            //申请第2个锁
            //pthread_mutex_lock(&mutex2);//申请完了之后不放开
        }
        printf("%s is running ..\n",name);
        sleep(1);
    }
    pthread_mutex_unlock(&mutex1);
}

void* thread_run2(void* arg)
{
    char* name=(char*)arg;
    //锁上
    pthread_mutex_lock(&mutex2);
    int count=0;
    while(1)
    {
        if(count++==5)
        {
            printf("%s is requesting a lock..\n",name);
            //申请第2个锁
            //pthread_mutex_lock(&mutex1);//申请完了之后不放开
        }
        printf("%s is running ..\n",name);
        sleep(1);
    }
    pthread_mutex_unlock(&mutex2);
}

int main()
{
    pthread_t t1,t2;
    //初始化锁
    pthread_mutex_init(&mutex1,NULL);
    pthread_mutex_init(&mutex2,NULL);
    //创建两个线程
    pthread_create(&t1,NULL,thread_run1,(void*)"thread 1");
    pthread_create(&t2,NULL,thread_run2,(void*)"thread 2");

    //创建完之后就要进行等待
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    //锁用完就要销毁
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}


//发现两个线程都想申请对方的资源，但是自己都不把自己手上的东西放开就造成了死锁，请求与保持条件：一个执行流因请求资源而阻塞时，对已获得的资源保持不放



*/

/*
    条件变量
    概念：利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：
    1.一个线程等待条件变量的条件成立而挂起，另一个线程使条件成立，条件变量总是和互斥锁一起使用

    同步：在保证数据安全的前提下，让线程能够按某种特定的顺序访问临界资源，避免饥饿问题，叫做同步
    让我们访问临界资源具有合理性，

    竞态同步：因为时序问题，而导致程序异常，我们称之为竞态条件，旨在描述一个系统或者进程的输出依赖于不受控制的事件，或者顺序或者出现的时机

    # 线程同步可以让每个线程都能够访问临界资源，多个线程高效的完成某些任务

    # 条件变量与互斥锁的结合
    条件变量是包含一个等待队列的，多个线程可以去竞争一把锁，没有得到锁的线程会被继续挂起等待，当拥有锁的线程在条件满足的情况之下，会先释放锁资源
    然后进入到条件变量的等待队列之中，去等待（其他线程被唤醒），这样其他的线程就可以获得到锁资源，如果此时唤醒的条件变量满足，该线程可以去唤醒等待队列中的第一个线程
    自己释放锁资源，然后让第一个线程重新拥有锁资源，依次如此，多个线程就可以顺序的执行工作了，这样就可以实现线程同步了

    

*/


/*
    类型pthread_cond_t 
    #条件变量的初始化
    1.静态创建： PTHREAD_COND_INITIALIZER
    2.动态创建
        int pthread_cond_init(pthread_cond_t * restrict cond,const pthread_condattr_t* restrict attr)
    *第一个要初始化的条件变量，第二个可以NULL


    # 条件变量的销毁
    int pthread_cond_destroy(pthread_t* cond)
    要销毁的条件变量

    等待条件变量满足
    pthread_cond_wait (pthread_t * restrict cond,pthread_mutex_t * restrict mutex)
    第一个在这个条件之下等待
    第二个是互斥量

    使用互斥量的原因：
    一个线程进入等待队列中还不释放锁资源，这样其他线程也没有办法得到锁资源，这样唤醒条件永远也不会满足，这个线程就会一直等待下去，
    所以一个线程进入等待队列中要释放自己手里的资源才可以实现同步 


    # 唤醒的条件变量满足

    int pthread_cond_broadcast(pthread_cond_t *cond)//唤醒所有线程，
    int pthread_cond_signal(pthread_cond_t *cond)//唤醒这个等待队列当中的第一个线程

    第一个会带来惊群效应，不建议使用

*/
//创建5个进程，4个进程执行run1，上来就在条件变量下等待，另一个线程执行run2，然后无脑唤醒等待队列下的线程




pthread_cond_t cond;//条件变量
pthread_mutex_t mutex;//锁


void* threadrun1(void* arg)
{
    char* name=(char*)arg;
    while(1)
    {
        //上锁
        pthread_mutex_lock(&mutex);

        pthread_cond_wait(&cond,&mutex);//挂起释放锁，当函数返回的时候，进入临界区
        printf("%s is waked up...\n",name);
        sleep(1);
        //解锁
        pthread_mutex_unlock(&mutex);
    }
}


void* threadrun2(void* arg)
{
    char* name=(char*)arg;
    while(1)
    {
        sleep(2);
        //唤醒第一个等待队列中的线程,上面那些线程，这个线程只是用来呼唤的
        //pthread_cond_signal(&cond);//会发现一个一个按顺序的唤醒
        pthread_cond_broadcast(&cond);//群起序列就已经乱了
        printf("%s is wakeing up...\n",name);
    }
}


int main()
{
    pthread_t pthread1 ,pthread2,pthread3,pthread4,pthread5;
    //初始化条件变量,和锁
    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mutex,NULL);
    
    pthread_create(&pthread1,NULL,threadrun1,(void*)"pthread 1");
    pthread_create(&pthread2,NULL,threadrun1,(void*)"pthread 2");
    pthread_create(&pthread3,NULL,threadrun1,(void*)"pthread 3");
    pthread_create(&pthread4,NULL,threadrun1,(void*)"pthread 4");
    pthread_create(&pthread5,NULL,threadrun2,(void*)"pthread 5");
    pthread_join(pthread1,NULL);
    pthread_join(pthread2,NULL);
    pthread_join(pthread3,NULL);
    pthread_join(pthread4,NULL);
    pthread_join(pthread5,NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}