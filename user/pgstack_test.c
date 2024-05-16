#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

// Потестить фрагмент со стеком достаточно трудно
// потому что при вызове функций переменные кладуться на стек
// и доступ снова обновляется, поэтому изменения можно отследить
// выделять размер большего размера -- не вариант, потому что размер стека - 1 страница

#define SIZE 100

int
main(int argc, char *argv[])
{
    int buff[SIZE];

    vmprint();
    int access = pgaccess((char *)buff, SIZE * sizeof(int));
    printf("Access before filling: %d\n", access);
    vmprint();

    for (int i = 0; i < SIZE; i++) {
        buff[i] = i;
    }

    vmprint();
    access = pgaccess((char *)buff, SIZE * sizeof(int));
    printf("Access after filling: %d\n", access);
    vmprint();

    access = pgaccess((char *)buff, SIZE * sizeof(int));
    printf("Access after calling checking: %d\n", access);
    vmprint();

    exit(0);
}