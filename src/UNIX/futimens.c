#include "apue.h"
#include <fcntl.h>

/*
 * 在不更改其访问时间及修改时间的情况下，
 * 首先用stat函数得到这些时间
 * 再使用futimens重置时间
 */ 

int main(int argc, char const *argv[])
{
    int i, fd;
    struct stat statbuf;
    struct timespec times[2];
    for(i = 1; i < argc; i++) {
        if (stat(argv[i], &statbuf) < 0) {
            err_ret("%s: stat error", argv[i]);
            continue;
        }
        if ((fd = open(argv[i], O_RDWR)) < 0) {
            err_ret("%s: open error", argv[i]);
            continue;
        }
        times[0] = statbuf.st_atim;
        times[1] = statbuf.st_mtim;
        if (futimens(fd, times) < 0)
            err_ret("%s: futimens error", argv[i]);
        close(fd);
    }
    return 0;
}
