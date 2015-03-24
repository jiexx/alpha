#pragma once

#define SYS_MBOX_NULL	0
#define SYS_SEM_NULL	0

// More or less proxy for Win32 Handle
typedef u32_t* sys_mutex_t;
typedef u32_t* sys_sem_t;
typedef u32_t* sys_mbox_t;
typedef u32_t  sys_thread_t;

// Not implemented
#define sys_mutex_valid(mutex)        
#define sys_mutex_set_invalid(mutex)  

#define sys_sem_valid			__noop
#define sys_sem_set_invalid		__noop

__inline int sys_mbox_valid(sys_mbox_t *x) 
{
	return ((*x)==0)?0:1;
}
#define sys_mbox_set_invalid	__noop


