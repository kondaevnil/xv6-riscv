#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include <stddef.h>

const int buf_size = PHYSPAGESIZE * BUFPAGES;

int
main(int argc, char *argv[])
{
    int status;
    char *args[] = {"/echo", "Hello!"};
    int pid = fork();

    if (pid == 0)
    {
        int res = exec("/echo", args);
        if (res == -1)
        {
            fprintf(2, "Echo error");
            exit(1);
        }

        exit(0);
    }
    else if (pid > 0)
    {
        wait(&status);

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

    exit(-1);
}