#include "myshell.h"
extern char *cdpath[30];
extern int cdpos;
void CommandAnalys(char *argv[], int size/*,char* buf*/)
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

               ; //DoPipe(argv, size);
                // command_with_Pipe(buf);
                DoCommandPipe(argv,size);
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




void DoCommandPipe(char* argv[],int size)//处理管道
{
    //获得每个管道的位置
    int pipepos[5];
    int pipe_num=0;//计算管道的数量
    for(int i=0;i<size;i++)
    {
        if(strcmp(argv[i],"|")==0)
        {
            pipepos[pipe_num++]=i;
        }
    }
    int cmd_num=pipe_num+1;

    //获得管道之间的命令
    char* cmd[cmd_num][7];

    for(int i=0;i<cmd_num;i++)//获得那些命令
    {
        if(i==0)//第一个命令
        {
            int n=0;
            for(int j=0;j<pipepos[0];j++)
            {
                cmd[i][n++]=argv[j];
            }
            cmd[i][n]=NULL;
        }
        else if(i==pipe_num)//最后一个命令
        {
            int n=0;
            for(int j=pipepos[i-1]+1;j<size;j++)
            {
                cmd[i][n++]=argv[j];
            }
            cmd[i][n]=NULL;
        }
        else
        {
            //中间命令
            int n=0;
            for(int j=pipepos[i-1]+1;j<pipepos[i];j++)
            {
                cmd[i][n++]=argv[j];
            }
            cmd[i][n]=NULL;
        }
    } 
    //创建子进程来执行这些操作
    int fd[pipe_num][2];
    int i=0;
    pid_t pid;
    for(i=0;i<pipe_num;i++)
    {
        pipe(fd[i]);//创建管道
    }
    for(i=0;i<cmd_num;i++)
    {
        if((pid=fork())==0)
        {
            break;
        }
    }
    if(pid==0)
    {
        //child
        if(pipe_num)
        {
            if(i==0)
            {
                //第一个进程
                dup2(fd[0][1],1);
                close(fd[0][0]);
                //其他都关掉
                for(int j=1;j<pipe_num;j++)
                {
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }
            else if(i==pipe_num)//执行最后一个命令
            {
                dup2(fd[i-1][0],0);
                close(fd[i-1][1]);
                //其他全关掉
                for(int j=0;j<pipe_num-1;j++)
                {
                    close(fd[j][0]);
                    close(fd[j][1]);
                }

            }
            else
            {
                //中间命令
                //执行前面的读端，和后面的写端
                dup2(fd[i-1][0],0);
                close(fd[i-1][1]);
                dup2(fd[i][1],1);
                close(fd[i][0]);
                //其他全关闭
                for(int j=0;j<pipe_num;j++)
                {
                    if(j!=i||j!=(i-1))
                    {
                        close(fd[j][0]);
                        close(fd[j][1]);
                    }
                }
            }
        }
        execvp(cmd[i][0],cmd[i]);
    }
    //父进程
    for(i=0;i<pipe_num;i++)
    {
        close(fd[i][0]);
        close(fd[i][1]);//父进程端口全部关掉

    }
    for(i=0;i<cmd_num;i++)
    {
        wait(NULL);
    }


}


// void DoPipe(char *argv[], int size, int pipepos) //实现管道
// {
//     int piped[10];
//     int n = 0;
//     for (int i = 0; i < size; i++)
//     {
//         if (strcmp(argv[i], "|") == 0)
//         {
//             piped[n++] = i;
//         }
//     }
//     for (int k = 0; k < n; k++)
//     {
//         char *wstream[piped[k] + 1]; //存管道前面的内容
//         char **rstream = NULL;
//         if (k + 1 < n)
//         {
//             rstream = (char **)malloc(sizeof(char *) * (piped[k + 1] - piped[k]));
//         }
//         else
//         {
//             rstream = (char **)malloc(sizeof(char *) * (size - piped[k]));
//         }
//         int wpos = 0;
//         int rpos = 0;
//         int left = 0;
//         if (k > 0)
//             left = piped[k] + 1;
//         int right;
//         if (k + 1 < n)
//             right = piped[k + 1];
//         else
//             right = size;
//         for (int i = left; i < right; i++)
//         {
//             if (i < piped[k])
//                 wstream[wpos++] = argv[i];
//             else if (i > piped[k])
//                 rstream[rpos++] = argv[i];
//         }
//         wstream[piped[k]] = NULL;
//         if (k + 1 < n)
//             rstream[piped[k + 1] - piped[k] - 1] = NULL;
//         else
//             rstream[size - piped[k] - 1] = NULL;
//         pid_t id = fork();

//         if (id == 0)
//         {
//             int fd[2];
//             int ret = pipe(fd);
//             pid_t id1 = fork();
//             if (id1 > 0)
//             {
//                 //父进程执行写端
//                 close(fd[0]); //关闭读段
//                 dup2(fd[1], 1);
//                 // close(fd[1]);

//                 int ret = execvp(wstream[0], wstream);
//                 if (ret == -1)
//                 {
//                     printf("fail\n");
//                 }
//                 sleep(1);
//                 waitpid(id, NULL, 0);
//             }
//             else if (id1 == 0)
//             {
//                 close(fd[1]);
//                 int save=dup(0);
//                 dup2(fd[0], 0);
//                 // close(fd[0]);
//                 execvp(rstream[0], rstream);
//                 // exit(0);
//                 dup2(save,0);
//             }
//         }
//         sleep(1);
//         waitpid(id, NULL, 0);
//     }
// }

// void DoPipe(char *argv[], int size)
// {
//     int piped[10];
//     int flag = 1;
//     int n = 0;
//     for (int i = 0; i < size; i++)
//     {
//         if (strcmp(argv[i], "|") == 0)
//         {
//             piped[n++] = i;
//         }
//     }
//     pid_t id = fork();
//     int fd[2];
//     int fd1[2];
//     if (id == 0)
//     {
//         for (int i = 0; i <= n; i++)
//         {

//             if (flag)
//             {
//                 pipe(fd);
//                 close(fd[0]);
//                 dup2(fd[1], 1);
//                 pid_t id1 = fork();
//                 if (id1 > 0)
//                 {
//                     // runexecv
//                     char *command[5];
//                     int k = 0;
//                     for (k = 0; k < piped[i]; k++)
//                     {
//                         command[k] = argv[k];
//                     }
//                     command[k] = NULL;
//                     execvp(command[0], command);
//                     waitpid(id1, NULL, 0);
//                 }
//                 flag = ~flag;
//             }
//             else if (i < n)
//             {
//                 close(fd[1]);
//                 dup2(fd[0], 0);

//                 pipe(fd1);
//                 dup2(fd1[1], 1);
//                 close(fd1[0]);
//                 pid_t id2 = fork();
//                 if (id2 > 0)
//                 {
//                     // run
//                     char *command[5];
//                     int pos = 0;
//                     for (int k = piped[i - 1] + 1; k < piped[i]; k++)
//                     {
//                         command[pos++] = argv[k];
//                     }
//                     command[pos] = NULL;
//                     execvp(command[0], command);
//                     waitpid(id2, NULL, 0);
//                 }
//             }
//             else if (i == n)
//             {
//                 close(fd[0]);
//                 dup2(fd[1], 1);
//                 // run code without pipe
//                 char *command[5];
//                 int pos = 0;
//                 for (int k = piped[i - 1]; k < size; k++)
//                 {
//                     command[pos++] = argv[k];
//                 }
//                 command[pos] = NULL;
//                 execvp(command[0], command);
//                 waitpid(id ,NULL, 0);
//             }
//             // exchange pipe
//             fd[0] = fd1[0];
//             fd[1] = fd1[1];
//         }
//         // else if (i == n)
//         // {
//         //     close(fd[0]);
//         //     dup2(fd[1], 1);
//         //     // run code without pipe
//         //     int pos = 0;
//         //     for (int k = piped[i - 1]; k < size; k++)
//         //     {
//         //         command[pos++] = argv[k];
//         //     }
//         //     command[pos] = NULL;
//         //     execvp(command[0], command);
//         //     waitpid(id1, NULL, 0);
//         // }

//         // pipe(fd);
//         // if(flag)
//         // {
//         //     close(fd[0]);
//         //     dup2(fd[1],1);
//         //     flag=~flag;
//         // }
//         // else
//         // {
//         //     close(fd[1]);
//         //     dup2(fd[0],0);
//         //     flag=~flag;
//         // }
//         // pid_t id1=fork();
//         // if(id1==0)
//         // {
//         // }
//         // else
//         // {
//         //     //father
//         //     char* command[5];
//         //     //得到命令
//         // if (i == 0)
//         // {
//         //     int k = 0;
//         //     for (k = 0; k < piped[i]; k++)
//         //     {
//         //         command[k] = argv[k];
//         //     }
//         //     command[k] = NULL;
//         //     execvp(command[0], command);
//         //     waitpid(id1, NULL, 0);
//         // }
//         // //     else if(i==n)
//         // //     {
//         // //         //最后一次
//         // int pos = 0;
//         // for (int k = piped[i - 1]; k < size; k++)
//         // {
//         //     command[pos++] = argv[k];
//         // }
//         // command[pos] = NULL;
//         // execvp(command[0], command);
//         // waitpid(id1, NULL, 0);
//         // //     }
//         // else
//         // {
//         //     int pos = 0;
//         //     for (int k = piped[i - 1] + 1; k < piped[i]; k++)
//         //     {
//         //         command[pos++] = argv[k];
//         //     }
//         //     command[pos] = NULL;
//         //     execvp(command[0], command);
//         //     waitpid(id1, NULL, 0);
//         // }

//         // }
//     }

// else if (id > 0)
// {
//     waitpid(id, NULL, 0);
// }
// else
// {
//     perror("fork");
// }
// }

//mustuse

// int command_with_Pipe(char *buf)
// {
//     int i, j;
//     int cmd_num = 0, pipe_num = 0;
//     int fd[16][2];
//     char *curcmd;
//     char *nextcmd = buf;
//     for (int k = 0; buf[k]; k++){
//         if(buf[k] == '|'){
//             pipe_num++;
//         }
//     }
//     while ((curcmd = strsep(&nextcmd, "|"))){
//         if(parse_pipe(curcmd, cmd_num++) < 0){
//             cmd_num--;
//             break;
//         }
//         if(cmd_num == 17)//16根管道最多支持17条命令
//             break;
//     }

//     for (i = 0; i < pipe_num; i++){//创建管道
//         if(pipe(fd[i])){
//             my_error("pipe", __LINE__);
//         }
//     }

//     pid_t pid;
//     for (i = 0; i <= pipe_num; i++){ //管道数目决定创建子进程个数
//         if((pid = fork()) == 0)
//             break;
//     }

//     if(pid == 0){
//         if(pipe_num != 0){
        
//             if (i == 0){ //第一个创建的子进程
//             //管道的输入为标准输入
//                 dup2(fd[0][1], STDOUT_FILENO);
//                 close(fd[0][0]);

//                 for (j = 1; j < pipe_num; j++){
//                     close(fd[j][0]);
//                     close(fd[j][1]);
//                 }
//             }else if (i == pipe_num){ //最后一个创建的子进程
//             //管道的输出为标准输出
//                 dup2(fd[i-1][0], STDIN_FILENO);
//                 close(fd[i-1][1]);

//                 for (j = 0; j < pipe_num - 1; j++){
//                     close(fd[j][0]);
//                     close(fd[j][1]);
//                 }
//             }else{
//                 //重定中间进程的标准输入至管道读端
//                 dup2(fd[i-1][0], STDIN_FILENO); 
//                 close(fd[i-1][1]);
//                 //重定中间进程的标准输出至管道写端
//                 dup2(fd[i][1], STDOUT_FILENO);
//                 close(fd[i][0]);

//                 for (j = 0; j < pipe_num; j++){ //关闭不使用的管道读写两端
//                     if (j != i || j != i - 1){
//                         close(fd[j][0]);
//                         close(fd[j][1]);
//                     }
//                 }
//             }
//         }
//         pipe_num = 0;
//         execvp(cmd[i].argv[0], cmd[i].argv); //执行用户输入的命令
//         my_error("execvp",__LINE__);
//         exit(1);
//     }else{// parent
//     //关闭父进程所有管道
//     for (i = 0; i < pipe_num; i++){
//             close(fd[i][0]);
//             close(fd[i][1]);
//         }
        
//         for (i = 0; i <= cmd_num; i++){
//             int wpid = wait(NULL);
//             // if(wpid == -1)
//             // {
//             //     my_error("wait error",__LINE__);
//             // }
//         }
//     }
// }




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
