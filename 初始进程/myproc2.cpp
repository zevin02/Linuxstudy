#include <iostream>
#include<unistd.h>
using namespace std;
int main()
{
    pid_t id=fork();
    if(id==0)
    {
        //child process
       while(1)
       {
        cout<<"childpid="<<getpid()<<endl;
        sleep(2);

    }
    }
    else if(id>0)
    {
        //father process
    while(1)
    {
        cout<<"fatherpid"<<getppid()<<endl;
        
        sleep(2);
   
    
    }
    }
    else
    {
        cout<<"失败"<<endl;
    }
    sleep(1);
    return 0;
}
