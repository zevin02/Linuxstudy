#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>

int main()
{
  if(fork()==0)
  {
  printf("command begin\n");
  //l以列表的形式进行传递参数
  //execl("/usr/bin/ls","ls","-a","-l",NULL);//以列表的形式进行传参
//  char* argv[]={"ls","-a","-l","-i",NULL};
 // //v是以数组的方式
 // execv("/usr/bin/ls",argv);//打包在数组里面传进去,和之前的没什么区别，只不过变成了用数组来传参数
 // //有p的话直接写要执行的程序名，不需要路径，会在环境变量path中搜索
 // execlp("ls","ls","-a",NULL); 
  //execvp,同理
  //execvp("ls",argv);
  //execl("./myexe","myexe",NULL);
  char*env[]={"myenv=hahah","ds=ads",NULL};
  execle("./myexe","myexe",NULL,env);//自己的环境变量
  }
  waitpid(-1,NULL,0);
  printf("wait success\n");
  //exit(-1);
  return 0;
}
