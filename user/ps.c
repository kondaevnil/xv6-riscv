#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/procinfo.h"
#include "user/user.h"

static char *names[] = {
        "UNUSED  ", "USED    ", "SLEEPING", "RUNNABLE", "RUNNING ", "ZOMBIE  "
};

int
main(int argc, char *argv[])
{
    struct procinfo *pi;
    int i, r = -1;
    int size = sizeof(struct procinfo) * 4;

    while (r == -1) {
        if ((pi = malloc(size)) == 0) {
            exit(1);
        }

        r = ps_listinfo(pi, size);

        if (r == 0) {
            break;
        } else if (r == -2) {
            free(pi);
            exit(1);
        }

        size *= 2;
        free(pi);
    }

    printf("\nname\tstate   \tparent\n");
    for (i = 0; i < r; i++) {
        printf("%s\t%s\t%d\n", pi[i].name, names[pi[i].state], pi[i].ppid);
    }

    exit(0);
}