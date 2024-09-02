#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/fcntl.h"
#include "user.h"

#define FNAME "OUT"

const uint64 a = 17, b = 97;
const uint64 m = 2000000011;

uint64
nxt(uint64 i)
{
    return (a * i + b) % m;
}

uint64
atoi64(const char *s)
{
    uint64 n;

    n = 0;
    while('0' <= *s && *s <= '9')
        n = n*10 + *s++ - '0';
    return n;
}

int
main(int argc, char *argv[])
{
    uint cnt;
    uint64 t, s;
    int F;

    if (argc != 3)
    {
        printf("not enough arguments\n");
        exit(1);
    }

    cnt = atoi64(argv[1]) / sizeof(uint64);
    t = atoi64(argv[2]);
    s = t;


    F = open(FNAME, O_CREATE | O_WRONLY);

    for (uint64 i = 0; i < cnt; i++) {
        if (write(F, &t, 8) != 8) {
            close(F);
            printf("Writing error\n");
            exit(2);
        }
        t = nxt(t);
    }

    close(F);

    F = open(FNAME, O_RDONLY);
    uint64 curr = s, value;

    while (cnt-- > 0) {
        if (read(F, &value, 8) != 8) {
            close(F);
            printf("Reading error\n");
            exit(-1);
        }

        if (value != curr) {
            close(F);
            printf("Incorrect value\n: %d %d", curr, value);
            exit(-2);
        }
        curr = nxt(curr);
    }

    close(F);
    printf("Everything is correct!\n");
    exit(0);
}