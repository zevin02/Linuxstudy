
#ifndef _LS_H_
#define _LS_H_
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include<string.h>
#include<stdlib.h>

extern int aflag;
extern int lflag;//作为标识符，如果aflag lflag为1则有-a和-l这个参数，执行选项
extern int iflag;
extern int sflag;
extern int rflag;
extern int tflag;
extern int Rflag;
extern char filename[256][260];
extern long filetime[256];

void IsFile(char* dir);
void dp_R(char*dir);
void print(struct stat buf,char* filename);
void display_rights(struct stat buf);
void display_mode(struct stat buf);
void display_sflag(char*fname,char*nname);
void display_file(char *fname , char *nname);//fname里面存放的是目录的路径,显示文件的信息
void display_iflag(char*fname,char*nname);
void getfilename(char* dir,int *cnt);
void sortbyletter(int cnt);
void gettime(int cnt,char* dir);//获得时间的数字
void display_dir(char *dir);//显示目录下的所有文件，同时判断是否有-a选项，里面的是目录的路径名，有相对路径，也有绝对路径
void judge_mode(int argc,char*argv[],int ch,char *s);
void IsFile(char* dir);
void dp_R(char*dir);
#endif
