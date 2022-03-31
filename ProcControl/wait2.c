
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
  }
  int status=0;
 
  //pid_t ret=waitpid(id,&status,0);//阻塞等待
  while(1)//非阻塞的轮询方案
  {
  pid_t ret=waitpid(id,&status,WNOHANG);//非阻塞等待
  if(ret==0){
   
    //检测子进程没有退出，但是waitpid等待是成功的，需要父进程继续进行等
    printf("do father thing\n");//得知还没好，就可以做一下自己的事情
    
  }
  else if(ret>0)
  {

    //子进程退出了，waitpid也成功了，获取到了对应的结果
    printf("exit code: %d\n,",WEXITSTATUS(status));
    break;
  }
  else 
  {
    //ret<0
    //等待失败
    perror("waitpid");
    break;
  }
  sleep(1);
  }
  return 0;

}
