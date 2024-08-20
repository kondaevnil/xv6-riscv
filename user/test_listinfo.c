#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/procinfo.h"
#include "user/user.h"

#define BSIZE 3

void small_buffer_size(void) {
    struct procinfo pi[BSIZE];
    int r;

    for (int i = 0; i <= BSIZE; i++) {
        r = ps_listinfo(pi, i);

        switch (r) {
            case -1:
                printf("small buffer size\n");
                break;
            case -2:
                printf("error during copying\n");
                exit(1);
            default:
                printf("\n");
                for (int j = 0; j < r; j++) {
                    printf("ppid: %d  name:  %s\n", pi[j].ppid, pi[j].name);
                }
                break;
        }
    }
}

void invalid_address(void) {
    int t;
    int r = ps_listinfo((struct procinfo *)(&t + 256), 10);

    switch (r) {
        case -1:
            printf("small buffer size\n");
            break;
        case -2:
            printf("error during copying\n");
            break;
        default:
            printf("invalid situation\n");
            exit(1);
    }
}


int
main(int argc, char *argv[])
{
    if (argc != 2) {
        exit(1);
    }

    int n = atoi(argv[1]);
    // 1 -- small_buffer_size
    // 2 -- invalid_address
    // 3 --
    switch (n) {
        case 1:
            small_buffer_size();
            break;
        case 2:
            invalid_address();
            break;
        default:
            break;
    }

    exit(0);
}