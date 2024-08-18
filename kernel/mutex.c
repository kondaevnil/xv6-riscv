#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "proc.h"
#include "mutex.h"

struct mutex mut[NMUTEX];

void
mutexinit(void) {
    for (int i = 0; i < NMUTEX; i++) {
        initlock(&mut[i].lock, "spl");
        initsleeplock(&mut[i].mutex_lock, "sll");
        mut[i].ucount = 0;
    }
}

int
get_mutex(void) {
    for (int i = 0; i < NMUTEX; i++) {
        acquire(&mut[i].lock);
        if (mut[i].ucount == 0) {
            mut[i].ucount = 1;
            release(&mut[i].lock);
            return i;
        }
        release(&mut[i].lock);
    }

    return -2;
}

int
acquire_mutex(int mdesc) {
    if (mdesc < 0 || mdesc >= NMUTEX) {
        return -1;
    }

    acquire(&mut[mdesc].lock);
    if (mut[mdesc].ucount == 0) {
        release(&mut[mdesc].lock);
        return -2;
    }
    release(&mut[mdesc].lock);

    acquiresleep(&mut[mdesc].mutex_lock);

    return 0;
}

int
release_mutex(int mdesc) {
    if (mdesc < 0 || mdesc >= NMUTEX) {
        return -1;
    }

    acquire(&mut[mdesc].lock);
    if (mut[mdesc].ucount == 0) {
        release(&mut[mdesc].lock);
        return -2;
    }
    release(&mut[mdesc].lock);

    releasesleep(&mut[mdesc].mutex_lock);
    return 0;
}

int
free_mutex(int mdesc) {
    if (mdesc < 0 || mdesc >= NMUTEX) {
        return -1;
    }

    acquire(&mut[mdesc].lock);
    if (mut[mdesc].ucount == 0) {
        release(&mut[mdesc].lock);
        return -2;
    }

    if (holdingsleep(&mut[mdesc].mutex_lock) > 0) {
        releasesleep(&mut[mdesc].mutex_lock);
    }

    mut[mdesc].ucount--;
    release(&mut[mdesc].lock);
    return 0;
}

void
fork_mutex(int mdesc) {
    acquire(&mut[mdesc].lock);
    mut[mdesc].ucount++;
    release(&mut[mdesc].lock);
}

//int
//exit_mutex(int mdesc) {
//    acquire(&mut[mdesc].lock);
//    if (mut[mdesc].ucount == 0) {
//        release(&mut[mdesc].lock);
//        return -2;
//    }
//    mut[mdesc].ucount--;
//    release(&mut[mdesc].lock);
//    return 0;
//}
