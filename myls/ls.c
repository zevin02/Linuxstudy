#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include<string.h>
int aflag = 0,lflag = 0;//作为标识符，如果aflag lflag为1则有-a和-l这个参数，执行选项
int iflag=0;
int sflag=0;
typedef struct filenode
{
  char rights[12];
  int nlink;
  char username[15];
  char groupname[15];
  int size;
  char filetime[30];
  char filename;
  
}filenode;



void display_rights(struct stat buf)
{
  
      if(buf.st_mode & S_IRUSR)
				printf("r");
			else
				printf("-");
			if(buf.st_mode & S_IWUSR)
				printf("w");
			else
				printf("-");
			if(buf.st_mode & S_IXUSR)
				printf("x");
			else
				printf("-");
      //组权限
			if(buf.st_mode & S_IRGRP)
				printf("r");
			else
				printf("-");
			if(buf.st_mode & S_IWGRP)
				printf("w");
			else
				printf("-");
			if(buf.st_mode & S_IXGRP)
				printf("x");
			else
				printf("-");
     //其他人权限
			if(buf.st_mode & S_IROTH)
				printf("r");
			else
				printf("-");
			if(buf.st_mode & S_IWOTH)
				printf("w");
			else			
				printf("-");
			if(buf.st_mode & S_IXOTH)
				printf("x");
			else
				printf("-");
}


//打印文件类型
void display_mode(struct stat buf)
{
  
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
}


void display_sflag(char*fname,char*nname)
{

    struct stat buf;//buf用来接收文件的各种信息
    stat(fname,&buf);
    if(stat(fname,&buf) ==-1)//路径存放失败
    {
       perror("stat error\n");
       return ;                 
    }
   long long size= buf.st_size/1024;
   if(size<=4)
   {
     printf("4 ");
   }
   else 
   {
     printf("%4lld ",size);
   }
      printf("%s  ",nname);
}

void display_file(char *fname , char *nname)//fname里面存放的是目录的路径,显示文件的信息
{
            struct stat buf;//buf用来接收文件的各种信息
            //   struct tm *t;//用来接收时间的参数，

           if(stat(fname,&buf) ==-1)//路径存放失败
          {
           perror("stat error\n");
           return ;                 
          }
          if(iflag)//有-i这个选项，就在首行打印
          {
            printf("%ld ",buf.st_ino);
          }

          if(sflag)//有-s这个选项
          {
   long long size= buf.st_size/1024;
   if(size<=4)
   {
     printf("%3d ",4);
   }
   else 
   {
     printf("%3lld ",size);
   }
          }

    //打印文件类型
           display_mode(buf);
            //打印权限
           // user other group
            display_rights(buf);
            printf(" %2ld ",buf.st_nlink);//硬链接数

            printf("%10s ",getpwuid(buf.st_uid)->pw_name);
            printf("%10s ",getgrgid(buf.st_gid)->gr_name);

            printf(" %8ld  ",buf.st_size);//所占的字节大小
               // char buf_time[32];
                //strcpy(buf_time,ctime(&buf.st_mtime));
                
              //  buf_time[strlen(buf_time)-1] = '\0'; //去掉换行符
               // printf(" %s",buf_time);             //打印文件的时间信息
               // t = localtime(&buf.st_mtime);//用t来接收时间
            char *ctime();
            printf("%.12s  ",4+ctime(&(&buf) -> st_mtime));
               // printf("%d-%02d-%02d  %02d:%02d ",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min);
            printf("%s\n",nname);//文件名
      return ;

}







void display_iflag(char*fname,char*nname)
{

    struct stat buf;//buf用来接收文件的各种信息
    stat(fname,&buf);
    if(stat(fname,&buf) ==-1)//路径存放失败
    {
       perror("stat error\n");
       return ;                 
    }
      printf("%ld  ",buf.st_ino);
      printf("%5s  ",nname);
}

void display_dir(char *dir)//显示目录下的所有文件，同时判断是否有-a选项，里面的是目录的路径名，有相对路径，也有绝对路径
{
    int i=0;
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
          
        //fname里面是目录的名字和文件的名字,全都弄到fname里面
        
           sprintf(fname,"%s/%s",dir,myitem->d_name);//dir这个目录的路径名字，文件名，这些名字全都答应到fname这个字符串里面来接收
           if(myitem->d_name[0] == '.' && aflag==0)//没有-a参数，如果if条件成立的就继续下一次循环，否则往下执行
         {
           continue;//在目录往下继续搜索，遇到隐藏文件就跳过
         }


          if(lflag)//走到这里就说明有-a参数，ls -l -a dir，同时也有-l参数
        {
           //srvprintf(fname,"%s/%s",dir,myitem->d_name);//dir这个目录的路径名字，文件名，这些名字全都答应到fname这个字符串里面来接收
       //    printf("%s",fname);
      
            display_file(fname,myitem->d_name);//把文件的秘密打印出来,第一个是目录的路径名，第二个是里面的文件名                                      
        }


          else if(iflag)//出现了-i选项就执行
          {

           //sprintf(fname,"%s/%s",dir,myitem->d_name);//dir这个目录的路径名字，文件名，这些名字全都答应到fname这个字符串里面来接收
            display_iflag(fname,myitem->d_name);
            i++;
            if(i%5==0)
            {
              printf("\n");
            }
          }
          else if(sflag)
          {
            display_sflag(fname,myitem->d_name);
          }
          else // ls -a dir,没有参数，只显示一个目录，
          { 
            if(iflag)
            {
            display_iflag(fname,myitem->d_name);
            }
            else 
            {

            printf("%5s  ",myitem->d_name);// 显示文件名，
            }
          }


      }
         printf("\n");
         closedir(mydir);

}
//判断是否有带什么参数
void judge_mode(int argc,char*argv[],int ch,char *s)
{

       while((ch = getopt(argc,argv,s)) != -1)//getopt用来解析命令行的参数，返回int，错误就返回-1,解析-a和-l两个命令,getopt处理-开头的参数
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
                   case 'i': 
                        iflag = 1;  
                        break;
                   case 's':
                        sflag=  1;
                        break;
                   default:  
                        printf("wrong option:%c\n",optopt);
                        return ;
                      }
          }
}



int main(int argc,char *argv[])
{
    int ch,i;
    struct stat buf;//用来记录文件的信息

      //opterr = 0;
          //解析命令
          //用来解析命令行参数命令,控制是否向STDERR打印错误。为0，则关闭打印
          //optind默认是1，调用一次getopt就会+1
          //  
        judge_mode(argc,argv,ch,"lias");
          
                  // 没有带参直接ls当前目录,后面没有参数，默认就是访问当前目录
       if(argc==1||*argv[argc-1]=='-')             
          display_dir(".");
          
            for(i = optind; i < argc ; i++) //ls name1 name2....，从optind开始执行,执行后面的文件名字
           {
               if(stat(argv[i],&buf)==-1)//stat,判断的结果不成立，我们就直接返回-1，结束循环
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
                printf("%s",argv[i]);//只打印文件名
                printf("\n");
              } 
              }
           }
  
      return 0;
 }

