#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
    int pid = fork();
    int status;

    if (pid == 0)
    {
        sleep(30);
        exit(1);
    }
    else if (pid > 0)
    {
        printf("parent id: %d\n", getpid());
        printf("child id: %d\n", pid);

        if (kill(pid) == -1)
            exit(-1);

        pid = wait(&status);
        printf("child %d is done with %d status code\n", pid, status);
        exit(0);
    }

    fprintf(2, "Error in child process creation\n");
    exit(1);
}
