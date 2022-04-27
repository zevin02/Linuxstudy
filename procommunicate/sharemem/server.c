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
  if(shmid<0)
  {
    perror("shmget");
    return 2;
  }
printf("%d -> shm delete success",shmid);
  sleep(10);
  shmctl(shmid,IPC_RMID,NULL);// 我们使用shmid把这个内存给删除掉，属性设置为NULL，

  printf("%d -> shm delete success",shmid);
  sleep(10);
  
  return 0;
}

