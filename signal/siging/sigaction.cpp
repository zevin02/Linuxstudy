#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<cstring>
using namespace std;



void handler(int signo)
{
    //信号捕捉动作
     while(1)
    cout<<"signo="<<signo<<endl;
}

int main()
{
    struct sigaction act;//定义了一个sigaction类型
    memset(&act,0,sizeof(act));
    act.sa_handler=handler;
    
    //我们再处理2号的时候，想把3，4，5号信号都屏蔽掉
    
    //sa_mask时一个sigset的类型的，所有的操作都需要使用系统调用
    //sa_mask里面说明了需要被额外屏蔽的信号
    
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,3);//在执行sigaction 2 号信号的时候，3，4信号都被屏蔽掉了
    sigaddset(&act.sa_mask,4);



    // act.sa_handler=SIG_IGN;//忽略，或者也可以是设置位defalut
    //act.sa_handler=SIG_DFL;;//忽略，或者也可以是设置位defalut
    sigaction(2,&act,nullptr);//对2号信号进行注册了
    //本质是修改当前进程的handler函数指针数组特定的内容
    
    while(true)
    {
        cout<<"hello "<<endl;
        sleep(1);
    }
    return 0;
}