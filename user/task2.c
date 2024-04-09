#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 20

int
main(int argc, char **argv)
{
    int pid, status, i;
    int p[2], cnt;
    char buf[BUF_SIZE];
    int str_len;

    pipe(p);
    pid = fork();

    if (pid == 0)
    {
        close(p[1]);

        while ((cnt = read(p[0], buf, BUF_SIZE)) > 0)
        {
            if (write(0, buf, cnt) != cnt)
            {
                printf("writing error\n");
                close(p[0]);
                exit(1);
            }
        }

        close(p[0]);
        exit(0);
    }
    else if (pid > 0)
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
