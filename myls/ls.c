#include "ls.h"
//打印颜色

int aflag = 0;
int lflag = 0; //作为标识符，如果aflag lflag为1则有-a和-l这个参数，执行选项
int iflag = 0;
int sflag = 0;
int rflag = 0;
int tflag = 0;
int Rflag = 0;
char filename[256][260];
long filetime[256];
void print(struct stat buf, char *filename)
{
  if (S_ISREG(buf.st_mode)) //一般文件
  {
    if (buf.st_mode & S_IXOTH) //可执行文件,打印成绿色
    {
      printf("\033[32m %s  \033[0m", filename);
    }
    else
    {

      printf("%s  ", filename); //直接接打印文件名,不显示文件
    }
  }
  else if (S_ISDIR(buf.st_mode)) //是一个目录打印蓝色
  {
    printf("\033[34m %s  \033[0m", filename);
  }
}
void display_rights(struct stat buf)
{

  if (buf.st_mode & S_IRUSR)
    printf("r");
  else
    printf("-");
  if (buf.st_mode & S_IWUSR)
    printf("w");
  else
    printf("-");
  if (buf.st_mode & S_IXUSR)
    printf("x");
  else
    printf("-");
  //组权限
  if (buf.st_mode & S_IRGRP)
    printf("r");
  else
    printf("-");
  if (buf.st_mode & S_IWGRP)
    printf("w");
  else
    printf("-");
  if (buf.st_mode & S_IXGRP)
    printf("x");
  else
    printf("-");
  //其他人权限
  if (buf.st_mode & S_IROTH)
    printf("r");
  else
    printf("-");
  if (buf.st_mode & S_IWOTH)
    printf("w");
  else
    printf("-");
  if (buf.st_mode & S_IXOTH)
    printf("x");
  else
    printf("-");
}

//打印文件类型
void display_mode(struct stat buf)
{

  switch (buf.st_mode & S_IFMT) //s_ifmt是一个子域掩码，按位与的结果来判断是什么权限
  {
  case S_IFSOCK:
    printf("s");
    break;
  case S_IFLNK:
    printf("l");
    break;
  case S_IFREG:
    printf("-");
    break;
  case S_IFBLK:
    printf("b");
    break;
  case S_IFDIR:
    printf("d");
    break;
  case S_IFCHR:
    printf("c");
    break;
  case S_IFIFO:
    printf("p");
    break;
  }
}

void display_sflag(char *fname, char *nname)
{

  struct stat buf; //buf用来接收文件的各种信息
  stat(fname, &buf);
  if (stat(fname, &buf) == -1) //路径存放失败
  {
    perror("stat error\n");
    return;
  }
  long long size = buf.st_size / 1024;
  if (size <= 4)
  {
    printf("4 ");
  }
  else
  {
    printf("%4lld ", size);
  }
  print(buf, nname);
}

void display_file(char *fname, char *nname) //fname里面存放的是目录的路径,显示文件的信息
{
  struct stat buf; //buf用来接收文件的各种信息
                   //   struct tm *t;//用来接收时间的参数，

  if (stat(fname, &buf) == -1) //路径存放失败
  {
    perror("stat error\n");
    return;
  }
  if (iflag) //有-i这个选项，就在首行打印
  {
    printf("%ld ", buf.st_ino);
  }

  if (sflag) //有-s这个选项
  {
    long long size = buf.st_size / 1024;
    if (size <= 4)
    {
      printf("%3d ", 4);
    }
    else
    {
      printf("%3lld ", size);
    }
  }

  //打印文件类型
  display_mode(buf);
  //打印权限
  // user other group
  display_rights(buf);
  printf(" %2ld ", buf.st_nlink); //硬链接数
                                  //打印用户名和所属组
  printf("%10s ", getpwuid(buf.st_uid)->pw_name);
  printf("%10s ", getgrgid(buf.st_gid)->gr_name);

  printf(" %8ld  ", buf.st_size); //所占的字节大小
  char *ctime();
  printf("%.12s  ", 4 + ctime(&(&buf)->st_mtime));
  print(buf, nname); //打印的同时显示颜色
  printf("\n");
  return;
}

//实现- i选项
void display_iflag(char *fname, char *nname)
{

  struct stat buf; //buf用来接收文件的各种信息
  stat(fname, &buf);
  if (stat(fname, &buf) == -1) //路径存放失败
  {
    perror("stat error\n");
    return;
  }
  printf("%ld  ", buf.st_ino);
  // printf("%5s  ",nname);
  print(buf, nname);
}
//获取文件的名字
void getfilename(char *dir, int *cnt)
{

  DIR *cntdir;
  struct dirent *cntitem;
  cntdir = opendir(dir);
  int len = 0;
  while ((cntitem = readdir(cntdir)) != NULL) //记录文件名，之后再进行文件名的字典序排序

  {
    strcpy(filename[*cnt], cntitem->d_name);
    len = strlen(filename[*cnt]);
    len = 0;
    (*cnt)++;
  }
}
//按照字典序排列,默认按升序排列，如果有-r的话就逆序排列
void sortbyletter(int cnt)
{
  char temp[260];
  int j = 0;
  int i = 0;
  for (i = 0; i < (cnt)-1; i++)
  {
    for (j = 0; j < (cnt)-1 - i; j++)
    {
      if (tflag)
      {
        if (filetime[j] < filetime[j + 1])
        {
          strcpy(temp, filename[j]);
          strcpy(filename[j], filename[j + 1]);
          strcpy(filename[j + 1], temp);
        }
      }
      if (rflag)
      {

        if (strcmp(filename[j], filename[j + 1]) < 0)
        {
          strcpy(temp, filename[j]);
          strcpy(filename[j], filename[j + 1]);
          strcpy(filename[j + 1], temp);
        }
      }
      else
      {

        if (strcmp(filename[j], filename[j + 1]) > 0)
        {
          strcpy(temp, filename[j]);
          strcpy(filename[j], filename[j + 1]);
          strcpy(filename[j + 1], temp);
        }
      }
    }
  }
}

void gettime(int cnt, char *dir) //获得时间的数字
{
  struct stat buf;
  char fname[256];
  int j = 0;
  sprintf(fname, "%s/%s", dir, filename[j]);
  stat(fname, &buf);
  for (j = 0; j < (cnt); j++)
  {

    filetime[j] = buf.st_mtime;
  }
}

void display_dir(char *dir) //显示目录下的所有文件，同时判断是否有-a选项，里面的是目录的路径名，有相对路径，也有绝对路径
{
  int i = 0;
  DIR *mydir;
  struct dirent *myitem;
  char fname[256];
  struct stat buf; //获取文件属性打印文件颜色
  if ((mydir = opendir(dir)) == NULL)
  {
    perror("fail to opendir!\n");
    return;
  }
  int cnt = 0;
  getfilename(dir, &cnt); //得到目录下文件的名字以及时间
  gettime(cnt, dir);
  sortbyletter(cnt);
  int j = 0;
  for (j = 0; j < cnt; j++)
  {
    //fname里面是目录的名字和文件的名字,全都弄到fname里面
    sprintf(fname, "%s/%s", dir, filename[j]); //dir这个目录的路径名字，文件名，这些名字全都答应到fname这个字符串里面来接收
    stat(fname, &buf);
    if (filename[j][0] == '.' && aflag == 0) //没有-a参数，如果if条件成立的就继续下一次循环，否则往下执行

    {
      continue; //在目录往下继续搜索，遇到隐藏文件就跳过
    }

    if (lflag) //走到这里就说明有-a参数，ls -l -a dir，同时也有-l参数
    {
      //srvprintf(fname,"%s/%s",dir,myitem->d_name);//dir这个目录的路径名字，文件名，这些名字全都答应到fname这个字符串里面来接收
      //    printf("%s",fname);

      display_file(fname, filename[j]); //把文件的秘密打印出来,第一个是目录的路径名，第二个是里面的文件名
    }

    else if (Rflag) //出现了-R选项
    {
      //display_Rflag(fname,filename[j],buf);//fname里面是路径名，filename[j]里面都是文件
    }
    else if (iflag) //出现了-i选项就执行
    {

      //sprintf(fname,"%s/%s",dir,myitem->d_name);//dir这个目录的路径名字，文件名，这些名字全都答应到fname这个字符串里面来接收
      display_iflag(fname, filename[j]);
      i++;
      if (i % 5 == 0)
      {
        printf("\n");
      }
    }
    else if (sflag)
    {
      display_sflag(fname, filename[j]);
    }
    else // ls -a dir,没有参数，只显示一个目录，
    {
      if (iflag)
      {
        display_iflag(fname, filename[j]);
      }
      else
      {
        //如果是目录的话就用蓝色
        //如果是可执行文件的话就用绿色
        print(buf, filename[j]);
      }
      //   printf("%5s  ",filename[j]);// 显示文件名，
    }
  }
  printf("\n");
  closedir(mydir);
}
//判断是否有带什么参数
void judge_mode(int argc, char *argv[], int ch, char *s)
{

  while ((ch = getopt(argc, argv, s)) != -1) //getopt用来解析命令行的参数，返回int，错误就返回-1,解析-a和-l两个命令,getopt处理-开头的参数
                                             //每次getopt后，这个索引指向argv里当前分析的字符串的下一个索引，因此,optind也就往后移动
                                             //argv[optind]就能得到下个字符串
                                             //
  {
    switch (ch) //用ch来接收返回值
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
      sflag = 1;
      break;

    case 'r': //递归显示文件，从根目录开始
      rflag = 1;
      break;
    case 't':
      tflag = 1;
      break;
    case 'R':
      Rflag = 1;
      break;
    default:
      printf("wrong option:%c\n", optopt);
      return;
    }
  }
}

void isFile(char *name, char *filename)
{
    char fname[PATH_MAX];
    int ret = 0;
    struct stat sb;
    ret = lstat(name, &sb);
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
    if(dp==NULL)
    {
      return;
    }
   //先收集名字再排序
    while (sdp = readdir(dp))

    {
      sprintf(fname, "%s/%s", name, sdp->d_name);
      lstat(fname, &sb);
      if (aflag == 0)
      {
        if (strcmp(sdp->d_name, ".") == 0 || strcmp(sdp->d_name, "..") == 0 || sdp->d_name[0] == '.')
        {
          continue;
        }
      }

      print(sb, sdp->d_name);
      //  printf("%s ",sdp->d_name);
    }
    printf("\n");
    //是真就是目录//要考虑目录下还有目录
    display_R(name);
    //
    closedir(dp);
  }
  else
  {
    return;
  } //
    //   printf("%s ",filename);//路径
}

void display_R(char *dir)
{
  char path[PATH_MAX];
  DIR *dp;
  struct dirent *sdp;
  dp = opendir(dir);
  if (dp == NULL)
  {
    perror("opendir error");
    return;
  }
  while (sdp = readdir(dp))
  {

    if (strcmp(sdp->d_name, ".") == 0 || strcmp(sdp->d_name, "..") == 0 || sdp->d_name[0] == '.')
    {
      continue;
    }

    //读这个目录项，继续判断是不是目录，如果是目录iu还得再进入，如果是文件就打印
    //    //ifFile(sdp->d_name);//不能直接这样，要绝对路径才可以
     if (strcmp(dir, "/") == 0)
      sprintf(path, "/%s", sdp->d_name);
    else    
    sprintf(path, "%s/%s", dir, sdp->d_name);
    isFile(path, sdp->d_name);
  }
  printf("\n");
  closedir(dp);
  return;
}
