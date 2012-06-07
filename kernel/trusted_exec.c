#define __KERNEL__
#define MODULE

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <sys/syscall.h>

EXPORT_NO_SYMBOLS;
MODULE_LICENSE("GPL");

extern void *sys_call_table[];

int (*old_execve)(const char *, const char *[], const char *[]);
int new_execve(const char *, const char *[], const char *[]);

int init_module(void)
{
  old_execve = sys_call_table[__NR_execve];
  sys_call_table[__NR_execve] = new_execve;
  return 0;
}

void cleanup_module(void)
{
  sys_call_table[__NR_execve] = old_execve;
}

int new_execve(const char *filename, const char *argv[], const char *envp[])
{
  // struct nameidata nd;

  /*  
  if(!path_walk(filename,&nd))
    {
      if(nd.dentry->d_inode->i_mode & (S_IWGRP | S_IWOTH) || nd.dentry->d_inode->i_uid)
	{
	  printk("[trusted_exec]: Access denied!\n");
	  // return -EPERM;
	}
      else
	{
	  printk("[trusted_exec]: Access granted\n");
	  // return old_execve(filename,argv,envp);
	}
    }
  else
    {
      printk("[trusted_exec]: An error occured!\n");
      // return -EIO;
    }
  */
  return old_execve(filename,argv,envp);
}
