#include <unistd.h>
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
void handler(int signo)//函数名就是我们目标函数的地址
{
  switch(signo)
  {
    case 2:
      printf("hello bite %d",signo);
      break;
    case 3:
      printf("hello world %d",signo);
      break;
    case 9:
      printf("quit");
      break;
  }
  printf("get a signal: signo:%d\n,pid:%d\n",signo,getpid());
  exit(1);
}

void signaldemo()
{
  int sig=1;
  for(sig=1;sig<32;sig++)
  {
    signal(sig,handler);
  }
}


void sigtest()
{
  while(1)
  {
   // int *p=NULL;
   // p=(int*)100;
   // *p=100;//对指针解引用，指向空间为NULL，我们是不可以写入的,进程的崩溃
  int a=10;
  a/=0;

  }
}
int main()
{
  //将所有的信号都进行捕捉
  //我们可以通过signal注册对2号信号（终止）处理动作，改成我们自定义的动作
  signal(2,handler);//我们对2号信号进行处理
  //注册的时候，并不会调用这个函数，只有当信号到来的时候，这个函数才会被调用
  signaldemo();
  sigtest();
  //while(1)
  //{
  //  printf("hello world\n,pid: %d\n",getpid());
  //  sleep(1);
  //}
  return 0;
}
