#include<unistd.h>
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

int count=0;
//统计一下1s，我们的server可以对int递增到多少

void HandAlarm(int signo)
{
  printf("%d\n",count);
  exit(1);
}
int main()
{
  signal(SIGALRM,HandAlarm);//我们对14号信号进行注册一下
  //设置闹钟
  alarm(1);//我们没有设置alarm信号的捕捉动作（没有自定义调用signal函数），执行默认动作
  while(1)
  {
    count++;
    //printf("hello %d\n",count++);
    //为何在这里这么慢呢
    //+了printfIO操作，与硬件进行交互，还有远端的操作，所以效率差别非常的大
    //所以程序中出现大量的IO的话，要考虑程序的效率
  }
  return 0;
}
