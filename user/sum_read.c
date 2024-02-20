#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_LEN 11
char buf[NUM_LEN];

int
main(int argc, char *argv[])
{
    uint64 a, b;
    int i, cc;
    char c;

    for (i = 0; i < NUM_LEN; i++)
    {
        cc = read(0, &c, 1);

        if (cc < 1)
        {
            printf("Error while reading\n");
            exit(0);
        }

        if (!('0' <= c && c <= '9'))
        {
            if (c != ' ')
            {
                printf("Invalid character between numbers: %c", c);
                exit(0);
            }

            break;
        }

        buf[i] = c;
    }

    if (c != ' ')
    {
        printf("Max number length was reached\n");
        exit(0);
    }

    buf[i] = '\0';
    a = atoi(buf);

    for (i = 0; i < NUM_LEN; i++)
    {
        cc = read(0, &c, 1);

        if (cc == 0)
            break;

        if (!('0' <= c && c <= '9'))
        {
            if (c != '\r' && c != '\n')
            {
                printf("Invalid character between numbers: %c", c);
                exit(0);
            }

            break;
        }

        buf[i] = c;
    }

    if (c != '\r' && c != '\n' && i == 11)
    {
        printf("Max number length was reached\n");
        exit(0);
    }

    buf[i] = '\0';
    b = atoi(buf);

    printf("%l\n", a + b);

    exit(0);
}
