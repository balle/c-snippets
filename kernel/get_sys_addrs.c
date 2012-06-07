#define __KERNEL__
#define MODULE

#include <linux/module.h>

EXPORT_NO_SYMBOLS;
MODULE_LICENSE("GPL");

extern void *sys_call_table[];

int init_module(void)
{
  int *i;

  for(i=(int*)&sys_call_table; *i != '\0'; i += 4)
    printk("%x\n",i);

  return 0;
}

void cleanup_module(void)
{

}
