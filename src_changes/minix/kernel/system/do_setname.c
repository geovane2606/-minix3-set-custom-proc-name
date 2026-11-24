/* The kernel call implemented in this file:
 *   m_type:	SYS_SETNAME
 *
 * The parameters for this kernel call are:
 *   m_lsys_krn_sys_setname.endpt	(target process endpoint)
 *   m_lsys_krn_sys_setname.name	(pointer to new name in caller)
 *   m_lsys_krn_sys_setname.len		(length of name buffer)
 */
#include "kernel/system.h"
#include <string.h>
#include <minix/endpoint.h>

/*===========================================================================*
 *				do_setname				     *
 *===========================================================================*/
int do_setname(struct proc * caller, message * m_ptr)
{
	register struct proc *rp;
	int proc_nr;
	size_t len;
	char name[PROC_NAME_LEN];

	if (!isokendpt(m_ptr->m_lsys_krn_sys_setname.endpt, &proc_nr))
		return EINVAL;

	rp = proc_addr(proc_nr);
	len = m_ptr->m_lsys_krn_sys_setname.len;

	if (len == 0)
		return EINVAL;
	if (len > sizeof(name) - 1)
		len = sizeof(name) - 1;

	if (data_copy(caller->p_endpoint, m_ptr->m_lsys_krn_sys_setname.name,
	    KERNEL, (vir_bytes) name, (phys_bytes) len) != OK)
		return EFAULT;

	name[len] = '\0';
	name[sizeof(name) - 1] = '\0';

	strncpy(rp->p_name, name, PROC_NAME_LEN - 1);
	rp->p_name[PROC_NAME_LEN - 1] = '\0';

	return OK;
}
