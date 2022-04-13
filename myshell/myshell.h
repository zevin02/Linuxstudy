#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>


//解决颜色和避免输入太长导致被覆盖掉
#define CLOSE "\001\033[0m\002"                   // 关闭所有属性
#define BLOD "\001\033[1m\002"                    // 强调、加粗、高亮
#define BEGIN(x, y) "\001\033[" #x ";" #y "m\002" // x: 背景，y: 前景
#define NUM 128
#define CMD_NUM 64

//读取命令使用记录，上一次运行的记录也会存在
extern void add_history PARAMS((const char *)); //解决
extern int read_history PARAMS((const char *));
extern int write_history PARAMS((const char *));
extern HIST_ENTRY **history_list PARAMS((void));//用来执行history命令的声明


void CommandAnalys(char *argv[], int size);
int IsFarDo(char *filename);                     //判断是否需要父进程去执行
void DoFarProcess(char *filename, char *argv[]); //需要父进程去实现的命令
void Do_cd(char *filename, char *argv[]);        //执行cd命令
// void Do_pipe(char*argv[]);//子进程执行管道命令
void my_signal();
void PrintHint();
void DoRedefDir(char *argv[], int size, int rightpos, char *command);  //实现输出重定向
void DoInputRedef(char *argv[], int size, int leftpos, char *command); //输入重定向
void DoProcExit();
void ShowHistory();
