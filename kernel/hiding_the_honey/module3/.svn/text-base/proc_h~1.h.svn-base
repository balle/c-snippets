#ifndef PROCHOOK_H
#define PROCHOOK_H

#include "includes.h"

#include <linux/proc_fs.h>

void init_proc();
void restore_proc();

int conf_read(char *, char **, off_t, int, int *, void *);
int conf_write(struct file *, const char *, unsigned long, void *);
int m_uptime_write(struct file *, const char *, unsigned long, void *);

int proc_hidden();

#endif 
