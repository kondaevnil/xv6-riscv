#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "logger.h"

uint64
sys_logstart(void)
{
    int type;
    int state;

    argint(0, &type);
    argint(1, &state);

    return logstart((enum logtype)type, state);
}

uint64
sys_logint(void)
{
    int type;
    int t;

    argint(0, &type);
    argint(1, &t);

    acquire(&tickslock);
    uint tcks = ticks;
    release(&tickslock);

    return logint((enum logtype)type, tcks, t);
}
