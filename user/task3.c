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
    int str_len;

    pipe(p);
    pid = fork();

    if (pid > 0)
    {
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);

        int res = exec("/wc", args);
        if (res == -1)
        {
            printf("exec error\n");
            exit(1);
        }

        exit(0);
    }
    else if (pid == 0)
    {
        close(p[0]);
        for (i = 0; i < argc; i++)
        {
            str_len = strlen(argv[i]);
            if (write(p[1], argv[i], str_len) != str_len || write(p[1], "\n", 1) != 1)
            {
                printf("writing error\n");
                close(p[1]);
                exit(1);
            }
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
