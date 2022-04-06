#include<stdio.h>
int main()
{
  extern char** environ;
  int i=0;
  for(i=0;environ[i];i++)//打印环境变量
  {
   printf("%s\n",environ[i]); 
  }
  printf("i am a exec\n");
  return 0;
}
