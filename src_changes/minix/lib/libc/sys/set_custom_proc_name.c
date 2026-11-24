#include <sys/cdefs.h>
#include "namespace.h"
#include <errno.h>
#include <lib.h>
#include <string.h>
#include <unistd.h>

int set_custom_proc_name(pid_t pid, const char *name)
{
	message m;

	if (name == NULL) {
		errno = EINVAL;
		return -1;
	}

	memset(&m, 0, sizeof(m));
	m.m_lc_pm_setprocname.pid = pid;
	m.m_lc_pm_setprocname.name = (vir_bytes) name;
	m.m_lc_pm_setprocname.len = strlen(name) + 1;

	return _syscall(PM_PROC_NR, PM_SET_CUSTOM_PROC_NAME, &m);
}
