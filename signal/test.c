#include <unistd.h>
#include<stdio.h>
#include<signal.h>

void handler(int signo)//函数名就是我们目标函数的地址
{
  printf("get a signal: signo:%d\n,pid:%d\n",signo,getpid());
}
int main()
{
  //我们可以通过signal注册对2号信号（终止）处理动作，改成我们自定义的动作
  signal(2,handler);//我们对2号信号进行处理
  //注册的时候，并不会调用这个函数，只有当信号到来的时候，这个函数才会被调用
  while(1)
  {
    printf("hello world\n,pid: %d\n",getpid());
    sleep(1);
  }
  return 0;
}
