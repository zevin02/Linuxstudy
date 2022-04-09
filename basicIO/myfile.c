#include <stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
int main()
{
  const char *msg = "hello ";
  //fputs(msg,stdout);//直接向显示器去写入
  //标准错误
  //fputs(msg, stderr); //直接向显示器去写入
  
  #if 0
  FILE *fp = fopen("./log.txt", "r");
  FILE *fp = fopen("./log.txt", "a"); //追加，不会覆盖掉
  if (NULL == fp)
  {
    perror("fopen");
    return 1;
  }
  char buffer[32];
  while (fgets(buffer, sizeof(buffer), fp) != NULL)
  {
    printf("%s", buffer);
  }
  if (!feof(fp))
  {
    printf("fgets quit not normal\n");
  }
  else
  {
    printf("fgets quit normal\n");
  }

  int cnt = 10;
  const char *msg = "hello ";
  while (cnt--)
  {
    fputs(msg, fp);
  }
  
  fclose(fp);
  #endif
  //路径选项参数
  int fd=open("./g.txt",O_WRONLY|O_CREAT,0644);//一次传递两个标志位
  int fd1=open("./g1.txt",O_WRONLY|O_CREAT,0644);//一次传递两个标志位
  int fd2=open("./g2.txt",O_WRONLY|O_CREAT,0644);//一次传递两个标志位
  int fd3=open("./g3.txt",O_WRONLY|O_CREAT,0644);//一次传递两个标志位
  //以只写方式打开,如果文件不存在，就会帮助我们创建一个
  //相当于C语言中的w选项,0644以二进制的方式显示权限
  if(fd<0)
  {
    //打开文件失败
    printf("open err\n");
  }
  printf("fd:%d\n",fd);
  printf("fd:%d\n",fd1);
  printf("fd:%d\n",fd2);
  printf("fd:%d\n",fd3);
  close(fd);//文件就关掉了
  return 0;
}
