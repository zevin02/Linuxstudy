#include"myshell.h"


int IsFarDo(char*filename)
{
    if(strcmp(filename,"cd")==0||strcmp(filename,">")==0||strcmp(filename,">>")==0||strcmp(filename,"<")==0)
    return 1;
    return 0;
}


void DoFarProcess(char* filename,char*argv[])
{
    
    if(strcmp(filename,"cd")==0)
    {
      //内建命令
      Do_cd(filename,argv);
    }

}

void Do_cd(char* filename,char*argv[])
{
    if(argv[1])
    {
        chdir(argv[1]);
    }
}

void Do_pipe(char*argv[])
{
    
}