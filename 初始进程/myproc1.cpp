#include <iostream>
using namespace  std;
#include<unistd.h>
#include <sys/types.h>
int main()
{
    pid_t id=fork();//创建子进程
    cout<<"hello proc "<<getpid()<< " hello parent "<<getppid()<<" ret: "<< id <<endl;
    sleep(1);
    return 0;
}
