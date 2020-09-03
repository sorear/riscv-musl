#include <sys/wait.h>
#include "syscall.h"
#include "wait4_waitid.h"

hidden pid_t __wait4_waitid(pid_t pid, int *status, int options, void *kru, int cp)
{
	idtype_t t;
	int r;
	siginfo_t info;

	info.si_pid = 0;
	if (pid < -1) {
		t = P_PGID;
		pid = -pid;
	} else if (pid == -1) {
		t = P_ALL;
	} else if (pid == 0) {
		t = P_PGID;
	} else {
		t = P_PID;
	}

	if (cp) r = __syscall_cp(SYS_waitid, t, pid, &info, options|WEXITED, kru);
	else r = __syscall(SYS_waitid, t, pid, &info, options|WEXITED, kru);

	if (r<0) return r;

	if (info.si_pid && status) {
		int sw=0;
		switch (info.si_code) {
		case CLD_CONTINUED:
			sw = 0xffff;
			break;
		case CLD_DUMPED:
			sw = info.si_status&0x7f | 0x80;
			break;
		case CLD_EXITED:
			sw = (info.si_status&0xff) << 8;
			break;
		case CLD_KILLED:
			sw = info.si_status&0x7f;
			break;
		case CLD_STOPPED:
		case CLD_TRAPPED:
			sw = ((info.si_status&0xff) << 8) + 0x7f;
			break;
		}
		*status = sw;
	}

	return info.si_pid;
}
