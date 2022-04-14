#include "myshell.h"

void CommandAnalys(char *argv[], int size)
// argv{"touch ","a","NULL"}
{

    //父进程执行的命令
    if (size != 0 && strcmp(argv[0], "cd") == 0)
    {
        Do_cd(argv[0], argv);
    }
    else if (size != 0 && strcmp(argv[0], "exit") == 0)
    {
        DoProcExit(); //进程退出
    }
    else if (size != 0 && strcmp(argv[0], "history") == 0)
    {
        ShowHistory();
    }
    // 5.执行第3方命令
    //子进程来执行的命令
    else
    {
        int flag = 0;
        for (int i = 0; i < size; i++)
        {
            if (strcmp(argv[i], ">") == 0)
            {
                if (strcmp(argv[0], "ls") == 0)
                {
                    size--;
                }
                DoRedefDir(argv, size, i, argv[i]);
                flag = 1;
                break;
            }
            if (strcmp(argv[i], ">>") == 0)
            {
                if (strcmp(argv[0], "ls") == 0)
                {
                    size--;
                }
                DoRedefDir(argv, size, i, argv[i]);
                flag = 1;
                break;
            }
            if (strcmp(argv[i], "<") == 0)
            {
                if (strcmp(argv[0], "ls") == 0)
                {
                    size--;
                }
                DoInputRedef(argv, size, i, argv[i]);
                flag = 1;
                break;
            }
            if(strcmp(argv[i],"|")==0)
            {
                DoPipe(argv,size);
                flag=1;
                break;
            }
        }
        if(strcmp(argv[size-1],"&")==0)
        {
            //ls -a -l & -auto 
            argv[size-1]='\0';
            size--;
            DoBackRun(argv,size);
            return;
        }

        if (flag)
        {
            return;
        }
        else
        {
            pid_t id = fork();
            if (id == 0) //这里我们touch东西，cd的话都是子进程，但是我们需要让父进程去进行这些操作
            {
                
                execvp(argv[0], argv);
                exit(1);
            }
            waitpid(id, NULL, 0);
        }
    }
}



void DoBackRun(char* argv[],int size)
{
    //执行后台命令
    pid_t id=fork();
    if(id<0)
    {
        perror("fork");
    }
    if(id==0)
    {
        //child
        freopen("/dev/null", "w", stdout); 
        freopen("/dev/null", "r", stdin);
        signal(SIGCHLD, SIG_IGN);
        CommandAnalys(argv,size);
        execvp(argv[0], argv);
        perror("execvp");
    }
    else
    {
        exit(0);
    }
}
void ShowHistory()
{
    int i = 3;
    HIST_ENTRY **his;
    his = history_list();
    printf(" 1 1\n");
    printf(" 2 2\n");
    while (his[i] != NULL)
    {
        printf(" %d %s\n", i, his[i]->line);
        i++;
    }
}

//进程退出
void DoProcExit()
{
    exit(0);
}

//输入重定向
void DoInputRedef(char *argv[], int size, int leftpos, char *command)
{
    // cat < log.txt
    //先fork子进程，把0改掉，用log.txt的fd，读书log.txt里面内容
    int fd = open(argv[size - 1], O_RDONLY);
    int oldfd = dup(0);
    char *tmp[leftpos + 1];
    dup2(fd, 0);
    for (int i = 0; i < leftpos; i++)
    {
        tmp[i] = argv[i];
    }
    tmp[leftpos] = NULL;
    if (fork() == 0)
    {
        char line[128];

        // while (fgets(line, sizeof(line) - 1, stdin))
        // {
        //     printf("%s\n", line);
        // }
        execvp(argv[0], tmp);
    }
    waitpid(-1, NULL, 0);
    dup2(oldfd, 0);
    close(fd);
}

//输出重定向与追加重定向
void DoRedefDir(char *argv[], int size, int youpos, char *command)
{
    // echo "hello"> file
    // fork->child -> dup2(fd,1)->exec
    //>会清空原来的内容，还会创建新文件，以写的方式打开
    int fd;
    if (strcmp(command, ">") == 0)
    {
        fd = open(argv[size - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }
    if (strcmp(command, ">>") == 0)
    {
        fd = open(argv[size - 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
    }
    if (fd < 0)
    {
        perror("open");
        exit(-1);
    }
    int oldfd;
    oldfd = dup(1);
    dup2(fd, 1);
    char *tmp[youpos + 1];
    for (int i = 0; i < youpos; i++)
    {
        tmp[i] = argv[i];
    }
    tmp[youpos] = NULL;
    if (fork() == 0)
    {
        execvp(argv[0], tmp);
    }

    waitpid(-1, NULL, 0);
    dup2(oldfd, 1);
    close(fd);
}

void Do_cd(char *filename, char *argv[])
{
    if (argv[1])
    {
        if (strcmp(argv[1], "~") == 0)
        {
            chdir("/home/xvzewen");
        }
        else
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
    signal(SIGHUP, SIG_IGN);
}
int IsFarDo(char *filename)
{
    if (strcmp(filename, "cd") == 0 || strcmp(filename, ">") == 0 || strcmp(filename, ">>") == 0 || strcmp(filename, "<") == 0)
        return 1;
    return 0;
}


void DoPipe(char*argv[],int size)//
{
 
    char*str1[100]={NULL};
    char*str2[100]={NULL};
    int i=0;
    int flag=0;
    int p=0;
    while(argv[i]!=NULL)
    {
      if(strcmp(argv[i],"|")==0)
      {
        i++;
        flag=1;
        str1[p]="--color=auto";
        p=0;
      }
      else if(flag==0)
      {
        str1[p++]=argv[i++];
      }
      else if(flag==1)
      {
        str2[p++]=argv[i++];
      }
    }
     //str2[p]="--color=auto";

    pid_t pid=fork();
    if(pid<0)
    {
      perror("fork");
      exit;
    }
    else if(pid==0)//子进程
    {
      int ret;
      int fd[2];//存放文件句柄pipe
      ret=pipe(fd);//建立管道

      pid_t pid1=fork();
      if(pid<0)
      {
        perror("fork");
        exit;
      }
      else if(pid1>0)//子进程
      {
        close(fd[0]);//关闭读端
        dup2(fd[1],1);
        execvp(str1[0],str1);     
      }
      else if(pid1==0)
      {
        int fdin=dup(0);//保存标准输入
        close(fd[1]);//关闭写端
        dup2(fd[0],0);
        execvp(str2[0],str2); 
      }
    }
    else if(pid>0)
    {
      wait(NULL);
    }
}
