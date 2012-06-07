/*
 * 	includes.h	- Important header files and some Macros I need
 */

#ifndef __INCLUDES__
#define __INCLUDES__

#ifndef __KERNEL__
 #define __KERNEL__
#endif
#ifndef MODULE
 #define MODULE
#endif

static int errno;
#define __KERNEL_SYSCALLS__

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/smp_lock.h>
#include <linux/unistd.h>
#include <sys/syscall.h>

#include <asm/uaccess.h>

#ifdef DEBUG
 #define P_DEBUG(format, arg...) printk(format, ## arg)
#else
 #define P_DEBUG(format, arg...)
#endif

#endif
