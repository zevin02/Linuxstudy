

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
  int shmid=shmget(key,SIZE,0);//我们这里只需要获得就可以了
  //挂接上
  char* mem=(char*)shmat(shmid,NULL,0);
  //这个地方就是我们要通信的区域
  
  //我们让client去写，但是共享内存可不是管道，他既可以写也可以读，随便写
  char c='A';
  while(c<='Z')
  {
    //我们认为共享内存他当前是一个长字符串
    //我们就是在mem这个共有的空间里面进行操作,我们认为mem是一个长字符串
    mem[c-'A']=c;//客户端不断向mem的0下标处写
    c++;//这个字符串会不断的变长
    mem[c-'A']=0;
    sleep(1);
  }


  //取消挂接
  shmdt(mem);
  //sleep(5);
  printf("client process detach sucess\n");
  

  return 0;
}

