#include "syslib.h"

int sys_setname(endpoint_t proc_ep, vir_bytes name, size_t len)
{
	/* Tell the kernel to update the process name in struct proc. */
	message m;

	m.m_lsys_krn_sys_setname.endpt = proc_ep;
	m.m_lsys_krn_sys_setname.name = name;
	m.m_lsys_krn_sys_setname.len = len;

	return _kernel_call(SYS_SETNAME, &m);
}
