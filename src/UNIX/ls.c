#include "apue.h"
#include <dirent.h>

int main(int argc, char const *argv[])
{
    DIR *dp;
    struct dirent *dirp;

    if (argc != 2) {
        ;
    }

    if ((dp = opendir(argv[1])) == NULL) {
        ;
    }
    while ((dirp = readdir(dp)) != NULL) {
        printf("%s\n", dirp->d_name);
    }

    closedir(dp);
    return 0;
}
