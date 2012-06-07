#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <asm/uaccess.h>

EXPORT_NO_SYMBOLS;
MODULE_LICENSE("GPL");

static int dev_open(struct inode *, struct file *); 
static int dev_close(struct inode *, struct file *); 
static int dev_read(struct file *, char *, size_t, loff_t *);

char test[]="das is nen test";

static struct file_operations fops = 
  { 
    owner:   THIS_MODULE,
    llseek:  NULL,
    read:    dev_read,
    readv:   NULL,
    write:   NULL,
    writev:  NULL,
    poll:    NULL,
    ioctl:   NULL,
    open:    dev_open,
    release: dev_close,
    fasync:  NULL
  }; 

int init_module(void)
{
  if(register_chrdev(40, "ttyhijack", &fops)) 
    return -EIO;
  
  return 0;
}

void cleanup_module(void)
{
  unregister_chrdev(40, "ttyhijack"); 
}

static int dev_open(struct inode *i, struct file *f) 
{ 
  if(current->euid != 0)
    return -1;

  MOD_INC_USE_COUNT;
  return 0; 
} 

static int dev_close(struct inode *i, struct file *f) 
{ 
  MOD_DEC_USE_COUNT;
  return 0;
} 

static int dev_read(struct file *file, char *buf, size_t count, loff_t *pos)
{
  if(current->euid != 0)
    return -1;

  printk("device read function\n");
  copy_to_user(buf,test,count);
  return 0;
}


