//#include<stdio.h>
//#include<string.h>
//#include<unistd.h>
//#include<stdlib.h>
//#include<pthread.h>
//#include <sys/stat.h>
//#include <dirent.h>
//
//void dp_R(char*dir);
//void IsFile(char* dir);
//
//void print(struct stat buf,char* filename)
//{
//                if(S_ISREG(buf.st_mode))//一般文件
//                {
//                  if(buf.st_mode&S_IXOTH)//可执行文件,打印成绿色
//                  {
//                  printf("\033[32m %s  \033[0m",filename);
//                  }
//                  else 
//                  {
//
//                printf("%s  ",filename);//直接接打印文件名,不显示文件
//                  }
//                }
//                else if(S_ISDIR(buf.st_mode))//是一个目录打印蓝色
//                {
//                  printf("\033[34m %s  \033[0m",filename);
//                }
//}
//void IsFile(char* name)
//
//{
//  int ret=0;
//  struct stat sb;
//  ret=stat(name,&sb);
//  if(ret==-1)
//  {
//    perror("stat error");
//    return;
//  }
//  if(S_ISDIR(sb.st_mode))//是目录就继续遍历
//  {
//   dp_R(name);//是目录就继续下去递归
//  }
// printf("%s\t%ld\n",name,sb.st_size); 
//  return;
//}
//
//
//void dp_R(char*dir)
//{
//  char path[256];
//  DIR *dp;
//  struct dirent * sdp;
//  dp=opendir(dir);
//  if(dp==NULL)
//  {
//    perror("opendir error");
//    return;
//  }
//  while((sdp=readdir(dp))!=NULL)
//  {
//    if(strcmp(sdp->d_name,".")==0||strcmp(sdp->d_name,".."))
//    {
//      continue;
//    }
//    sprintf(path,"%s/%s",dir,sdp->d_name);
//    IsFile(path);
//  }
//  closedir(dp);
//  return;
//
//
//}
//
//int main(int argc,char* argv[])
//{
//  if(argc==1)
//  {
//    IsFile(".");
//
//  }
//  else 
//  {
//    IsFile(argv[1]);
//  }
//}
//


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/stat.h>
#include<dirent.h>

void read_dir(char *dir);

void isFile(char *name,char*filename);
void isFile(char *name,char*filename)
{
    int ret = 0;
    struct stat sb;
    ret = stat(name, &sb);
    if(S_ISDIR(sb.st_mode))
    {
      printf("%s: \n",name);
     
    if(ret == -1)
          {
                perror("stat error\n");
                    return;
          }
    DIR*dp;
    struct dirent* sdp;
    dp=opendir(name);
    while(sdp=readdir(dp))
    {
                    if(strcmp(sdp->d_name,".") == 0 ||strcmp(sdp->d_name,"..") == 0||sdp->d_name[0]=='.')
                    {
                            continue;
                                
                    }
printf("%s ",sdp->d_name);
    }
    printf("\n");
                  //是真就是目录//要考虑目录下还有目录
                      read_dir(name);
                  //        
         closedir(dp);
    }
     else 
     {
       return;
     }//          
         //   printf("%s ",filename);//路径

}

void read_dir(char *dir)
{
    char path[256];
      DIR *dp;
        struct dirent *sdp;
          dp = opendir(dir);
            if(dp == NULL)
            {
                  perror("opendir error");
                      return;
                        
            }
              while(sdp = readdir(dp))
              {
                    if(strcmp(sdp->d_name,".") == 0 ||strcmp(sdp->d_name,"..") == 0||sdp->d_name[0]=='.')
                    {
                            continue;
                                
                    }

                        //读这个目录项，继续判断是不是目录，如果是目录iu还得再进入，如果是文件就打印
                        //    //ifFile(sdp->d_name);//不能直接这样，要绝对路径才可以
                                sprintf(path, "%s/%s", dir, sdp->d_name);
                                isFile(path,sdp->d_name);

              }
printf("\n");
                closedir(dp);
                  return;

}

int main(int argc, char*argv[])
{
  if(argc == 1){
        isFile(".",".");
          
  }else{
        isFile(argv[1],argv[1]);

  }
    return 0;

}




