/*
 * 遍历文件层次结构的小程序
 * 得到各种类型文件的计数
 */

#include "apue.h"
#include <dirent.h>
#include <limits.h>

typedef int MyFunc(const char *, const struct stat *, int);

static MyFunc myfunc;
static int myftw(char *, MyFunc *);
static int dopath(MyFunc *);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int main(int argc, char const *argv[])
{
    int ret;
    if (argc != 2)
        err_quit("usage: ftw <starting-pathname>");
    ret = myftw(argv[1], myfunc);
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0)
        ntot = 1;
    
    printf("regular files = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
    printf("directories = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
    printf("block special = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
    printf("char special = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
    printf("FIFOs = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
    printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
    printf("sockets = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);

    return 0;
}


#define FTW_F   1     // file other than directory
#define FTW_D   2     // directory
#define FTW_DNR 3     // directory can't be read
#define FTW_NS  4     // file can't be stat

static char *fullpath;
static size_t pathlen;

static int myftw(char *pathname, MyFunc *func)
{
    fullpath = path_alloc(&pathlen);
    size_t len = strlen(pathname);
    if (pathlen <= len) {
        pathlen = len * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
            err_sys("realloc failed");
        }
    }
    strcpy(fullpath, pathname);
    return dopath(func);
}

static int dopath(MyFunc* func) 
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR  *dp;
    int ret, n;
    if (lstat(fullpath, &statbuf) < 0)  // can't stat
        return func(fullpath, &statbuf, FTW_NS);
    if (S_ISDIR(statbuf.st_mode) == 0)  // not a directory
        return func(fullpath, &statbuf, FTW_F);
    // is a directory
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return 0;
    n = strlen(fullpath);
    if (n + NAME_MAX + 2 > pathlen) {
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            err_sys("realloc failed");
    }
    fullpath[n++] = '/';
    fullpath[n] == 0;
    if ((dp = opendir(fullpath)) == NULL)   // can't read dir
        return func(fullpath, &statbuf, FTW_DNR);
    while ((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") == 0 
            || strcmp(dirp->d_name, "..") == 0)
            continue;
        strcpy(&fullpath[n], dirp->d_name);
        if ((ret = dopath(func)) != 0)
            break;
    }
    fullpath[n-1] = 0;
    if (closedir(dp) < 0)
        err_ret("can't close directory %s", fullpath);
    return ret;
}


static int myfunc(const char* pathname, const struct stat *statptr, int type)
{
    switch (type)
    {
    case FTW_F:
        switch (statptr->st_mode & S_IFMT)
        {
        case S_IFREG: nreg++; break;
        case S_IFBLK: nblk++; break;
        case S_IFCHR: nchr++; break;
        case S_IFIFO: nfifo++; break;
        case S_IFLNK: nslink++; break;
        case S_IFSOCK:nsock++; break;
        }
        break;
    case FTW_D:
        ndir++;
        break;
    case FTW_DNR:
        err_ret("can't read directory %s", pathname);
        break;
    case FTW_NS:
        err_ret("stat error for %s", pathname);
        break;
    default:
        break;
    }
    return 0;
}
