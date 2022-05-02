#include<stdio.h>
#include<sys/types.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
void usage(const char* proc)
{
  printf("Usage:\n \t,%s,sign who\n",proc);
}

int main(int argc,char*argv[])

{

  while(1)
  {
    printf("1");
    sleep(3);
    raise(8);//给自己发一个8号信号
  }

  //if(argc!=3)
  //{
  //  usage(argv[0]);
  //  return 1;
  //}
  //int signo=atoi(argv[1]);
  //int who=atoi(argv[2]);
  //kill(who,signo);
  //printf("%d %d",signo,who);
  return 0;
}
