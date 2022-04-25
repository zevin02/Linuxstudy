#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include <string.h>
#include<sys/wait.h>

int main()
{
  int fd[2];
  if(pipe(fd)!=0)//创建管道
  {
    perror("pipe");
    exit(-1);
  }
  //打印文件描述符
  printf("pipe[0]=%d\n",fd[0]);
  printf("pipe[1]=%d\n",fd[1]);
  //实现进程键通信
  pid_t id =fork();
  if(id<0)
  {
    perror("fork");
    exit(1);
  }
  //我们向让父进程执行读取，子进程执行写入
  //0----->就是读取端，1----->就是写入端
  else if(id==0)//创建子进程
  {
    //child
    //关闭读写端，建立一个单向的管道
    //子进程把文件描述符也都给继承下来了
    close(fd[0]);
    const char* msg="hello world";
    int cout=0;
    //我们还可以使用read 和write进行读写操作
    while(1)
    {

    //write(fd[1],msg,strlen(msg));//我们不需要+1,因为\0本身只是c语言的标准，
    write(fd[1],"a",1);//子进程不断往管道里面写入，但是父进程不读
    cout++;
    printf("cout=%d\n",cout);
    //break;
    //write的动作就是pipe里面只要有缓冲区区域就一直写入
    //当我们一直些的时候，写满64KB的时候，写段就不再写入了，因为管道有大小，
    //当写端写满的时候，为什么不写了，
    //因为要让读端来读，
    }
    //close(fd[1]);//子进程把自己的写段给关闭了
    exit(0);
    //dup2(fd[1],1);
  }
  else 
  {
    //father
    close(fd[1]);
    while(1){
    sleep(10);
    char c[1024*2+1];
    ssize_t s=read(fd[0],c,sizeof(c));
    //读端一直在读取
    if(s>0)
    c[s]=0;
    else{ 
      printf("writer quit\n");
    break;
    }
    printf("father take %c\n",c[0]);

    } //父进程就一直在那里读取
    close(fd[0]);
    int status=0;
    waitpid(-1,&status,0);//我们可以获取子进程退出的原因，
    //status中如何查看子进程是如何结束的呢？？
    printf("exit code:%d\n",WEXITSTATUS(status));//获取退出码
    printf("exit signal:%d\n",WTERMSIG(status));//获取退出信号
    //while(1)
    //{
    //  char buf[32]={0};
    //  ssize_t ret=read(fd[0],buf,sizeof(buf));//如果此时ret=0说明子进程关闭文件描述符了，相当于读到文件的结尾了
    //  //read是只要有数据就可以一直读取
    //  //这就叫字节流
    //  if(ret==0)
    //    break;
    //  else 
    //  {
    //  buf[ret]=0;//把\0位置就设置为0
    //  printf("child say : %s\n",buf);

    //  //我们没有让父进程去sleep
    //  }

    ////dup2(fd[0],0);//绑定打印到文件里面
    //}
    //dup2(fd[0],0);
  }
  return 0;
}
