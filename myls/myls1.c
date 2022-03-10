#include<stdio.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>


int aflag=0,lflag=0;

void display_dir(char*dir)//打印文件名
{
    DIR *mydir;
    struct dirent*myitem;
    char fname[256];
    //打开目录
    if((mydir=opendir(dir))==NULL)
    {
        perror("fail to opendir\n");
        return;
    }
    while((myitem=readdir(mydir))!=NULL)//当把当前目录中的文件都读完，就返回NULL
    {
        if(myitem->d_name[0]=='.'&& aflag==0)//没有-a这个选项，文件名的第一个字符是.就代表是一个隐藏文件，就跳过不去打印，
        {
            continue;
        }
        else  if(lflag)//有 -l ,有-a
        {
            sprintf(fname,"%s%s",dir,myitem->d_name);

        }
        else //ls后面什么都没有
        {
            printf("%s ",myitem->d_name);
        }
    }
    printf("\n");
    closedir(mydir);

}


int main(int argc,char*argv[])    
    
{
    int ch;
    struct stat;//用来显示文件的信息
    opterr=0;//用来解析命令行参数命令,控制是否向STDERR打印错误。为0，则关闭打印
   //optind默认是1，调用一次getopt就会+1
    while((ch=getopt(argc ,argv,"la"))!=-1)//getopt用来解析命令行的参数，返回int，错误就返回-1,解析-a和-l两个命令,getopt处理-开头的参数
        //
        //每次getopt后，这个索引指向argv里当前分析的字符串的下一个索引，因此,optind也就往后移动
        //argv[optind]就能得到下个字符串
    {
        switch(ch)//ch用来接收其中的参数
        {
            case 'a':
                aflag=1;
                break;
            case 'l':
                lflag=1;
                break;
        }

    }
    if(optind==argc)//相当于没有参数，没有过上面的循环
    {
    display_dir(".");//默认执行当前目录
    }
        return 0;
}


