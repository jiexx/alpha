/**************************************************************************
 *
 * IP Host Demo
 * ....................................
 * Init and main loop
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
//.........................................
#include "lwip/api.h"
//.........................................

//-----------------------------------------------------------------------------
// TCP Echo client thread
static DWORD __stdcall EchoClientThread(void *param)
{
	static const char welcome[]={"Welcome. Just type and I repeat !\n\rPress X to quit\n\xFF\xFE\x01\n\r"};
	struct netconn *session = (struct netconn *)param;

	netconn_write(session, welcome,sizeof(welcome)-1,NETCONN_COPY);
	for(;;)
	{
		struct pbuf *p;
		err_t re=netconn_recv_tcp_pbuf(session,&p);
		if(re)
		{
			DBGNOT("netconn_recv failed : {0}") % re;
			break;
		}

		DBGNOT("Echo {0} bytes") % p->len;

		u8_t v = ((const u8_t*)p->payload)[0];
		if(v=='X') break;
		if(v==255) continue; // Ignore telnet chat

		err_t we=netconn_write(session, p->payload,p->len,NETCONN_COPY);

		pbuf_free(p);
		if(we)
		{
			DBGNOT("netconn_write failed : {0}") % we;

			break;
		}
	}
	DBGNOT("End of session");
	netconn_delete(session);
	return 0;
}


//-----------------------------------------------------------------------------
// TCP Echo server listen loop
static DWORD __stdcall EchoServerThread(void *param)
{
	struct netconn *conn = (struct netconn *)param;

	for(;;)
	{
		struct netconn *session;
		
		netconn_accept(conn,&session);

		HANDLE hEchoCliThread = StartThread(NULL,0,EchoClientThread,(void*)session,0,NULL);
		if(hEchoCliThread==NULL)
		{
			DBGERR("StartThread(TapPollThread) failed: {0}") % DBGLASTERROR();
			netconn_delete(session);
		}
		CloseHandle(hEchoCliThread);
	}
}

bool StartEchoServer()
{
	struct netconn *conn = netconn_new(NETCONN_TCP);

	netconn_bind	(conn, NULL, 23);
	netconn_listen	(conn);

	HANDLE hEchoThread = StartThread(NULL,0,EchoServerThread,(void*)conn,0,NULL);
	if(hEchoThread==NULL)
	{
		DBGERR("StartThread(EchoServerThread) failed: {0}") % DBGLASTERROR();
		return false;
	}
	CloseHandle(hEchoThread);
	return true;
}