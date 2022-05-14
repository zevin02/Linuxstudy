#include<stdio.h>
#include<signal.h>
volatile int flag=0;//内存里面读取flag，来检测flag


void handler(int signo)
{
    flag=1;
    printf("change flag 0 to 1\n");
}
int main()
{
    signal(2,handler);
    while(!flag);//发送2号信号之后就会停止循环
    printf("这个进程是正常退出的\n");
    return 0;
}