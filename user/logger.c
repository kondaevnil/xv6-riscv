#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// format
// logger <exec/swtch/intr/syscall> <0/1>
int
main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "<exec/swtch/intr/syscall> <0/1>\n");
        return -1;
    }

    char *event = argv[1];
    char *on_off = argv[2];
    int op = 0;

    if (strcmp(on_off, "1") == 0) {
        op = 1;
    } else if (strcmp(on_off, "0") == 0) {
        op = 0;
    } else {
        fprintf(2, "write '1' or '0'\n", on_off);
        exit(-2);
    }

    if (strcmp(event, "exec") == 0) {
        exit(logstart(3, op));
    } else if (strcmp(event, "intr") == 0) {
        exit(logstart(2, op));
    } else if (strcmp(event, "swtch") == 0) {
        exit(logstart(1, op));
    } else if (strcmp(event, "syscall") == 0) {
        exit(logstart(0, op));
    }

    fprintf(2, "(exec, swtch, intr, syscall)\n", event);
    exit(-3);
}