#include "myshell.h"

void CommandAnalys(char *argv[],int size)
{
    if (IsFarDo(argv[0]))
    {
        DoFarProcess(argv[0], argv);

    }
    // 5.执行第3方命令
    int flag=0;
    for(int i=0;i<size;i++)
    {
        if(strcmp(argv[i],">")==0)
        {
            DoRedefDir(argv,size,i);
            flag=1;
            break;
        }
    }
    if(flag)
    {
        return;
    }
    if (fork() == 0) //这里我们touch东西，cd的话都是子进程，但是我们需要让父进程去进行这些操作
    {
        // fork执行的是第三方命令，独立的命令
        //  if(argv[0]=="|")
        //  {
        //    Do_pipe(argv);
        //  }
        execvp(argv[0], argv);
        exit(1);
    }
}


void DoRedefDir(char*argv[],int size,int youpos)
{
    //echo "hello"> file
    //fork->child -> dup2(fd,1)->exec
    //>会清空原来的内容，还会创建新文件，以写的方式打开
    int fd=open(argv[size-1],O_WRONLY|O_CREAT|O_TRUNC);

    int oldfd=dup(1);
    dup2(fd,1);
    int i=0;
    char*tmp[youpos];
    for(int i=0;i<youpos-1;i++)
    {
        tmp[i]=argv[i];
    }
    tmp[youpos-1]=NULL;

    if(fork()==0)
    {
        execvp(argv[0],tmp);
    }
    waitpid(-1,NULL,0);

}
void DoFarProcess(char *filename, char *argv[])
{

    if (strcmp(filename, "cd") == 0)
    {
        //内建命令
        Do_cd(filename, argv);
    }
}

void Do_cd(char *filename, char *argv[])
{
    if (argv[1])
    {
        chdir(argv[1]);
    }
    else
    {
        chdir("/home/xvzewen");
    }
}

void my_signal()
{
    signal(SIGINT, SIG_IGN);
}
int IsFarDo(char*filename)
{
    if(strcmp(filename,"cd")==0||strcmp(filename,">")==0||strcmp(filename,">>")==0||strcmp(filename,"<")==0)
    return 1;
    return 0;
}