#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
EXPORT_NO_SYMBOLS;

char *input;

MODULE_PARM(input, "s");

int init_module(void)
{
  printk("%s\n",input);
  return 0;
}

void cleanup_module(void) {}
