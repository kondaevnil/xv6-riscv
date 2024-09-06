#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int t;

    if (argc == 3) {
        if (link(argv[1], argv[2]) < 0)
            fprintf(2, "link %s %s: failed\n", argv[1], argv[2]);
    } else if (argc == 4 && strcmp(argv[1], "-s") == 0) {
        if ((t = symlink(argv[3], argv[2])) < 0)
            fprintf(2, "symlink creation error: %d\n", t);
    } else {
        fprintf(2, "Usage: ln old new \n ln -s old new\n");
        exit(1);
    }

  exit(0);
}
