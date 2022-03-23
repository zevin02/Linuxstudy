#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//void fun()
//{
//  int *x=(int*)malloc(10*sizeof(int));
//  x[10]=0;//堆溢出
//  //未释放
//  free(x);
// 
//}

void fun2()
{
  char* ptr=(char*)malloc(10);
  ptr[12]='a';//内存越界
  memcpy(ptr+1,ptr,5);//内存重叠
  char a[10];
  a[12]='i';
  free(ptr);
  free(ptr);
  char*p1;
  *p1='a';//p1未初始化，赋值会错误
}

int main()
{
 // fun();
  fun2();
  return 0;
}


