#ifndef BUFFER_H
#define BUFFER_H

#include "types.h"
#include "param.h"
#include "spinlock.h"

struct kernel_logs {
    struct spinlock lock;
    char *tail;
    char head[BUFPAGES * PHYSPAGESIZE];
};

#endif // BUFFER_H