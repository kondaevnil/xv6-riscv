#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_dmesg(void)
{
    int sz;
    uint64 buf;

    argaddr(0, &buf);
    argint(1, &sz);

    return dmesg((char *)buf, sz);
}