#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
EXPORT_NO_SYMBOLS;

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
  if(current->uid > 0)
    printk("uid %d is opening file %s\n", current->uid, file);
  return old_open(file, flags, mode);
}
