#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 20

int
main(int argc, char **argv)
{
    int pid, status, i;
    int p[2];
    char *args[] = {"/wc", 0};

    pipe(p);
    pid = fork();

    if (pid > 0)
    {
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        exec("/wc", args);
        exit(0);
    }
    else if (pid == 0)
    {
        close(p[0]);
        for (i = 0; i < argc; i++)
        {
            write(p[1], argv[i], strlen(argv[i]));
            write(p[1], "\n", 1);
        }
        close(p[1]);
        wait(&status);
        exit(status);
    }

    close(p[0]);
    close(p[1]);
    fprintf(2, "Error in child process creation\n");
    exit(1);
}
