/**************************************************************************
 *
 * IP Host Demo
 * ....................................
 * Mutex API
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
#include "lwip/sys.h"
//.........................................


extern "C" err_t sys_mutex_new(sys_mutex_t *mutex)
{
	CRITICAL_SECTION *newCS = new CRITICAL_SECTION();
	::InitializeCriticalSection(newCS);
	*mutex = (sys_mutex_t)newCS;
	return ERR_OK;
}


/** Lock a mutex
 * @param mutex the mutex to lock */
extern "C" void sys_mutex_lock(sys_mutex_t *mutex)
{
	::EnterCriticalSection( (LPCRITICAL_SECTION) *mutex );
}

/** Unlock a mutex
 * @param mutex the mutex to unlock */
extern "C" void sys_mutex_unlock(sys_mutex_t *mutex)
{
	::LeaveCriticalSection( (LPCRITICAL_SECTION) *mutex );
}

/** Delete a semaphore
 * @param mutex the mutex to delete */
extern "C" void sys_mutex_free(sys_mutex_t *mutex)
{
	CRITICAL_SECTION *oldCS = (LPCRITICAL_SECTION) *mutex;
	::DeleteCriticalSection( oldCS );
	delete oldCS;
}