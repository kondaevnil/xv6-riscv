#include "types.h"
#include "riscv.h"
#include "param.h"
#include "spinlock.h"
#include "defs.h"
#include "proc.h"
#include "logger.h"

struct logger logger;

int
islog(struct interval event, uint t)
{
    return event.inf || (event.start <= t && t <= event.end);
}

void
loginit(void)
{
    memset((void *)&logger, 0, sizeof(logger));
    initlock(&logger.lck, "logger lock");
}

void
logsc(int pid, const char *name, int num)
{
    acquire(&tickslock);
    uint t = ticks;
    release(&tickslock);

    acquire(&logger.lck);

    if (islog(logger.syscall, t)) {
        pr_msg("pid: %d pname: %s sc: %d\n", pid, name, num);
    }

    release(&logger.lck);
}

void
logusc(int pid, const char *name, int num)
{
    acquire(&tickslock);
    uint t = ticks;
    release(&tickslock);

    acquire(&logger.lck);

    if (islog(logger.syscall, t)) {
        pr_msg("%d %s: unknown sys call %d\n", pid, name, num);
    }

    release(&logger.lck);
}

void
logex(int pid, const char *path)
{
    acquire(&tickslock);
    uint t = ticks;
    release(&tickslock);

    acquire(&logger.lck);

    if (islog(logger.exec, t)) {
        pr_msg("proc: %d exec: %s", pid, path);
    }

    release(&logger.lck);
}
