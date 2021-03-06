#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

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
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = proc->sz;
  
  //cprintf("before assign proc->sz:%d\n", proc->sz);
  
  
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
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_clone(void)
{
  void * fcn,* arg,* stack;
  int num, x;
  num = argint(0, &x);
  fcn = (void*) x;
  
  num = argint(1, &x);
  arg = (void*) x;
  
  num = argint(2, &x);
  stack = (void*) x;
  if(DEBUG){
    cprintf("Printing from sys_clone()\n");
    cprintf("fcn:%p\n", fcn);
    cprintf("arg: %d\n", * (int *) arg);
    cprintf("stackptr: %p\n", stack);
    //cprintf("Value at stack: %d\n",* (uint*)stack); 
    cprintf("Value at stack bottom of stack: %d\n",* (uint*)(stack+ PGSIZE-sizeof(void *))); 
  }
  return clone(fcn, arg, stack);
  //return 1;
}

int
sys_join(void)
{
  void **stack;
  int stackArg;
  stackArg = argint(0, &stackArg);
  stack = (void**) stackArg;
  if(DEBUG){
    cprintf("Printing from sys_clone()\n");
    cprintf("stack:%p\n", stack);
    cprintf("*stack:%p\n", *stack);
    cprintf("stackArg:%d\n", stackArg);
  }
  //struct joinproc jp = join(stack);
  return join(stack);
   
  //return join();
  //return 1;
}


