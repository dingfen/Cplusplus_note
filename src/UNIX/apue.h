/*
 * Our own header, to be included before all standard system headers
 */

#ifndef _APUE_H
#define _APUE_H

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

/*
 * Default file access permissions
 */ 
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


void err_sys(const char *, ...);

#endif // _APUE_H