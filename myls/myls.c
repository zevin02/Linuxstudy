#include<sys/types.h>
#include<dirent.h>
#include<stdio.h>
void ls(char dirname [])//目录名称
{
DIR* dir_ptr;//目录指针
struct dirent *direntp;//目录中的文件内容参数
if((dir_ptr=opendir(dirname))==NULL)//假如说失败了
{
    fprintf(stderr,"can not open %s\n ",dirname);
}
else 
{
    while((direntp=readdir(dir_ptr)))
    {
        printf("%s  ",direntp->d_name);
    }
    printf("\n");
    closedir(dir_ptr);
}
}
int main(int argc,char*argv[])//size是字符串的长度 dirent是目录的路径名
{
    if(argc==1)//none string 
    {
        ls(".");//default current direct
    }
    else 
    {
    while(--argc)
    {
//        printf("%s:\n",* ++dirent);
++argv;
        ls(*argv);
    }
    }
    return 0;
}
