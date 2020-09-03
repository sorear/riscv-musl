#define _GNU_SOURCE
#include <sys/wait.h>
#include "syscall.h"
#include "wait4_waitid.h"

pid_t waitpid(pid_t pid, int *status, int options)
{
#ifdef SYS_wait4
	return syscall_cp(SYS_wait4, pid, status, options, 0);
#else
	return __syscall_ret(__wait4_waitid(pid, status, options, 0, 1));
#endif
}
