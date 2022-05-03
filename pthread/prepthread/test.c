#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/*

fork是对父进程代码和数据进行写实拷贝，在没有用的时候，子进程和父进程数据一样，在用的时候，会进行拷贝，确保两者数据不相同


vfork
1.会与父进程共享数据段
2.会保证子进程先运行，在他调用完exec（进程替换）或exit（进程退出）之后，父进程才会进行调度
所以如果子进程没有exec或exit的话，程序会导致死锁，程序是有问题的
*/


/*

int main()
{
    int val = 100;
    pid_t id = vfork();
    if (id == 0)
    {
        //子进程先被运行

        // child

        int cnt = 0;
        while (1)
        {
            printf("i am child pid :%d,val :%d,&val :%p\n", getpid(), val, &val);
            cnt++;
            sleep(1);
            if (cnt == 2)
            {
                val = 200;
            }
            if (cnt == 5)
            {
               // exit(0); //调用了exit这样才可以使用父进程，不然的话就会一直停在这里，一直执行子进程
            }
        }
    }
    else if (id > 0)
    {
        //父进程
        int cnt = 0;
        while (1)
        {
            printf("i am father pid :%d,val :%d,&val :%p\n", getpid(), val, &val);
            cnt++;
            sleep(1);
            if(cnt==3)
            {
                val=300;
            }
        }
    }
    return 0;
}

*/