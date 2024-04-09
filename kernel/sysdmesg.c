#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "memlayout.h"

uint64
sys_dmesg(void)
{
    uint64 buf;
    int size;
    int copied;

    argaddr(0, &buf);
    argint(1, &size);

    copied = copy_buffer((char *)buf, size);
    return copied > 0 ? copied : 0;
}