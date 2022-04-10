#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void FdWrite()
{
    int fd = open("demo.txt", O_CREAT | O_WRONLY, 0644);
    if (fd < 0)
    {
        perror("open");
        exit(-1);
    }
    const char *msg = "hello \n";
    int cnt = 5;
    while (cnt)
    {
        write(fd, msg, strlen(msg)); //我们写入文件的过程中，我们要不要加入\0呢，不需要，
        //因为\0作为字符串的结束标志位，只是c的规定，而文件关心字符串的内容，
        cnt--;
    }

    close(fd);
}
void FdRead()
{
    int fd=open("./demo.txt",O_RDONLY);//以读的方式打开不涉及到创建,权限也不要写了
    if(fd<0)
    {
        perror("open");
        exit(-1);
    }
    char buff[1024];
    ssize_t s=read(fd,buff,sizeof(buff)-1);//-1是因为我们不需要\0

    if(s>0)
    {
        //说明我们读取到了有效的内容
        //因为我们要把读取到的内容当作一个字符串看待，所以要在最结尾添加一个\0,作为字符串结束标志
        buff[s]=0;
        printf("%s\n",buff);
    }
    close(fd);
    
}
void Fd()
{
    int fd1=open("demo.txt",O_WRONLY);
    int fd2=open("demo.txt",O_WRONLY);
    int fd3=open("demo.txt",O_WRONLY);
    printf("%d ",fd1);
    printf("%d ",fd2);
    printf("%d ",fd3);

    close(fd1);
    close(fd2);
    close(fd3);
}



void Fd_DeIO()
{
    char buf[1024];
    ssize_t s=read(0,buf,sizeof(buf)-1);//直接从键盘上写入
    buf[s-1]=0;//添加\0作为结束的标志,但是回车也是一个按键，所以我们把回车建写成0,即可
    printf("echo :%s",buf);

    // const char* msg="hello";
    // write(2,msg,strlen(msg));//向标准输出去写入
    // write(2,msg,strlen(msg));//向标准输出去写入
    // write(2,msg,strlen(msg));//向标准输出去写入

}
void  Fd_Alloc_Base()
{
    close(1);
    // close(2);//把0对应的标准输入给关闭
    int fd=open("./g.txt",O_CREAT|O_WRONLY|O_APPEND,0644);
    printf("fd=%d \n",fd);
    printf("hello world");
    printf("hello world");
    printf("hello world");
    printf("hello world");
    printf("hello world");
    // close(fd);
}


void Fd_Redefout()
{
    close(0);//把输入给关掉
    int fd=open("./g.txt",O_RDONLY);
    char line[128];
    while(fgets(line,sizeof(line)-1,stdin))//因为现在g.txt的fd为0,所以stdin就是g.txt
    {
        //原本应该从键盘读取的内容，现在是从文件里面读取了
        //输入重定向
        printf("%s\n",line);
    }

}
void Verify_IO()
{
    printf("stdin-> %d",stdin->_fileno);
    printf("stdout-> %d",stdout->_fileno);
    printf("stderr-> %d",stderr->_fileno);
}
void Test_Dup2_Write()
{
    int fd=open("./g.txt",O_WRONLY|O_TRUNC|O_APPEND);//以只写的方式打开,打开的时候，还会帮我们清空,这就是>,没加的话就是>>
    if(fd<0)
    {
        perror("open");
        exit(-1);
    }
    
    int save=dup(STDOUT_FILENO);
    dup2(fd,1);//输出重定向，显示到显示器的内容，写入到文件
    printf("输出重定向");
    fprintf(stdout,"next redef");
    fputs("fputs",stdout);
    dup2(save,1);

    printf("save\n");
    printf("success");
    
    close(fd);
}


void Test_Dup2_Read()
{
    int fd=open("./x.txt",O_RDONLY);//以只写的方式打开,打开的时候，还会帮我们清空,这就是>,没加的话就是>>
    if(fd<0)
    {
        perror("open");
        exit(-1);
    }
    dup2(fd,0);//输入重定向，
    char buffer[1024];
    scanf("%s",buffer);//从g.txt里面读
    printf("%s",buffer);

}
int main()
{
//     FdWrite();
//     Fd();
//     Fd_DeIO();
//     FdRead();
    // Fd_Alloc_Base();
    // Fd_Redefout();
    // Verify_IO();
    Test_Dup2_Write();
    Test_Dup2_Read();
    return 0;
}