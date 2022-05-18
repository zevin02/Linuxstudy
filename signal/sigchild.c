#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

void handler(int signo)
{
    printf("get a signo :%d ",signo);
}
int main()
{
    signal(SIGCHLD,handler);//子进程退出就发送一个退出信号
    pid_t id=fork();
    if(id==0)
    {
        int cnt=5;
        while(cnt--)
        {
            printf("我是子进程:%d \n",getpid());
            sleep(1);
        }
        exit(0);
    }
    waitpid(-1,NULL,0);//父进程要等待wait才会有那个信号
    // while(1);
    return 0;
}