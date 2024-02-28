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
            fprintf(2, "Error while reading\n");
            exit(-1);
        }

        if (!('0' <= c && c <= '9'))
        {
            if (c != ' ')
            {
                fprintf(2, "Invalid character between numbers: %c\n", c);
                exit(-1);
            }

            break;
        }

        buf[i] = c;
    }

    if (c != ' ')
    {
        fprintf(2, "Max number length was reached\n");
        exit(-1);
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
                fprintf(2, "Invalid character between numbers: %c\n", c);
                exit(-1);
            }

            break;
        }

        buf[i] = c;
    }

    if (c != '\r' && c != '\n' && i == 11)
    {
        fprintf(2, "Max number length was reached\n");
        exit(-1);
    }

    buf[i] = '\0';
    b = atoi(buf);

    printf("%l\n", a + b);

    exit(0);
}
