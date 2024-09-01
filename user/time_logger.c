#include "../kernel/types.h"
#include "../kernel/param.h"
#include "user.h"

// format
// <(exec, swtch, intr, syscall)> <number of ticks>
int
main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(2, "<(exec, swtch, intr, syscall)> <number of ticks>\n");
        return -1;
    }

    char *event = argv[1];
    int timer = atoi(argv[2]);

    if (strcmp(event, "exec") == 0) {
        exit(logint(3, timer));
    } else if (strcmp(event, "intr") == 0) {
        exit(logint(2, timer));
    } else if (strcmp(event, "swtch") == 0) {
        exit(logint(1, timer));
    } else if (strcmp(event, "syscall") == 0) {
        exit(logint(0, timer));
    }

    exit(-3);
}