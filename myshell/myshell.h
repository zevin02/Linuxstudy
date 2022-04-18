#include <stdio.h>
#include<unistd.h>
#include <sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include <stdlib.h>
//#include <readline/readline.h>
#include<sys/types.h>
#include<fcntl.h>
#define  NUM 128
#define  CMD_NUM 64

void CommandAnalys(char*argv[],int size);
int IsFarDo(char*filename);//判断是否需要父进程去执行
void DoFarProcess(char* filename,char*argv[]);//需要父进程去实现的命令
void Do_cd(char* filename,char*argv[]);//执行cd命令
//void Do_pipe(char*argv[]);//子进程执行管道命令
void my_signal();
void PrintHint();
void DoRedefDir(char*argv[],int size,int youpos,char*command);
void DoInRedefDir(char* argv[],int size,int i);
