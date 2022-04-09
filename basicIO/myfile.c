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
  int fd=open("./lg.txt",O_WRONLY|O_CREAT);//以只写方式打开,如果文件不存在，就会帮助我们创建一个
  //相当于C语言中的w选项,
  if(fd<0)
  {
    //打开文件失败
    printf("open err\n");
  }
  close(fd);//文件就关掉了
  return 0;
}
