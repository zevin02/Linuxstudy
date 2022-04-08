#include<stdio.h>
int main()
{
  //FILE* fp=fopen("./log.txt","r");
  FILE* fp=fopen("./log.txt","a");//追加，不会覆盖掉
  if(NULL==fp)
  {
    perror("fopen");
    return 1;

  }
//  char buffer[32];
//  while(fgets(buffer,sizeof(buffer),fp)!=NULL)
//  {
//    printf("%s", buffer);
//  }
//  if(!feof(fp))
//  {
//    printf("fgets quit not normal\n");
//
//  }
//  else 
//  {
//    printf("fgets quit normal\n");
//  }

  int cnt=10;
  const char* msg="hello ";
  while(cnt--){
  fputs(msg,fp);
  }
  fclose(fp);
  return 0;
}
