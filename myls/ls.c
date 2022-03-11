#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

int aflag = 0,lflag = 0;//作为标识符，如果aflag lflag为1则有-a和-l这个参数，执行选项

void display_file(char *fname , char *nname)//fname里面存放的是目录的路径
{
    struct stat buf;//buf用来接收文件的各种信息
    struct tm *t;//用来接收时间的参数，
    int i;

    if(stat(fname,&buf) < 0)//路径存放失败
    {
       perror("stat error\n");
       return ;                 
    }

            switch(buf.st_mode & S_IFMT)//s_ifmt是一个子域掩码，按位与的结果来判断是什么权限
            {
                case S_IFSOCK:  printf("s");  break;
                case S_IFLNK: printf("l");  break;
                case S_IFREG: printf("-");  break;
                case S_IFBLK: printf("b");  break;
                case S_IFDIR: printf("d");  break;
                case S_IFCHR: printf("c");  break;
                case S_IFIFO: printf("p");  break;
                                                                                                                                  
            }
          
           for(i = 8 ; i >= 0 ; i--)
           {
               if(buf.st_mode & (1 << i))//按位与之后非0，为真
               {
                   if(i % 3 == 2)  printf("r");
                   if(i % 3 == 1)  printf("w");
                   if(i % 3 == 0)  printf("x");
                                            
               }
               else
                   printf("-");                    
           }


                printf(" %2d ",buf.st_nlink);//硬链接数

                printf("%s ",getpwuid(buf.st_uid)->pw_name);
                printf("%s ",getgrgid(buf.st_gid)->gr_name);

                printf("%4ld ",buf.st_size);//所占的字节大小

                t = localtime(&buf.st_mtime);//用t来接收时间
                printf("%d-%02d-%02d  %02d:%02d ",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min);
                printf("%s\n",nname);//文件名
      return ;

}

void display_dir(char *dir)//显示目录下的所有文件，同时判断是否有-a选项，里面的是目录的路径名，有相对路径，也有绝对路径
{
    DIR *mydir;
    struct dirent *myitem;
    char fname[256];

    if((mydir = opendir(dir)) == NULL)
    {
        perror("fail to opendir!\n");
        return ;                     
    }
     while((myitem = readdir(mydir)) != NULL)
     {
         if(myitem->d_name[0] == '.' && !aflag)//没有-a参数，如果if条件成立的就继续下一次循环，否则往下执行
         {
           continue;//在目录往下继续搜索，遇到隐藏文件就跳过
         }
          if(lflag)//走到这里就说明有-a参数，ls -l -a dir，同时也有-l参数
        {
           sprintf(fname,"%s/%s",dir,myitem->d_name);//dir这个目录的路径名字，文件名，这些名字全都答应到fname这个字符串里面来接收
          // sprintf(fname,"%s",dir);//dir这个目录的路径名字，文件名，这些名字全都答应到fname这个字符串里面来接收
       //    printf("%s",fname);
        //fname里面是目录的名字和文件的名字
      
            display_file(fname,myitem->d_name);//把文件的秘密打印出来,第一个是目录的路径名，第二个是里面的文件名                                      
        }
          else // ls dir,没有参数，只显示一个目录，
          { 
            printf("%s  ",myitem->d_name);// 显示文件名，
          }                  
      }
         printf("\n");
         closedir(mydir);
         return ;

}

int main(int argc,char *argv[])
{
    int ch,i;
    struct stat buf;//用来记录文件的信息

        opterr = 0;
          //解析命令
          //用来解析命令行参数命令,控制是否向STDERR打印错误。为0，则关闭打印
          //optind默认是1，调用一次getopt就会+1
          //  
            while((ch = getopt(argc,argv,"la")) != -1)//getopt用来解析命令行的参数，返回int，错误就返回-1,解析-a和-l两个命令,getopt处理-开头的参数
              //每次getopt后，这个索引指向argv里当前分析的字符串的下一个索引，因此,optind也就往后移动
              //argv[optind]就能得到下个字符串
              //
              {
                  switch(ch)//用ch来接收返回值
                      {
                   case 'a': 
                        aflag = 1;
                        break;                                  
                   case 'l': 
                        lflag = 1;  
                        break;
                   default:  
                        printf("wrong option:%c\n",optopt);
                        return -1;
                      }
              }
          
                   if(optind == argc) //ls .没有带参直接ls当前目录,后面没有参数，默认就是访问当前目录
                     display_dir(".");
          
            for(i = optind; i < argc ; i++) //ls name1 name2....，从optind开始执行,执行后面的文件名字
           {
               if(stat(argv[i],&buf) < 0)//stat,判断的结果不成立，我们就直接返回-1，结束循环
              {
                 // perror("cannot access argv[i]!\n");
                  printf("cannot access %s!\n",argv[i]);
                  printf(": No such file or directory\n ");
                   return -1;
              }
               if(S_ISDIR(buf.st_mode))//是目录，判断此路径下有目录文件,里面的参数是st_mode
              {
                  printf("%s:\n",argv[i]);
                  display_dir(argv[i]);//就对其进行打印, 对后面的参数,答应argv[i]代表的目录
              }
              else//file，为文件
              {
              if(lflag)//ls -l file,只打印一个
              {
                display_file(argv[i],argv[i]);//显示文件信息
              }
                else// ls file
                {
                  printf("%s",argv[i]);//只答应文件名
                  printf("\n");
                } 
              }
           }
  
      return 0;
 }
          
              
