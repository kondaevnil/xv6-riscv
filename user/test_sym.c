#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/param.h"
#include "kernel/fcntl.h"
#include "user.h"

void
create_link(char *filename, char *target)
{
    if (symlink(target, filename) != 0) {
        printf("Cant create link %s to file %s\n", filename, target);
        exit(1);
    }
}

void
create_file(char *fname, char *buf)
{
    int fd = open(fname, O_CREATE | O_WRONLY);
    if (fd < 0) {
        printf("Cant create file %s\n", fname);
        exit(1);
    }
    int n = strlen(buf);
    if (write(fd, buf, n) != n) {
        printf("Cant write content to file %s\n", fname);
        exit(1);
    }
    if (close(fd) != 0) {
        printf("Cant close file %s\n", fname);
        exit(1);
    }
}

void
check_link(char *linkname, int res)
{
    int fd = open(linkname, 0);

    if (res > 0 && fd >= 0) {
        printf("correct: %s\n", linkname);
        close(fd);
    } else if (res < 0 && fd < 0) {
        printf("incorrect: %s\n", linkname);
        close(fd);
    } else {
        close(fd);
        printf("failed fd: %d res: %d\n", fd, res);
        exit(1);
    }
}


int
main(int argc, char *argv[])
{
    create_link("to_wc_rel", "./wc");
    check_link("to_wc_rel", 1);

    create_link("to_wc_abs", "/wc");
    check_link("to_wc_abs", 1);

    mkdir("./dir1");
    mkdir("./dir1/dir2");
    mkdir("./dir1/dir2/dir3");
    create_file("./dir1/dir2/dir3/down_meow", "hi!!!");
    create_link("to_down_meow", "./dir1/dir2/dir3/down_meow");
    check_link("to_down_meow", 1);

    create_file("./up_meow", "hi!!!");
    create_link("./dir1/dir2/dir3/to_up_meow", "./../../../up_meow");
    check_link("./dir1/dir2/dir3/to_up_meow", 1);

    create_file("./Wow", "WOW!!!");
    create_link("./dir1/dir2/dir3/to_wow", "/Wow");
    create_link("./to_to_wow", "/dir1/dir2/dir3/to_wow");
    check_link("./to_to_wow", 1);

    create_file("./Ahh", "Ahh!!!");
    create_link("./dir1/dir2/dir3/to_ahh", "../../../Ahh");
    create_link("./to_to_ahh", "./dir1/dir2/dir3/to_ahh");
    check_link("./to_to_ahh", 1);

    create_link("./to_self", "./to_self");
    check_link("./to_self", -1);

    create_link("./to_other", "./dir1/to_other");
    create_link("./dir1/to_other", "./../to_other");
    check_link("./to_other", -1);
    check_link("./dir1/to_other", -1);

    create_link("./to_void", "/void");
    check_link("./to_void", -1);

    create_link("./dir1/to_void2", "./to_void");
    check_link("./dir1/to_void2", -1);

    create_link("./a", "./down_meow");
    check_link("./a", -1);

    create_link("./dir1/dir2/dir3/b", "./up_meow");
    check_link("./dir1/dir2/dir3/b", -1);



    exit(0);
}