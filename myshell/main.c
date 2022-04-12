#include "myshell.h"

//解释器的本质，先给一个提示符，在获取你的不断输入

int main()
{
  //一定是一个死循环程序
  my_signal();

  char command[NUM];
  for (;;)
  {
    char *argv[CMD_NUM] = {NULL};
    // 1.打印提示符号
    command[0] = 0; //用这样的方式，可以做到以O(1)的时间复杂度，清空字符串，因为c语言以\0结尾
    printf("[xzw-super-shell]# ");
    // PrintHint();
    fflush(stdout); //因为没有\n所以不会刷新，把输入缓冲区给清空
    //想办法把在命令行上的一个一个参数，获取到
    //获取命令字符串
    fgets(command, NUM, stdin); //从标准输入流里面读取数据，command里面就是我们想要的东西了
    //会发现空了一行，是因为回车也是一个字符
    command[strlen(command) - 1] = '\0'; //把\n赋成0，就把\n给吞掉了
                                         //解析命令字符串
                                         //"ls -a -l \0"

    // printf("echo: %s\n",command); 
    //获取command里面的每一个字符串，分割字符串
    // strtok截取字符串
    const char *sep = " ";
    argv[0] = strtok(command, sep);
    int i = 1;
    while (argv[i] = strtok(NULL, sep))
    {
      i++;
    }
    
    // 已经完成解析了
    //执行命令
    //不能让父进程直接替换
    //替换失败就终止
    //以内建命令的方式进行运行（不创建子进程）
    //相当于调用了自己的一个函数

    //检测命令是否需要shell自己去执行
    CommandAnalys(argv, i+1);

    // if (IsFarDo(argv[0]))
    // {
    //   // DoFarProcess(argv[0], argv);
    //   continue;
    // }
    // // 5.执行第3方命令
    // if (fork() == 0) //这里我们touch东西，cd的话都是子进程，但是我们需要让父进程去进行这些操作
    // {
    //   // fork执行的是第三方命令，独立的命令
    //   //  if(argv[0]=="|")
    //   //  {
    //   //    Do_pipe(argv);
    //   //  }
    //   execvp(argv[0], argv);
    //   exit(1);
    // int status = 0;
    // waitpid(-1, &status, 0);                         //等待任意一个进程
    // printf("exit code: %d \n", WEXITSTATUS(status)); //得到退出码
  }

  // for(i=0;argv[i];i++)
  // {
  //   printf("%s\n",argv[i]);
  // }
  return 0;
}
