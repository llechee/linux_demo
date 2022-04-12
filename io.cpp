//IO函数测试
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
using namespace std;
void func()
{
    //int fd = open("/home/lz/usr/linux_demo/io.txt",O_RDWR | O_CREAT,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IXOTH); //读，写打开 若文件不存在，则创建它，需要使用 mode 选项。来指明新文件的访问权限 
    int fd = open("/home/lz/usr/linux_demo/io.txt",O_RDWR | O_CREAT,0755);

    if(fd>=0) //
    {
        write(fd,"lz xy",5);
        flock(fd,LOCK_EX);
        sleep(6);
        flock(fd,LOCK_UN);
        close(fd);
    }
    else cout<<strerror(errno)<<endl;
}

int main()
{

    func();
    return 0;
}