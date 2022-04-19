#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
int main()
{
    //pipe这个函数可以创建并打开管道，
    //读端和写端
    //pipe(int pipefd[2])
    //参数；fd[0],读端，fd[1]写端
    //父子进程谁是读端还是写端都是可以的，
    //成功返回0,创建并且打开管道成功，失败返回-1,
    int ret;
    int fd[2];
    ret=pipe(fd);
    char buf[1024];
    char* str="hello pipe\n";
    if(ret==-1)
    {
        perror("pipe");
        exit(1);
    }
    //完成实现管道
    pid_t id=fork();
    if(id>0)
    {
        //父进程执行写入操作
        //parent
        close(fd[0]);//关闭读端
        write(fd[1],str,strlen(str));//父进程往管道写入了hello world，
        sleep(1);//为了避免父进程先死掉，子进程变成孤儿进程
        close(fd[1]);
    }
    else if(id==0)
    {
        //child，执行读
        close(fd[1]);//关闭写端
        ssize_t n=read(fd[0],buf,sizeof(buf));//读fd[0]里面的内容
        buf[n]=0;
        write(1,buf,n);
        close(fd[0]);

    }
    else
    {
        perror("fork");
    }
    return 0;
}