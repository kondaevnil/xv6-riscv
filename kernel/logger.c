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

void
logvirt(int irq)
{
    acquire(&tickslock);
    uint t = ticks;
    release(&tickslock);

    acquire(&logger.lck);

    if (islog(logger.interrupt, t)) {
        pr_msg("%d interrupt: virtio", irq);
    }

    release(&logger.lck);
}

void
loguart(int irq, char ch)
{
    acquire(&tickslock);
    uint t = ticks;
    release(&tickslock);

    acquire(&logger.lck);

    if (islog(logger.interrupt, t)) {
        pr_msg("%d interrupt: uart, keyboard %c", irq, ch);
    }

    release(&logger.lck);
}

void
logswch(int pid, const char *pname, struct trapframe tf, struct context context)
{
    acquire(&tickslock);
    uint t = ticks;
    release(&tickslock);

    acquire(&logger.lck);

    if (islog(logger.switching, t)) {
        pr_msg("Switch proc with pid: [%d], name: [%s],\n"
               "trapframe:\nkernel_satp: [%d], kernel_sp: [%d], kernel_trap: [%d], epc: [%d], kernel_hartid: [%d], ra : [%d], sp: [%d], gp: [%d], tp: [%d]\n"
               "t0: [%d], t1: [%d], t2: [%d], s0: [%d], s1: [%d],\n"
               "a0: [%d], a1: [%d], a2: [%d], a3: [%d], a4: [%d], a5: [%d], a6: [%d], a7: [%d],\n"
               "s2: [%d], s3: [%d], s4: [%d], s5: [%d], s6: [%d], s7: [%d], s8: [%d], s9: [%d], s10: [%d], s11: [%d],\n"
               "t3: [%d], t4: [%d], t5: [%d], t6: [%d],\n"
               "context:\nra: [%d], sp: [%d],\ns0: [%d], s1: [%d], s2: [%d], s3: [%d], s4: [%d], s5: [%d], s6: [%d], s7: [%d], s8: [%d], s9: [%d], s10: [%d], s11: [%d]",
               pid, pname,
               tf.kernel_satp, tf.kernel_sp, tf.kernel_trap, tf.epc, tf.kernel_hartid, tf.ra, tf.sp, tf.gp, tf.tp,
               tf.t0, tf.t1, tf.t2, tf.s0, tf.s1,
               tf.a0, tf.a1, tf.a2, tf.a3, tf.a4, tf.a5, tf.a6, tf.a7,
               tf.s2, tf.s3, tf.s4, tf.s5, tf.s6, tf.s7, tf.s8, tf.s9, tf.s10, tf.s11,
               tf.t3, tf.t4, tf.t5, tf.t6,
               context.ra, context.sp, context.s0, context.s1, context.s2, context.s3,  context.s4,
               context.s5, context.s6, context.s7, context.s8, context.s9, context.s10, context.s11);
    }

    release(&logger.lck);
}

int
logstart(enum logtype type, int state)
{
    switch (type) {
        case EXEC:
            acquire(&logger.lck);
            logger.exec.inf = state == 1;
            release(&logger.lck);
            return 0;
        case SWITCHING:
            acquire(&logger.lck);
            logger.switching.inf = state == 1;
            release(&logger.lck);
            return 0;
        case INTERRUPT:
            acquire(&logger.lck);
            logger.interrupt.inf = state == 1;
            release(&logger.lck);
            return 0;
        case SYSCALL:
            acquire(&logger.lck);
            logger.syscall.inf = state == 1;
            release(&logger.lck);
            return 0;
        default:
            return -1;
    }
}

int
logint(enum logtype type, uint start, uint t)
{
    switch (type) {
        case EXEC:
            acquire(&logger.lck);
            logger.exec.start = start;
            logger.exec.end = start + t;
            release(&logger.lck);
            return 0;
        case SWITCHING:
            acquire(&logger.lck);
            logger.switching.start = start;
            logger.switching.end = start + t;
            release(&logger.lck);
            return 0;
        case INTERRUPT:
            acquire(&logger.lck);
            logger.interrupt.start = start;
            logger.interrupt.end = start + t;
            release(&logger.lck);
            return 0;
        case SYSCALL:
            acquire(&logger.lck);
            logger.syscall.start = start;
            logger.syscall.end = start + t;
            release(&logger.lck);
            return 0;
        default:
            return -1;
    }
}
