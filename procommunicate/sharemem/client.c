

#include"head.h"
int main()
{
  //先创建一个共享内存
  //我们同样是让server去创建‘
  key_t key=ftok(MY_FILE,PROJECTID); //这是一个无符号整数
  if(key<0)
  {
    perror("ftok");
    return 1;
  }
  //获得了一个key值,但是这个key现在是只能在server这个进程使用，我们拷贝一下让client也能使用
  printf("key=%u\n",key);
  
  return 0;
}

