#include"head.h"

int  main()
{
  umask(0);//把掩码设置为1
  if(mkfifo(MY_FIFO,0666)<0)
  {
    perror("mkfifo");
    return 2;
  }
  //对于管道文件，我们只需要进行文件操作即可
  int fd=open(MY_FIFO,O_RDONLY,0666);
  //我们让他执行读操作
  if(fd<0)
  {
    perror("open");
    return 1;
  }
  //执行业务逻辑
  while(1)
  {
    char buffer[64]={0};
    //我们每个5秒才从缓冲区里面读进来
    sleep(5);
    ssize_t s=read(fd,buffer,sizeof(buffer));//我们对fd文件进行读取,client通过管道对fd这个文件进行写入，我们在通过这个进行读取
    if(s>0)
    {
      //读取结束了，buffer里面就是我们需要的字符串,我们用client去控制server让他去进行操作
      if(strcmp(buffer,"show")==0)
      {
        if(fork()==0)
        {
          //子进程
          execlp("ls","ls","-a",NULL);//程序替换
          exit(1);//如果执行失败我们就可以把退出码设置为1
        }
        waitpid(-1,NULL,0);//父进程执行等待
      }
      if(strcmp(buffer,"sl")==0)
      {
        if(fork()==0)
        {
          //子进程
          execlp("sl","sl",NULL);//程序替换
          exit(1);//如果执行失败我们就可以把退出码设置为1
        }
        waitpid(-1,NULL,0);//父进程执行等待
      }
      buffer[s]=0;
      printf("client# %s\n",buffer);
    }
    else if(s==0)//我们发现client的结束的时候，文件读写结束了，就会被不断的刷屏了
    {
      printf("client quit....\n");
    }
    else 
    {
      perror("read");
      break;
    }
  }
  close(fd);
  return 0;
}
