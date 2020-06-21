/*
 * Our own header, to be included before all standard system headers
 */

#ifndef _APUE_H
#define _APUE_H

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
/*
 * Default file access permissions
 */ 
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char *path_alloc(size_t *);

void err_sys(const char *, ...) __attribute__((noreturn));
void err_msg(const char *, ...);
void err_ret(const char *, ...);
void err_quit(const char *, ...) __attribute__((noreturn));

#endif // _APUE_H