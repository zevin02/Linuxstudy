#include<stdio.h>
#include<string.h>
int main()
{
  //printf("hello\n");
  int a=10;
  a/=0;//跑到一半异常终止
  //在vs中一般叫程序崩溃
  //

  int i=0;
  for(i=0;i<140;i++)
  {
    printf("%d: %s \n",i,strerror(i));//返回错误信息，;里面对应的i就是返回的错误码，返回的信息就是对应的错误信息
  }
  
  return 123;
}
