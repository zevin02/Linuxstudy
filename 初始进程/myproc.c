#include<stdio.h>
#include<unistd.h>//系统调用接口
#include<sys/types.h>


//pcb
//1。标识符pid
//

void fork_test()
{
int ret=fork();//fork可以创建子进程
while(1)
{
    printf("i am a process %d,%d\n",getpid(),getppid());
    sleep(1);
}

}

void pcb()
{
    
//     while(1)
//    {
//        //printf("hello \n");
//        printf("hello pid= %d,ppid=%d\n",getpid(),getppid());
//        sleep(1);
}
int main()
{
    fork_test();
    fork();
    printf("hello");


//    }
    return 0;//退出码，
}
