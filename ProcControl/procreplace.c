#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
  if(fork()==0)
  {
    //child
    //exec
   execl("/usr/bin/ls","ls -al",NULL);//第2个参数就是我们在命令行上怎么传递就怎么在第二个参数里面怎么写，以NULL 结尾表示程序跑完了 
   _exit(1);//返回的话就出错了
  }
  //parent
  waitpid(-1,NULL,0);
  printf("wait success\n");










//  pid_t id=fork();
//  if(id==0)
//  {
//
//    printf("I am a process, pid:%d \n",getpid());
//    printf("hahahaha\n");
//    //when the program of process is execuated successfully,the later code won't be done
//    //which means exec*function needn't check the return value
//    //exec* function return -1 for its failure call certainly
//    execl("/usr/bin/ls","-a","-l",NULL);//execl执行程序替换，后面的代码不执行了，执行ls命令的代码
//   // execl("/usr/bin/top","top",NULL);
//    printf("hahahaha\n");
//    
//    printf("hahahaha\n");
//    printf("hahahaha\n");
//    
//  }
//  //让子进程去执行程序替换，让父进程去执行自己的事情
//  while(1)
//  {
//    printf("i am father\n");
//    sleep(1);
//  }
//
//  waitpid(id,NULL,0);
//  printf("wait success\n");
  return 0;
}
