#include "apue.h"
#include <errno.h>
#include <limits.h>

#ifdef PATH_MAX
static long pathmax = PATH_MAX;
#else
static long pahtmax = 0;
#endif

#define PATH_MAX_GUESS 1024

char *path_alloc(size_t *sizep)
{
    char *ptr;
    size_t size;

    if (pathmax == 0) {
        errno = 0;
        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
            if (errno == 0)
                pathmax = PATH_MAX_GUESS;
            else 
                err_sys("pathconf error for _PC_PATH_MAX");
        } else {
            pathmax++;
        }
    }

    size = pathmax;
    if ((ptr = malloc(size)) == NULL)
        err_sys("malloc error for pathname");
    if (sizep != NULL)
        *sizep = size;
    return ptr;
}