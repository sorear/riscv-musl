#define _BSD_SOURCE
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdint.h>
#include "syscall.h"
#include "kstat.h"
#include "statx.h"

const char unsigned *__map_file(const char *pathname, size_t *size)
{
	const unsigned char *map = MAP_FAILED;
	int fd = sys_open(pathname, O_RDONLY|O_CLOEXEC|O_NONBLOCK);
	if (fd < 0) return 0;
#ifdef SYS_fstat
	struct kstat st;
	if (!syscall(SYS_fstat, fd, &st)) {
		map = __mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
		*size = st.st_size;
	}
#else
	struct statx st;
	if (!syscall(SYS_statx, fd, "", AT_EMPTY_PATH, STATX_SIZE, &st)) {
		map = __mmap(0, st.stx_size, PROT_READ, MAP_SHARED, fd, 0);
		*size = st.stx_size;
	}
#endif
	__syscall(SYS_close, fd);
	return map == MAP_FAILED ? 0 : map;
}
