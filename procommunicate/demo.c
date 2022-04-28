#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include <string.h>
#include<sys/wait.h>
void wmorer()
{
  //写端比读端快，读端处于阻塞状态
  //读端比写端快，写端处于阻塞状态
  int fd[2];
  int ret=pipe(fd);
  if(ret==-1)
  {
    perror("pipe");
    exit(0);
  }
  pid_t id=fork();
  if(id==-1)
  {
    perror("fork");
    exit(0);
  }
  else if(id==0)
  {
    //child
    //write
    close(fd[0]);
    const char* buf="hello world";
    while(1)
    {
      write(fd[1],buf,sizeof(buf));
      printf("child is sending message\n");
      sleep(5);
    }
  }
  else 
  {
    //father
    close(fd[1]);
    while(1)
    {
     // sleep(1);
      char buffer[32];
      ssize_t s=read(fd[0],buffer,sizeof(buffer));
      if(s>0)
      {
        buffer[s]=0;
        printf("read message :%s\n",buffer);
      }
      else 
      {
        printf("end of file\n");
        exit(0);
      }
    }
  }

}
int main()
{
  wmorer();
  return 0;
}
