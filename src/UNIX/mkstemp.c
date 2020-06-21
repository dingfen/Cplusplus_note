// tmpnam is dangerous
// use mkstemp instead
#include "apue.h"
#include <errno.h>

void make_temp(char *template)
{
    int fd;
    struct stat buf;

    if ((fd = mkstemp(template)) < 0)
        err_sys("can't create temp file");
    printf("temp name = %s\n", template);
    close(fd);
    if (stat(template, &buf) < 0) {
        if (errno == ENOENT)
            printf("file doesn't exist\n");
        else
            err_sys("stat failed");
    } else {
        printf("file exists\n");
        unlink(template);
    }
}

int main(int argc, char const *argv[])
{
    char good_template[] = "/tmp/XXXXXX";
    char *bad_template   = "/tmp/XXXXXX"; // wrong

    printf("Trying to create good temp file...\n");
    make_temp(good_template);
    printf("Trying to create bad temp file...\n");
    make_temp(bad_template);
    return 0;
}
