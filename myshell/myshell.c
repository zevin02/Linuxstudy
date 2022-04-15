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

                // int ret=callCommandWithPipe(argv, 0, size);

                DoPipe(argv, size, i);
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
        // else if(strcmp(argv[1],"-")==0)////回到上一次的路径
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
    int piped[10];
    int n = 0;
    for (int i = 0; i < size; i++)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            piped[n++] = i;
        }
    }
    for (int k = 0; k < n; k++)
    {
        char *wstream[piped[k] + 1]; //存管道前面的内容
        char **rstream = NULL;
        if (k + 1 < n)
        {
            rstream = (char **)malloc(sizeof(char *) * (piped[k + 1] - piped[k]));
        }
        else
        {
            rstream = (char **)malloc(sizeof(char *) * (size - piped[k]));
        }
        int wpos = 0;
        int rpos = 0;
        int left = 0;
        if (k > 0)
            left = piped[k] + 1;
        int right;
        if (k + 1 < n)
            right = piped[k + 1];
        else
            right = size;
        for (int i = left; i < right; i++)
        {
            if (i < piped[k])
                wstream[wpos++] = argv[i];
            else if (i > piped[k])
                rstream[rpos++] = argv[i];
        }
        wstream[piped[k]] = NULL;
        if (k + 1 < n)
            rstream[piped[k + 1] - piped[k] - 1] = NULL;
        else
            rstream[size - piped[k] - 1] = NULL;
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
                sleep(1);
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
        sleep(1);
        waitpid(id, NULL, 0);
    }
}

// void callCommandWithPipe(char *argv[], int left, int right)
// { // 所要执行的指令区间[left, right)，可能含有管道

//     if (left >= right)
//         return;
//     /* 判断是否有管道命令 */
//     int pipeIdx = -1;
//     for (int i = left; i < right; ++i)
//     {
//         if (strcmp(argv[i], "|") == 0)
//         {
//             pipeIdx = i;
//             break;
//         }
//     }
//     // if (pipeIdx == -1) { // 不含有管道命令
//     //     return callCommandWithRedi(left, right);
//     // }
//     if (pipeIdx + 1 == right)
//     { // 管道命令'|'后续没有指令，参数缺失
//         return;
//     }
//     char *wstream[pipeIdx + 1];     //存管道前面的内容
//     char *rstream[right - pipeIdx]; //存管道后面的内容
//     int wpos = 0;
//     int rpos = 0;
//     for (int i = 0; i < right; i++)
//     {
//         if (i < pipeIdx)
//             wstream[wpos++] = argv[i];
//         else if (i > pipeIdx)
//             rstream[rpos++] = argv[i];
//     }
//     wstream[pipeIdx] = NULL;
//     rstream[right - pipeIdx - 1] = NULL;

//     /* 执行命令 */
//     int fds[2];
//     pipe(fds);
//     if (pipe(fds) == -1)
//     {
//         return;
//     }
//     int result = 1;
//     pid_t pid = fork();
//     if (pid == -1)
//     {
//         return;
//     }
//     else if (pid == 0)
//     { // 子进程执行单个命令
//         close(fds[0]);
//         dup2(fds[1], STDOUT_FILENO); // 将标准输出重定向到fds[1]
//         close(fds[1]);

//         execvp(rstream[0], rstream);
//         exit(result);
//     }
//     else
//     { // 父进程递归执行后续命令
//         int status;
//         waitpid(pid, &status, 0);
//         int exitCode = WEXITSTATUS(status);

//         if (pipeIdx + 1 < right)
//         {
//             close(fds[1]);
//             dup2(fds[0], STDIN_FILENO); // 将标准输入重定向到fds[0]
//             close(fds[0]);
//             callCommandWithPipe(argv, pipeIdx + 1, right); // 递归执行后续指令
//         }
//     }

// }

// int callCommandWithPipe(char *argv[], int left, int right)
// { // 所要执行的指令区间[left, right)，可能含有管道
//     if (left >= right)
//         return -1;
//     /* 判断是否有管道命令 */
//     int pipeIdx = -1;
//     for (int i = left; i < right; ++i)
//     {
//         if (strcmp(argv[i], "|") == 0)
//         {
//             pipeIdx = i;
//             break;
//         }
//     }
//     if (pipeIdx == -1)
//     { // 不含有管道命令
//         return callCommandWithRedi(argv, left, right);
//     }
//     else if (pipeIdx + 1 == right)
//     { // 管道命令'|'后续没有指令，参数缺失
//         return -2;
//     }

//     /* 执行命令 */
//     int fds[2];
//     if (pipe(fds) == -1)
//     {
//         return -1;
//     }
//     int result;
//     pid_t pid = fork();
//     if (pid == 0)
//     { // 子进程执行单个命令
//         close(fds[0]);
//         dup2(fds[1], STDOUT_FILENO); // 将标准输出重定向到fds[1]
//         close(fds[1]);

//         // callCommandWithRedi(argv, left, pipeIdx);
//         // int fd;

//         // int oldfd;
//         // oldfd = dup(1);
//         // dup2(fd, 1);
//         char *tmp[pipeIdx+1];
//         for (int i = 0; i < pipeIdx; i++)
//         {
//             tmp[i] = argv[i];
//         }
//         tmp[pipeIdx] = NULL;
//         pid_t id=fork();
//         if (id == 0)
//         {
//             execvp(argv[0], tmp);
//         }

//         waitpid(id, NULL, 0);
//         // dup2(oldfd, 1);
//         // close(fd);
//         exit(0);
//     }
//     else
//     { // 父进程递归执行后续命令
//         int status;
//         waitpid(pid, &status, 0);
//         int exitCode = WEXITSTATUS(status);
//         result=exitCode;
//     }
//     if (pipeIdx + 1 < right)
//     {
//         close(fds[1]);
//         dup2(fds[0], STDIN_FILENO); // 将标准输入重定向到fds[0]
//         close(fds[0]);
//         result=callCommandWithPipe(argv, pipeIdx + 1, right); // 递归执行后续指令
//     }
//     return result;
// }

// int callCommandWithRedi(char *argv[], int left, int right)
// { // 所要执行的指令区间[left, right)，不含管道，可能含有重定向

//     /* 判断是否有重定向 */
//     int inNum = 0, outNum = 0;
//     char *inFile = NULL, *outFile = NULL;
//     int endIdx = right; // 指令在重定向前的终止下标

//     for (int i = left; i < right; ++i)
//     {
//         if (strcmp(argv[i], "<") == 0)
//         { // 输入重定向
//             ++inNum;
//             if (i + 1 < right)
//                 inFile = argv[i + 1];
//             else
//                 return 1; // 重定向符号后缺少文件名

//             if (endIdx == right)
//                 endIdx = i;
//         }
//         else if (strcmp(argv[i], "<") == 0)
//         { // 输出重定向
//             ++outNum;
//             if (i + 1 < right)
//                 outFile = argv[i + 1];
//             else
//                 return 2; // 重定向符号后缺少文件名

//             if (endIdx == right)
//                 endIdx = i;
//         }
//     }
//     /* 处理重定向 */
//     if (inNum == 1)
//     {
//         FILE *fp = fopen(inFile, "r");
//         if (fp == NULL) // 输入重定向文件不存在
//             return 2;

//         fclose(fp);
//     }

//     if (inNum > 1)
//     { // 输入重定向符超过一个
//         return 1;
//     }
//     else if (outNum > 1)
//     { // 输出重定向符超过一个
//         return 1;
//     }
//     int result=0;
//     pid_t pid = fork();
//     if(pid<0)
//     result=-1;
//     if (pid == 0)
//     {
//         /* 输入输出重定向 */
//         if (inNum == 1)
//             freopen(inFile, "r", stdin);
//         if (outNum == 1)
//             freopen(outFile, "w", stdout);

//         /* 执行命令 */
//         char *comm[endIdx - left + 1];
//         int pos = 0;
//         for (int i = left; i < endIdx; ++i)
//             comm[pos++] = argv[i];
//         comm[pos] = NULL;
//         execvp(comm[0], comm);
//         // 执行出错，返回errno
//     }
//     else
//     {
//         int status;
//         waitpid(pid, &status, 0);
//         int err = WEXITSTATUS(status); // 读取子进程的返回码
//         if (err)
//         { // 返回码不为0，意味着子进程执行出错，用红色字体打印出错信息
//             printf("\e[31;1mError: %s\n\e[0m", strerror(err));
//         }
//     }
//     return result;
// }