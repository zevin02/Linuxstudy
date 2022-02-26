#include<stdio.h>
#include<unistd.h>
#include<string.h>
//1.回车换行是先回车到当前行的第一个位置，在换到第二行的同列处
//
//c程序，默认会打开3个输入输出流：
//stdin  stdout    stderr
//键盘    显示器  显示器
//
void test1()
{

  printf("helln");
  //printf("hell\n");

  sleep(5);

  //我们观察到没有\n就后出现hell，
  //并不是说明sleep先于printf执行，而是因为没有\n，字符串会先保存起来（在c语言级别的缓冲区 ）
  //显示器

}


//只回车不换行
void test2()
{

  //实现一个倒计时的功能
  //凡是显示到显示器上面的内容都是字符，如%d，x  x=123，是‘1’‘2’，都是字符
  //凡是从键盘读取的内容都是字符，scanf（%d，&a”）；
int cout=10;
while(cout)
{
 printf("%2d\r",cout);//\r就是只回车不换行，如果这样的话，printf的内容就不会立刻显示
  fflush(stdout);//这样可以清理缓冲区
  cout--;
  sleep(1);
  //因为显示器都是字符，第一个字符是1，第二个字符是0
  //cout-1后变成9，回车后9在第一个字符，
  //变成2个字符位的就可以,在第二个宽位操作
}


}

//进度条，只在一行出现，不会换行
//#######

void bar()
{
#define NUM 100
  char bar[NUM+1];
  memset(bar,'\0',sizeof(bar));
  int i=0;
const char *label="|/-\\";
  while(i<=100)
  {
    //显示进度条
    printf("\033[31m[%-100s][%3d%%] [%c]\r",bar,i,label[i%4]);//回车就可以一次显示一,左对其
    fflush(stdout);
    bar[i]='#';
    i++;
    usleep(10000);//usleep以微秒为单位
  }
printf("\n");

}




int main()
{
  
 // test2();
  bar();
  return 0;
}



