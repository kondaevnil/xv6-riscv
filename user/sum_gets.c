#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 23
#define MAX_STR_LEN 24

char buf[BUF_SIZE];

int
main(int argc, char *argv[])
{
    int a, b;
    uint len, cnt = 0;
    char *tmp = buf;

    gets(buf, BUF_SIZE);
    len = strlen(buf);

    if (len >= MAX_STR_LEN)
    {
        printf("Length limit\n");
        exit(0);
    }

    a = atoi(tmp);
    while ('0' <= *tmp && *tmp <= '9')
        tmp++, cnt++;

    if (cnt > 10)
    {
        printf("Wrong number length\n");
        exit(0);
    }

    if (*tmp != ' ')
    {
        printf("No space between nums\n");
        exit(0);
    }
    tmp++;
    cnt++;

    b = atoi(tmp);
    while ('0' <= *tmp && *tmp <= '9')
        tmp++, cnt++;

    if (cnt > 21)
    {
        printf("Wrong number length\n");
        exit(0);
    }

    if (*tmp == '\n' || *tmp == '\r')
        cnt++;

    if (cnt != len)
    {
        printf("Wrong string\n");
        exit(0);
    }

    printf("%d\n", a + b);

    exit(0);
}
