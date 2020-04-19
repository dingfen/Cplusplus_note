/*
 * pread pwrite 允许原子性的定位并执行IO
 * 调用pread相当于lseek后调用read 但不更新当前文件偏移量
 * pwrite相当于lseek后调用write
 * 
 * dup  返回的新文件描述符 一定是可用文件描述符的最小值
 * dup2 可以指定一个文件描述符
 */ 

#include "apue.h"
#include <fcntl.h>


void set_fl(int fd, int flags)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");
    val |= flags;   // flags turns on 

    if ((val = fcntl(fd, F_SETFL, val)) < 0)
        err_sys("fcntl F_SETFL error");
}

void clr_fl(int fd, int flags)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");
    val &= ~flags;   // flags turns off 

    if ((val = fcntl(fd, F_SETFL, val)) < 0)
        err_sys("fcntl F_SETFL error");
}

int main(int argc, char const *argv[])
{
    int val;
    if (argc != 2)
        exit(1);
    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
        err_sys("fcntl error for fd %d", atoi(argv[1]));
    
    switch (val & O_ACCMODE)
    {
    case O_RDONLY:
        printf("read only");
        break;
    
    case O_WRONLY:
        printf("write only");
        break;
    
    case O_RDWR:
        printf("read write");
        break;
    
    default:
        break;
    }

    if (val & O_APPEND)
        printf(", append");
    if (val & O_NONBLOCK)
        printf(", nonblocking");
    if (val & O_SYNC)
        printf(", synchronous writes");

    putchar('\n');
    return 0;
}
