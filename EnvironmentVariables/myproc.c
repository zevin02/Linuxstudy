#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc ,char* argv[],char *env[])//可以获得环境变量的字符串数组，和argv相类似
  //argv是一个字符串数组，有argc个变量
{
  printf("path: %s ",getenv("PATH"));//
  int j=0;
  for(j=0;env[j];j++)//这个是系统自动给我们添加的，所以不带数字的参数
  {
    printf("%s ",env[j]);//因为最后一个数据是NULL
  }
  int i=0;
  for(i=0;i<argc;i++)
  {
    printf("argv[%d]=%s\n",i,argv[i]);
  }
//  int cnt=5;
//  while(cnt--)
//  {
//   // printf("I am a process ,pid %d,ppid %d\n",getpid(),getppid());
//    sleep(1);
//    printf("I am a cmd -->process\n");
//
//  }
  return 0;
}
