#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "proc.h"
#include "mutex.h"

uint64
sys_get_mutex() {
    int r = get_mutex();
    struct proc *p = myproc();

    if (r == -1)
        return -1;

    for (int i = 0; i < PMUTEX; i++) {
        if (p->mdesc[i] == -1) {
            p->mdesc[i] = r;
            return i;
        }
    }

    return -2;
}

uint64
sys_acquire_mutex() {
    int desc;
    argint(0, &desc);

    if (desc < 0 || desc >= PMUTEX) return -1;

    return acquire_mutex(myproc()->mdesc[desc]);
}

uint64
sys_release_mutex() {
    int desc;
    argint(0, &desc);

    if (desc < 0 || desc >= PMUTEX) return -1;

    return release_mutex(myproc()->mdesc[desc]);
}

uint64
sys_free_mutex() {
    int desc;
    argint(0, &desc);

    if (desc < 0 || desc >= PMUTEX) return -1;

    int r = free_mutex(myproc()->mdesc[desc]);
    if (r == 0) myproc()->mdesc[desc] = -1;
    return r;
}