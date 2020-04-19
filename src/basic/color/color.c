#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define HIGHLIGHT 1
#define LOWLIGHT 2
#define ITALIC 3
#define UNDERLINE 4
#define REVERSE 7
#define INVISIBLE 8

#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define PURPLE 35
#define DARKGREEN 36
#define WHITE 37

#define BACKGROUND 10


void p(char *fmt, ...)
{
    va_list ap;
    char buf[1024];
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    printf("%s", buf);
    va_end(ap);
}

int main()
{
    char fmt[128];
    sprintf(fmt, "\033[%dm%%s\n\033[0m", YELLOW);
    p(fmt, "hello");

    sprintf(fmt, "\033[%d;%dm%%s\n\033[0m", GREEN+BACKGROUND, RED);
    p(fmt, "world");

    sprintf(fmt, "\033[%d;%dm%%s\n\033[0m", HIGHLIGHT, BLUE);
    p(fmt, "Look at me");

    sprintf(fmt, "\033[%d;%dm%%s\n\033[0m", UNDERLINE, PURPLE);
    p(fmt, "under line it now");

    sprintf(fmt, "\033[%d;%dm%%s\n\033[0m", LOWLIGHT, RED);
    p(fmt, "i dont know");

    sprintf(fmt, "\033[%d;%dm%%s\n\033[0m", ITALIC, WHITE);
    p(fmt, "what is it");

    sprintf(fmt, "\033[%d;%dm%%s\n\033[0m", REVERSE, WHITE);
    p(fmt, "it is lightening");

    sprintf(fmt, "\033[%d;%dm%%s\n\033[0m", INVISIBLE, WHITE);
    p(fmt, "Wow amazing");

    // printf("\033[2J");
    sprintf(fmt, "\033[%d;%dH\033[%dm%%s\n\033[0m", 15, 40, DARKGREEN);
    p(fmt, "Me Again");
}
