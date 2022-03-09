#include<iostream>
#include<sys/types.h>
#include<dirent.h>
using namespace std;

void test_opendir()
{
    DIR* a=opendir("~/githubgovern");
    cout<<a<<endl;
}

int main()
{
    test_opendir();
    return 0;
}
