#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>
#include<unistd.h>
#define MEM_CONTENT_LENGTH   128
//hook工具

typedef void*(*malloc_t)(size_t size);
malloc_t malloc_f=NULL;
typedef void*(*free_t)(void*p);
free_t free_f;
int enable_malloc_hook=1;//添加一个开关条件，避免里面的malloc递归的调用
int enable_free_hook=1;
//我们要知道malloc在哪里调用的
//0 ==f() ---> malloc,which function first call malloc
//1 ==s()--->f()__-->malloc 
//
void *malloc(size_t size)
{
  if(enable_malloc_hook)
  {
    enable_malloc_hook=0;
//  printf("malloc\n");//避免在printf里面递归的调用，类比拷贝构造递归的调用
  //我们可以创建一个文件，写入到文件里面进行操作
  // mem/%p:mem,在哪一行的
  void *p =malloc_f(size);
  void *caller=__builtin_return_address(0);//
  char buff[MEM_CONTENT_LENGTH]={0};
  sprintf(buff,"./mem/%p.mem",p);
  FILE * fp=fopen(buff,"w");//写入到文件里面
  fprintf(fp,"[+%p]malloc-->addr: %p, size: %lu\n",caller,p,size);//行，地址，
  fflush(fp);
  return p;
    enable_malloc_hook=1;
  }
  else 
  {
    return malloc_f(size);
  }
  //void *ptr=malloc_f(size);
  //return ptr;
}
void free(void *p)
{
  if(enable_free_hook)
  {
    enable_free_hook=0;
  char buff[MEM_CONTENT_LENGTH]={0};
  sprintf(buff,"./mem/%p.mem",p);
  if(unlink(buff)<0)//如果删除成功，返回一个大于0的值
  {
    printf("double free :%p\n",p);
  }
  free_f(p);
  enable_free_hook=1;
  }
  else 
  {
    free_f(p);
  }
  //printf("free\n");
  //free_f(p);
}
static int init_hook()//进程一开始我们就hook住，钩住
{
  malloc_f=dlsym(RTLD_NEXT,"malloc");
 //当我们的代码在加载的时候

  free_f=dlsym(RTLD_NEXT,"free"); 
}


int main()
{
  //printf("main\n");
  init_hook();//只加上了这一句,可以用这一个，通过判断malloc和free的数量来判断是否有内存错误，但是现在还无法判断在哪里有内存泄露
  void *p1=malloc(10);
  free(p1);
  void *p2=malloc(20);

  void *p3=malloc(30);
  free(p3);
  return 0;
}
