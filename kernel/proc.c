#define __KERNEL__
#define MODULE
#define PROCHOOK_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

EXPORT_NO_SYMBOLS;
MODULE_LICENSE("GPL");

struct proc_dir_entry *my_dir; 
struct proc_dir_entry *conf_file;
char buffer[100] = {0};

int conf_read(char *, char **, off_t, int, int *, void *);
int conf_write(struct file *, const char *, unsigned long, void *);

int init_module(void)
{
  strcpy(buffer,"hack the planet");

  my_dir = proc_mkdir("balle", &proc_root);
  
  if(my_dir)
    {
      conf_file = create_proc_entry("test", 0600, my_dir);
      conf_file->owner = THIS_MODULE;
      conf_file->write_proc = conf_write;
      conf_file->read_proc = conf_read;
    }

  return 0;
}

void cleanup_module(void)
{
  remove_proc_entry(my_dir->name, &proc_root);
}

int conf_read(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
	return sprintf(buf, "Buffer: %s\n", buffer);
}

int conf_write(struct file *file, const char *buf, unsigned long count, void *data)
{
	memset(buffer, '\0', 100);
	strncpy(buffer, buf, count);	
	return strlen(buf);
}
