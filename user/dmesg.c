#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUFSIZE 1024

int
main(int argc, char *argv[])
{
    char *buf = (char *)malloc(BUFSIZE);

    if (buf == 0) {
        exit(1);
    }

    int r = dmesg(buf, BUFSIZE);

    if (r < 0) {
        fprintf(2, "Error occurred %d\n", r);
        free(buf);
        exit(1);
    }

    printf(buf);
    free(buf);
    exit(0);
}