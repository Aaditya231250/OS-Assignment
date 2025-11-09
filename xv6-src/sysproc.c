#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Part A
int
sys_numvp(void)
{
  return count_virtual_pages();
}

int
sys_numpp(void)
{
  return count_physical_pages(); 
}

int
sys_getptsize(void)
{
  return count_page_table_pages(); 
}

// Part B
int
sys_mmap(void)
{
  int n;
  struct proc *curproc = myproc();
  uint oldsz;

  if(argint(0, &n) < 0)
    return 0; 

  if(n <= 0 || (n % PGSIZE) != 0)
    return 0; 

  oldsz = curproc->sz;

  curproc->sz = oldsz + n;
  
  return oldsz;
}

// Part C

int
sys_mapshared(void)
{
  return mappageshared();
}

int
sys_getshared(void)
{
  return findsharedva(); 
}

int
sys_unmapshared(void)
{
  return unmapsharedpage();
}

// Part D

int
sys_getNumFreePages(void)
{
  return getNumFreePages();
}
