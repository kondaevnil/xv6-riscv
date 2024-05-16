#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>

const int buf_size = BUFPAGES * PHYSPAGESIZE;

int
main(int argc, char *argv[])
{
    char *buf = malloc(buf_size);

    if (buf == NULL)
    {
        fprintf(2, "Memory allocation error\n");
        exit(-1);
    }

    int copied = dmesg(buf, buf_size);

    if (copied == 0)
    {
        fprintf(2, "Copying data error\n");
        exit(-2);
    }

    printf(buf);
    free(buf);

    exit(0);
}