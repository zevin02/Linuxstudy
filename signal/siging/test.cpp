#include<unistd.h>
#include<iostream>

#include<signal.h>
using namespace std;



void showpend(sigset_t& pending)
{
    //检测一下信号是否存在在里面
    int i=0;
    for(i=1;i<32;i++)
    {
        if(sigismember(&pending,i))//成功放回1，失败放回0，
        {
            //检查信号是否存在pending位图里面
            cout<<"1";
        }
        else
        {
            cout<<"0";
        }
    }
    cout<<endl;
}

void handler(int signo)
{
    cout<<"2号信号被递达了，已经完成了处理 "<<signo<<endl;
}
int main()
{
    //虽然sigset_t  是一个位图结构，但是不同的OS 实现是不一样的，不能让用户使用直接修改该变量，
    //需要使用特定的函数，

    //这里的set是一个变量，所以这个set在哪里保存着呢
    //set是一个变量，也是在栈空间保存着
    //用户栈上保存（地址空间）  


    signal(2,handler);//捕捉一下2号信号  
    sigset_t iset,oset;//有了这个类型
    // set | =1;
    //外面必须使用set对应的接口函数
    sigemptyset(&iset);//清空
    sigemptyset(&oset);

    //外面想对2，5，6三个信号做屏蔽
    sigaddset(&iset,2);//把2号信号添加进去
    // sigaddset(&iset,9);//把2号信号添加进去,9号信号无法被屏蔽掉的

    //设置当前信号的屏蔽字
    //获取当前进程老的屏蔽字
    sigprocmask(SIG_SETMASK,&iset,&oset);//把2号信号给屏蔽掉了
    // sigprocmask(SIG_UNBLOCK,&iset,&oset);//把2号信号给解除屏蔽了
    // sigprocmask(SIG_BLOCK,&iset,&oset);//把2号信号给屏蔽掉了
    //ctrl c 给屏蔽掉了，不会被递达的
    sigset_t pending;
    int cnt=0;
    while(1)
    {
        
        sigemptyset(&pending);//把他里面的数据都给他清空一下
        sleep(1);
        sigpending(&pending);//获取了pending位图
        showpend(pending);
        cout<<"hello"<<endl;
        cnt++;
        if(cnt==10)
        {
            //把2号信号解除屏蔽
            //老的屏蔽字没有对2号进行屏蔽
            //在这之前2号信号都已经被屏蔽掉了，所以前面用ctrl c是没什么用的
            sigprocmask(SIG_SETMASK,&oset,NULL);//在这里之后才可以有用的
            cout<<"恢复对2号屏蔽字的使用,可以被递达了"<<endl;
            //被递达了之后，又会,我们无法看到恢复屏蔽，2号信号的默认动作是终止进程，所以看不到现象
        }
    }

    return 0;
}