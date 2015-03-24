/**************************************************************************
 *
 * IP Host Demo
 * ....................................
 * MailBox API
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

namespace {
class mboxImpl
{
	std::list<void*> _mbox;
	CRITICAL_SECTION  _cs;
	HANDLE			  _hPushEvent;
public:
	mboxImpl(HANDLE hPushEvent) : _hPushEvent(hPushEvent),_mbox()
	{
		InitializeCriticalSection(&_cs);
	}
	~mboxImpl()
	{
		CloseHandle(_hPushEvent);
		DeleteCriticalSection(&_cs);
	}

	void post(void *msg)
	{
		CriticalSection cs(_cs);
		_mbox.push_back(msg);
		SetEvent(_hPushEvent);
	}

	void *fetch(u32_t timeout);
};


void *mboxImpl::fetch(u32_t timeout)
{
	for(;;)
	{
		//- Try to get something
		{
			CriticalSection cs(_cs);
			if(!_mbox.empty())
			{
				void *r = _mbox.front();
				_mbox.pop_front();
				if(_mbox.empty()) ResetEvent(_hPushEvent);
				return r;
			}
		}

		//- Wait
		switch(WaitForSingleObject(_hPushEvent,timeout))
		{
		case WAIT_TIMEOUT: return NULL;
		case WAIT_OBJECT_0: break;
		default:  		   return NULL;
		}
	}
}

}

/** Create a new mbox of specified size
 * @param mbox pointer to the mbox to create
 * @param size (miminum) number of messages in this mbox
 * @return ERR_OK if successful, another err_t otherwise */
extern "C" err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
	HANDLE hPushEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	if(hPushEvent==NULL)
	{
		DBGERR("CreateEvent failed {0}") % DBGLASTERROR();
		return ERR_MEM;
	}

	mboxImpl *box = new mboxImpl(hPushEvent);
	*mbox = (sys_mbox_t)box;
	return ERR_OK;
}

/** Delete an mbox
 * @param mbox mbox to delete */
void sys_mbox_free(sys_mbox_t *mbox)
{
	mboxImpl *box = (mboxImpl *)*mbox;
	delete box;
}

/** Post a message to an mbox - may not fail
 * -> blocks if full, only used from tasks not from ISR
 * @param mbox mbox to posts the message
 * @param msg message to post (ATTENTION: can be NULL) */
extern "C" void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
	mboxImpl *box = (mboxImpl *)*mbox;
	box->post(msg);
}


/** Try to post a message to an mbox - may fail if full or ISR
 * @param mbox mbox to posts the message
 * @param msg message to post (ATTENTION: can be NULL) */
extern "C" err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	mboxImpl *box = (mboxImpl *)*mbox;
	box->post(msg);
	return ERR_OK;
}

/** Wait for a new message to arrive in the mbox
 * @param mbox mbox to get a message from
 * @param msg pointer where the message is stored
 * @param timeout maximum time (in milliseconds) to wait for a message
 * @return time (in milliseconds) waited for a message, may be 0 if not waited
           or SYS_ARCH_TIMEOUT on timeout
 *         The returned time has to be accurate to prevent timer jitter! */
extern "C" u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	mboxImpl *box = (mboxImpl *)*mbox;
	s64_t ori = GetTickCount();
	*msg = box->fetch(timeout==0?INFINITE:timeout);
	if(*msg==NULL) return SYS_ARCH_TIMEOUT;
	s64_t time = GetTickCount()-ori;
	if(time<0) time=-time;
	return (u32_t)time;
}


/** Wait for a new message to arrive in the mbox
 * @param mbox mbox to get a message from
 * @param msg pointer where the message is stored
 * @param timeout maximum time (in milliseconds) to wait for a message
 * @return 0 (milliseconds) if a message has been received
 *         or SYS_MBOX_EMPTY if the mailbox is empty */
extern "C" u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	mboxImpl *box = (mboxImpl *)*mbox;
	*msg = box->fetch(0);
	if(*msg==NULL) return SYS_MBOX_EMPTY;
	return 0;
}