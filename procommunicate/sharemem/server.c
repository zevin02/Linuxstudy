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
  //printf("key=%u\n",key);
  //帮助我们在系统当中能够找到同样的共享内存
  int shmid=shmget(key,SIZE,IPC_CREAT|IPC_EXCL|0666) ;//作为server我们想要让他创建一个全新的共享内存，如果和系统已经存在的id冲突，我们出错返回,我们用0666，给共享内存设置权限，权限就是0666
  //成功了一定是一个全新的共享内存
  if(shmid<0)
  {
    perror("shmget");
    return 2;
  }
  printf("%d -> shm creat  success",shmid);
  //sleep(10);
  //让我们的进程和共享内存关联上
  char* mem=(char*)shmat(shmid,NULL,0);
  
  printf("attach success\n");
  //sleep(5);
  //相当于这前面都在构造函数里面
  //这里就是我们的通信逻辑
  
  while(1)
  {
    sleep(1);
    //这里我们有没有调用类似pipe or fifo 类似的read这样的接口呢？
    //我们可以认为server是一个读取共享内存的程序
    printf("%s\n",mem);//server认为共享内存里面放的是一个长字符串
    //server不断 读取client向mem里面输入的东西
    
    //server或者client并没有调用系统调用接口，read或write
    //所以共享内存一定建立好就可以直接映射到自己的进程地址空间里面，该进程就可以直接看到该共享内存，就如同我们malloc的空间，一般不需要如何系统调用接口，
    //read或write的本质就是，将数据结构从内核拷贝给用户，或者从用户拷贝到内核
    //
    //当client没有启动的时候，server端根本不会因为client不写，而不读
    //共享内存是所有的进程间通信的速度最快的
    //
    //共享内存不提供任何的同步或互斥机制，需要程序员自行保证数据的安全
  
  }



  //这后面都在析构函数里面
  shmdt(mem);//去关联

  printf("detach success\n");
  //sleep(5);

  //最后不用的时候就手动的删掉就可以了
  shmctl(shmid,IPC_RMID,NULL);// 我们使用shmid把这个内存给删除掉，属性设置为NULL，

  printf("%d -> shm delete success",shmid);
  //sleep(10);
  
  return 0;
}

