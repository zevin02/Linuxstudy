#include"head.h"

int  main()
{
  //因为fifo已经创建好了，所以我们不用再创建，直接用就行了
  int fd=open(MY_FIFO,O_WRONLY);
  //把文件打开
  if(fd<0)
  {
    perror("open");
    return 2;
  }
  //业务逻辑，我们再这里进行写入
  while(1)
  {
    printf("请输入： ");//因为没有+\n,为了避免缓冲区的问题
    fflush(stdout);//刷新一下缓冲区

    char buffer[64]={0};
    ssize_t s=read(0,buffer,sizeof(buffer));//我们要处理掉最后的回车键
    //我们从命令行中获取输入，输入到buffer里面
    if(s>0)
    {
      buffer[s]=0;//把最后的\0给吃掉
      buffer[s-1]=0;
      printf("%s\n",buffer);//读一条就发一下文件里面的内容
      write(fd,buffer,strlen(buffer));//传给server  
    }
  }
  close(fd);

  return 0;
}
