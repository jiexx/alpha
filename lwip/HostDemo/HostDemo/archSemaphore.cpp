/**************************************************************************
 *
 * IP Host Demo
 * ....................................
 * Semaphore API
 * 
 *
 * ------------------------------------------------------------------------
 * (c) copyright 2011 Laurent Dupuis
 * ........................................................................
 * < This program is free software: you can redistribute it and/or modify
 * < it under the terms of the GNU General Public License as published by
 * < the Free Software Foundation, either version 3 of the License, or
 * < (at your option) any later version.
 * < 
 * < This program is distributed in the hope that it will be useful,
 * < but WITHOUT ANY WARRANTY; without even the implied warranty of
 * < MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * < GNU General Public License for more details.
 * < 
 * < You should have received a copy of the GNU General Public License
 * < along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ........................................................................
 *
 */
//.........................................
#include "stdafx.h"
#include "log.h"
#include "lwip/sys.h"
//.........................................

/** Create a new semaphore
 * @param sem pointer to the semaphore to create
 * @param count initial count of the semaphore
 * @return ERR_OK if successful, another err_t otherwise */
extern "C" err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	HANDLE hSem = ::CreateSemaphore(NULL,count,255,NULL);
	if(hSem==NULL)
	{
		DBGERR("CreateSemaphore failed {0}") % DBGLASTERROR();
		return ERR_MEM;
	}

	*sem = (sys_sem_t)hSem;

	return ERR_OK;
}

/** Signals a semaphore
 * @param sem the semaphore to signal */
extern "C" void sys_sem_signal(sys_sem_t *sem)
{
	HANDLE hSem = (HANDLE)*sem;
	ReleaseSemaphore (hSem,1,NULL);
}

/** Wait for a semaphore for the specified timeout
 * @param sem the semaphore to wait for
 * @param timeout timeout in milliseconds to wait (0 = wait forever)
 * @return time (in milliseconds) waited for the semaphore
 *         or SYS_ARCH_TIMEOUT on timeout */
extern "C" u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	HANDLE hSem = (HANDLE)*sem;
	s64_t ori = GetTickCount();
	switch(WaitForSingleObject(hSem,!timeout?INFINITE:timeout))
	{
	case WAIT_TIMEOUT: return SYS_ARCH_TIMEOUT;
	case WAIT_OBJECT_0: break;

	default:
		return ERR_MEM;
	}
	s64_t time = GetTickCount()-ori;
	if(time<0) time=-time;
	return (u32_t)time;
}

/** Delete a semaphore
 * @param sem semaphore to delete */
extern "C" void sys_sem_free(sys_sem_t *sem)
{
	HANDLE hSem = (HANDLE)*sem;
	CloseHandle(hSem);
}