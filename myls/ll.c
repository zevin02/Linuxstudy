#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<stdio.h>
#include<sys/stat.h>
void showinfo(char*path ,struct stat* buf)                                         
     {
        printf("%o %d %d %d %d %d %s",buf->st_mode,buf->st_nlink,buf->st_uid,buf->st_gid,buf->st_size,buf->st_mtime,path);
            
    }                                            
int main(int argc ,char* argv[])
{
if(argc==2)
{
    struct stat buf;
    if((stat(argv[1],&buf))!=-1)
    {
        showinfo(argv[1],&buf);
    }
}
return 0;
}
