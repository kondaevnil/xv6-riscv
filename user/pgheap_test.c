#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


#define SIZE 100

int
main(int argc, char *argv[])
{
    int size = SIZE * sizeof(int);
    int *buff = malloc(size);

    vmprint();
    int access = pgaccess((char *)buff, size);
    printf("Access before filling: %d\n", access);
    vmprint();

    for (int i = 0; i < SIZE; i++) {
        buff[i] = i;
    }

    vmprint();
    access = pgaccess((char *)buff, size);
    printf("Access after filling: %d\n", access);
    vmprint();

    access = pgaccess((char *)buff, size);
    printf("Access after calling checking: %d\n", access);
    vmprint();

    exit(0);


}