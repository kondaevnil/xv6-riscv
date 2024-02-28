#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 23
#define MAX_STR_LEN 22

char buf[BUF_SIZE];

int
main(int argc, char *argv[])
{
    uint64 a, b;
    uint len, cnt = 0;
    char *tmp = buf;

    gets(buf, BUF_SIZE);
    len = strlen(buf);

    if (len > MAX_STR_LEN)
    {
        fprintf(2, "Length limit\n");
        exit(-1);
    }

    a = atoi(tmp);
    while ('0' <= *tmp && *tmp <= '9')
        tmp++, cnt++;

    if (cnt > 10)
    {
        fprintf(2, "Wrong number length\n");
        exit(-1);
    }

    if (*tmp != ' ')
    {
        fprintf(2, "No space between nums\n");
        exit(-1);
    }
    tmp++;
    cnt++;

    b = atoi(tmp);
    while ('0' <= *tmp && *tmp <= '9')
        tmp++, cnt++;

    if (cnt > 21)
    {
        fprintf(2, "Wrong number length\n");
        exit(-1);
    }

    if (*tmp == '\n' || *tmp == '\r')
        cnt++;

    if (cnt != len)
    {
        fprintf(2, "Wrong string\n");
        exit(-1);
    }

    printf("%l\n", a + b);

    exit(0);
}
