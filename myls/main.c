#include"ls.h"
int main(int argc,char *argv[])
{
    int ch,i;
    struct stat buf;//用来记录文件的信息

      //opterr = 0;
      //解析命令
      //用来解析命令行参数命令,控制是否向STDERR打印错误。为0，则关闭打印
      //optind默认是1，调用一次getopt就会+1
      // 判断是否带有参数 
        judge_mode(argc,argv,ch,"liasrtR");
          
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
                if(Rflag)
                {
                  IsFile(argv[i]);
                }
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
                print(buf,argv[i]);
              } 
              }
           }
        printf("\n"); 
      return 0;
 }

