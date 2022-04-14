#include "myshell.h"
extern char *cdpath[30];
extern int cdpos;
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
            if (strcmp(argv[i], "|") == 0)
            {
                if (strcmp(argv[0], "ls") == 0)
                {
                    size--;
                }
                DoPipe(argv, size, i);
                flag = 1;
                break;
            }
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
        }
        if (strcmp(argv[size - 1], "&") == 0)
        {
            // ls -a -l & -auto
            if (strcmp(argv[0], "ls") == 0)
            {
                size--;
            }
            DoBackRun(argv, size);
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

void DoBackRun(char *argv[], int size)
{
    //执行后台命令
    pid_t id = fork();
    if (id < 0)
    {
        perror("fork");
    }
    if (id == 0)
    {
        // child
        freopen("/dev/null", "w", stdout);
        freopen("/dev/null", "r", stdin);
        signal(SIGCHLD, SIG_IGN);
        CommandAnalys(argv, size);
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
            // cdpath[cdpos++]="/home/xvzewen";
        }
        // else if(strcmp(argv[1],"-")==0)
        // {
        //     if(cdpos==0)
        //     {
        //       chdir(".");
        //       cdpath[cdpos++]=".";
        //     }
        //     else
        //     {
        //     chdir(cdpath[cdpos-1]);
        //     cdpath[cdpos++]=cdpath[cdpos-2];
        //     }
        // }

        else
            chdir(argv[1]);
        // cdpath[cdpos++]=argv[1];
    }
    else
    {
        chdir("/home/xvzewen");
        // cdpath[cdpos++]="/home/xvzewen";
    }
}

void my_signal()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
}

void DoPipe(char *argv[], int size, int pipepos) //实现管道
{

    char *wstream[pipepos + 1];    //存管道前面的内容
    char *rstream[size - pipepos]; //存管道后面的内容
    int wpos = 0;
    int rpos = 0;
    for (int i = 0; i < size; i++)
    {
        if (i < pipepos)
            wstream[wpos++] = argv[i];
        else if (i > pipepos)
            rstream[rpos++] = argv[i];
    }
    wstream[pipepos] = NULL;
    rstream[size - pipepos - 1] = NULL;
    pid_t id = fork();
   
    if (id == 0)
    {
        int fd[2];
        int ret = pipe(fd);
        pid_t id1 = fork();
        if (id1 > 0)
        {
            //父进程执行写端
            close(fd[0]); //关闭读段
            dup2(fd[1], 1);
            // close(fd[1]);

            int ret = execvp(wstream[0], wstream);
            if (ret == -1)
            {
                printf("fail\n");
            }
            waitpid(id, NULL, 0);
        }
        else if (id1 == 0)
        {
            close(fd[1]);
            dup2(fd[0], 0);
            // close(fd[0]);
            execvp(rstream[0], rstream);
            // exit(0);
        }

    }
    waitpid(id,NULL,0);

  
}


