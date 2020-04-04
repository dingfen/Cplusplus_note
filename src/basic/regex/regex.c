/*
 * regex 正则表达式
 * 识别用户输入的是否是格式正确的IP地址
 * 或者是电子邮件等
 */

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc != 3 && strcmp(argv[1],"-p") && strcmp(argv[1],"-m")) {
        fprintf(stderr, "Usage: ./regex [-p|-m] string.\n");
        exit(EXIT_FAILURE);
    }
    const char *ippattern 
        = "^(([0-9]|[1-9][0-9]|1([0-9][0-9])|2([0-4][0-9]|5[0-5]))\\.){3}([0-9]|[1-9][0-9]|1([0-9][0-9])|2([0-4][0-9]|5[0-5]))$";
    const char *mailpattern = "^([a-zA-Z0-9_\\-\\.]+)@([a-zA-Z0-9_\\-\\.]+)((\\.[a-zA-Z0-9_\\-\\.]+)+)$";
    size_t nmatch = 4;
    regmatch_t pmatch[4];
    regex_t compile;
    int rc;
    if (strcmp(argv[1], "-p") ==0) {
        if ((rc = regcomp(&compile, ippattern, REG_EXTENDED))!=0) {
            printf("regcomp() failed, returning nonzero (%d)\n", rc);
            exit(EXIT_FAILURE);
        }
        if ((rc = regexec(&compile, argv[2], nmatch, pmatch, 0))==REG_NOMATCH) {
            printf("Failed to match '%s' with '%s',returning %d.\n",
            argv[2], ippattern, rc);
        }
        else {
            printf("Yes, it is a IP address.\n");
        }
    }
    if (strcmp(argv[1], "-m") ==0) {
        if ((rc = regcomp(&compile, mailpattern, REG_EXTENDED))==REG_NOMATCH) {
            printf("regcomp() failed, returning nonzero (%d)\n", rc);
            exit(EXIT_FAILURE);
        }
        if ((rc = regexec(&compile, argv[2], nmatch, pmatch, 0))!=0) {
            printf("Failed to match '%s' with '%s',returning %d.\n",
            argv[2], mailpattern, rc);
        }
        else {
            printf("Yes, it is an email address.\n");
        }
    }
    regfree(&compile);
    return 0;
}

