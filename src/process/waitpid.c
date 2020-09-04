#define _GNU_SOURCE
#include <sys/wait.h>
#include "syscall.h"
#include "wait4_waitid.h"

pid_t waitpid(pid_t pid, int *status, int options)
{
	return __wait4(SYS_wait4, pid, status, options, 0, 1);
}
