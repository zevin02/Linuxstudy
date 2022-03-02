#include<stdio.h>
#include<unistd.h>//系统调用接口
#include<sys/types.h>


//pcb
//1。标识符pid
//


int main()
{
while(1)
    {
        //printf("hello \n");
        printf("hello pid= %d,ppid=%d\n",getpid(),getppid());
        sleep(1);
    }
    return 0;//退出码，
}
