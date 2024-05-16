#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_vmprint(void)
{
    pagetable_t pagetable = myproc()->pagetable;
    printf("page table %p\n", pagetable);
    vmprint_req(pagetable, 1);

    return 0;
}

uint64
sys_pgaccess(void)
{
    uint64 va;
    int size;

    argaddr(0, &va);
    argint(1, &size);

    pagetable_t pg = myproc()->pagetable;
    char *p = (char *)va;
    int access = 0;

    for (; p != (char *)va + size; p++) {
        pte_t *pte = walk(pg, (uint64)p, 0);
        access |= (*pte & PTE_A) != 0;
    }

    for (p = (char *)va; p != (char *)va + size; p++) {
        pte_t *pte = walk(pg, (uint64)p, 0);
        *pte = *pte & ~(PTE_A);
    }

    return access;
}
