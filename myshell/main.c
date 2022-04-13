#include "myshell.h"

//解释器的本质，先给一个提示符，在获取你的不断输入

int main()
{
  //一定是一个死循环程序
  my_signal();
  read_history(NULL);
  for (;;)
  {
    // read_history(NULL);
    char *argv[CMD_NUM] = {NULL};
    //  1.打印提示符号
    // command[0] = 0; //用这样的方式，可以做到以O(1)的时间复杂度，清空字符串，因为c语言以\0结尾
    // printf("[xzw-super-shell]# ");
    char *command = readline(BEGIN(49, 34) "xzw super shell $ " CLOSE);
    add_history(command);
    write_history(NULL);
    if (command == NULL) //屏蔽ctrl d
    {
      continue;
    }

    // command[strlen(command) - 1] = '\0'; //把\n赋成0，就把\n给吞掉了
    //解析命令字符串
    //"ls -a -l \0"

    // printf("echo: %s\n",command);
    //获取command里面的每一个字符串，分割字符串
    // strtok截取字符串
    if (strlen(command) == 0)
    {
      continue;
    }
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

    CommandAnalys(argv, i);
    
    free(command);
  }
  write_history(NULL);
  return 0;
}
