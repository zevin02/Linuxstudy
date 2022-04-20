#include "myshell.h"
char prevpwd[128];
char *curpwd;

void CommandAnalys(char *argv[], int size, char *command)
// argv{"touch ","a","NULL"}
{

    //父进程执行的命令
    if (size != 0 && strcmp(argv[0], "cd") == 0)
    {
        Do_cd(argv);
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
        //判断是否有&
        int backflag = 0;
        for (int i = 0; i < size; i++)
        {

            if (strcmp(argv[i], "&") == 0)
            {
                backflag = 1;
                if (strcmp(argv[0], "ls") == 0)

                {
                    argv[i] = argv[i + 1];
                    argv[i + 1] = 0;
                }
                else
                    argv[i] = 0;
                size--;
            }
        }

        for (int i = 0; i < size; i++)
        {
            if (strcmp(argv[i], "|") == 0)
            {
                if (strcmp(argv[0], "ls") == 0)
                {
                    size--;
                }

                // int ret=callCommandWithPipe(argv, 0, size);

                ; // DoPipe(argv, size);
                // command_with_Pipe(buf);
                DoCommandPipe(argv, size, backflag);
                flag = 1;
                break;
                // exit(ret);
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

        if (flag)
        {
            return;
        }
        else
        {
            pid_t id = fork();
            if (id == 0) //这里我们touch东西，cd的话都是子进程，但是我们需要让父进程去进行这些操作
            {

                int ret = execvp(argv[0], argv);
                if (ret == -1)
                {
                    perror("execvp");
                    exit(-1);
                }
                exit(1);
            }
            if (backflag)
            {
                printf("%d  %s \n", getpid(), command);
                // exit(0);
                return;
            }
            else
                waitpid(id, NULL, 0);
        }
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
        int ret = execvp(argv[0], tmp);
        if (ret == -1)
        {
            perror("execvp");
            exit(-1);
        }
    }

    waitpid(-1, NULL, 0);
    dup2(oldfd, 1);
    close(fd);
}

void Do_cd(char *argv[])
{
    if (argv[1] == NULL) //什么都没有，就执行～
    {
        getcwd(prevpwd, sizeof(prevpwd)); //保存当前的路径，以被-使用
        chdir("/home/xvzewen");
    }
    else if (strcmp(argv[1], "-") == 0) //切换回之前的路径
    {
        char strpwd1[100]; //保存当前路径
        getcwd(strpwd1, sizeof(prevpwd));
        chdir(prevpwd); //切换到之前的路径
        strcpy(prevpwd, strpwd1);
    }
    else if (strcmp(argv[1], "~") == 0)
    {
        getcwd(prevpwd, sizeof(prevpwd));
        chdir("/home/xvzewen");
    }
    else
    {
        getcwd(prevpwd, sizeof(prevpwd));
        chdir(argv[1]);
    }
}

void my_signal()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
}

void DoCommandPipe(char *argv[], int size, int backflag) //处理管道
{
    //获得每个管道的位置
    int pipepos[5];
    int pipe_num = 0; //计算管道的数量
    int in,out;
    in=out=0;
    char *filename="1.txt";
    int rflag=0;
    for (int t = 0; t < size; t++)
    {
        
        if (strcmp(argv[t], "|") == 0||strcmp(argv[t],">")==0)
        {
            pipepos[pipe_num++] = t;
        }
    }
    int cmd_num = pipe_num + 1; 
    //获得管道之间的命令
    char *cmd[cmd_num][7];

    for (int i = 0; i < cmd_num; i++) //获得那些命令,
    {
        if (i == 0) //第一个命令
        {
            int n = 0;
            for (int j = 0; j < pipepos[0]; j++)
            {
                cmd[i][n++] = argv[j];
            }
            cmd[i][n] = NULL;
        }
        else if (i == pipe_num) //最后一个命令
        {
            int n = 0;
            for (int j = pipepos[i - 1] + 1; j < size; j++)
            {
                cmd[i][n++] = argv[j];
            }
            cmd[i][n] = NULL;
        }
        else
        {
            //中间命令
            int n = 0;
            for (int j = pipepos[i - 1] + 1; j < pipepos[i]; j++)
            {
                cmd[i][n++] = argv[j];
            }
            cmd[i][n] = NULL;
        }
    }
    //创建子进程来执行这些操作
    int fd[pipe_num][2]; //用来操作每一根管道
    int i = 0;
    pid_t pid; //对进程进行操作
    for (i = 0; i < pipe_num; i++)
    {
        pipe(fd[i]); //创建管道，对应的是每个i的管道
    }
    for (i = 0; i < cmd_num; i++)
    {
        if ((pid = fork()) == 0) //对应i的进程
        {
            break;
        }
    }
    if (pid == 0)
    {
        // child
        if (pipe_num)
        {
            if (i == 0)
            {
                //第一个进程，把读端关掉，写端绑定
                dup2(fd[0][1], 1);
                close(fd[0][0]);
                //其他管道读写端都关掉
                for (int j = 1; j < pipe_num; j++)
                {
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }
            else if (i == pipe_num) //执行最后一个命令
            {
                //把写端关掉，读端打开
                dup2(fd[i - 1][0], 0);
                close(fd[i - 1][1]);
                //其他的管道全关掉
                for (int j = 0; j < pipe_num - 1; j++)
                {
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }
            else
            {
                //中间命令
                //把该命令管道前面的读端绑定，写端关闭
                dup2(fd[i - 1][0], 0);
                close(fd[i - 1][1]);
                //把该命令后面的管道的写端绑定，读端关闭
                dup2(fd[i][1], 1);
                close(fd[i][0]);
                //其他全关闭
                for (int j = 0; j < pipe_num; j++) //除了我们操作的这两个管道以外的管道的读写端都关掉
                {
                    if (j != i && j != (i - 1))
                    {
                        close(fd[j][0]);
                        close(fd[j][1]);
                    }
                }
            }
        }

        int ret = execvp(cmd[i][0], cmd[i]); //执行命令
        if (ret == -1)
        {
            perror("execvp");
            exit(-1);
        }
    }
    if (in)
    {
        int fd = open(filename, O_RDONLY);
        dup2(fd, 0);
    }
    if (out)
    {
        int fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);
        dup2(fd, 1);
    }
    //父进程什么都不干，把管道的所有口都关掉
    for (i = 0; i < pipe_num; i++)
    {
        close(fd[i][0]);
        close(fd[i][1]); //父进程端口全部关掉
    }
    //每次子进程执行完之后都要让父进程等待

    for (i = 0; i < cmd_num; i++)
    {
        if (!backflag)
            wait(NULL);
        else
        {
            exit(0);
        }
    }
}
