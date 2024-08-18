#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        exit(1);
    }

    int ppc[2], pcp[2];
    int len = strlen(argv[1]);
    int mutex = get_mutex();

    pipe(ppc);
    pipe(pcp);

    int pid = fork();
    if (pid == 0) {
        close(ppc[1]);
        close(pcp[0]);

        char ch;
        while (read(ppc[0], &ch, 1) == 1) {
            acquire_mutex(mutex);
            printf("%d: received %c\n", getpid(), ch);
            release_mutex(mutex);
            write(pcp[1], &ch, 1);
        }
        close(pcp[1]);
        close(ppc[0]);
        free_mutex(mutex);
        exit(0);
    } else if (pid > 0) {
        close(ppc[0]);
        close(pcp[1]);

        write(ppc[1], argv[1], len);
        close(ppc[1]);

        char ch;
        while(read(pcp[0], &ch, 1) == 1) {
            acquire_mutex(mutex);
            printf("%d: received %c\n", getpid(), ch);
            release_mutex(mutex);
        }
        close(pcp[0]);
        free_mutex(mutex);
        exit(0);
    }

    exit(0);
}