#include "stdio_impl.h"
#include <sys/wait.h>
#include "wait4_waitid.h"
#include <errno.h>
#include <unistd.h>

int pclose(FILE *f)
{
	int status, r;
	pid_t pid = f->pipe_pid;
	fclose(f);
#ifdef SYS_wait4
	while ((r=__syscall(SYS_wait4, pid, &status, 0, 0)) == -EINTR);
#else
	while ((r=__wait4_waitid(pid, &status, 0, 0, 0)) == -EINTR);
#endif
	if (r<0) return __syscall_ret(r);
	return status;
}
