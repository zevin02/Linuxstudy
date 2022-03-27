#include <stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
  pid_t id=fork();
  if(id==0)
  {
    //child
    int cnt=5;
    while(cnt){
      printf("child[%d] is running : cnt is :%d \n",getpid() ,cnt);
      cnt--;
      sleep(1);
      //子进程会持续5秒之后就exit，但是父进程；立刻就退出了
      //子进程就会变成一个孤儿进程，被系统领养
    }
    exit(0);//我们执行完子进程之后直接退出，不执行父进程的代码
    //进程退出
  }
  sleep(10);//先让父进程等待10秒
  //前5秒是正常运行的
  //后5秒子进程处于z状态
  printf("father wait begin\n");
  //因为父进程和子进程是同时执行的
  //parent
  //为了不让子进程变成一个孤儿进程，父进程必须在那里等待
  pid_t ret=wait(NULL);//成功的话就返回其
  if(ret>0)
    printf("father wait %d\n ",ret);
  else 
    printf("father wait fail\n");
  //到这里僵尸状态就没了
  sleep(10);
  //回收完毕之后父进程继续活上10秒
  return 0;
}




