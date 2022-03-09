
#include<sys/types.h>
#include<dirent.h>
#include<stdio.h>
int main(int argc,char *argv[])
{
    DIR* dir_ptr;
    struct dirent * direntp;
    dir_ptr=opendir(argv[1]);
    while(direntp=readdir(dir_ptr))
    {
        printf("s\n ",direntp->d_name);
    }
    closedir(dir_ptr);
    return 0;
}
