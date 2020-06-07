#include "apue.h"
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    if (open("temp", O_RDWR) < 0)
        perror("open error");
    if (unlink("temp") < 0)
        perror("unlink error");
    printf("file unlinked\n");
    sleep(15);
    printf("Done\n");
    return 0;
}
