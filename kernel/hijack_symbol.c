#define __KERNEL__
#define MODULE

#include <linux/module.h>

EXPORT_NO_SYMBOLS;
MODULE_LICENSE("GPL");

extern int *ipt_register_table;
int *old_ipt_register_table;

int new_ipt_register_table(void)
{
  return 0;
}

int init_module(void)
{
  old_ipt_register_table = ipt_register_table;
  ipt_register_table = new_ipt_register_table;
  return 0;
}

void cleanup_module(void)
{
  ipt_register_table = old_ipt_register_table;
}
