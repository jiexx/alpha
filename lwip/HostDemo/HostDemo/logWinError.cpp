/**************************************************************************
 *
 * IP Host Demo
 * ....................................
 * LOG - Decode Windows errors
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

#ifdef _DEBUG

using namespace debug;

//////////////////////////////////////////////////////////////////////////////
void logWinError::getValue(std::string &toAppend) const
{
	LPVOID lpMsgBuf;
	char lBuf[20];
	_itoa_s(winErr,lBuf,sizeof(lBuf),10);
	toAppend.append(lBuf);
	toAppend.append(": ");

	if(winErr>WSABASEERR)
	{
#define CASE_CODE(x) case x: toAppend.append("Winsock" #x);return;
		switch(winErr)
		{
		CASE_CODE(WSAEINTR)
		CASE_CODE(WSAEBADF)
		CASE_CODE(WSAEACCES)
		CASE_CODE(WSAEFAULT)
		CASE_CODE(WSAEINVAL)
		CASE_CODE(WSAEMFILE)
		CASE_CODE(WSAEWOULDBLOCK)
		CASE_CODE(WSAEINPROGRESS)       
		CASE_CODE(WSAEALREADY)      
		CASE_CODE(WSAENOTSOCK)         
		CASE_CODE(WSAEDESTADDRREQ)
		CASE_CODE(WSAEMSGSIZE)          
		CASE_CODE(WSAEPROTOTYPE)        
		CASE_CODE(WSAENOPROTOOPT)       
		CASE_CODE(WSAEPROTONOSUPPORT)
		CASE_CODE(WSAESOCKTNOSUPPORT)
		CASE_CODE(WSAEOPNOTSUPP)  
		CASE_CODE(WSAEPFNOSUPPORT)
		CASE_CODE(WSAEAFNOSUPPORT)
		CASE_CODE(WSAEADDRINUSE)
		CASE_CODE(WSAEADDRNOTAVAIL)
		CASE_CODE(WSAENETDOWN)
		CASE_CODE(WSAENETUNREACH)
		CASE_CODE(WSAENETRESET)
		CASE_CODE(WSAECONNABORTED)
		CASE_CODE(WSAECONNRESET)
		CASE_CODE(WSAENOBUFS)
		CASE_CODE(WSAEISCONN)
		CASE_CODE(WSAENOTCONN)
		CASE_CODE(WSAESHUTDOWN)
		CASE_CODE(WSAETOOMANYREFS)
		CASE_CODE(WSAETIMEDOUT)
		CASE_CODE(WSAECONNREFUSED)
		CASE_CODE(WSAELOOP)
		CASE_CODE(WSAENAMETOOLONG)
		CASE_CODE(WSAEHOSTDOWN)
		CASE_CODE(WSAEHOSTUNREACH)
		CASE_CODE(WSAENOTEMPTY)
		CASE_CODE(WSAEPROCLIM)
		CASE_CODE(WSAEUSERS)
		CASE_CODE(WSAEDQUOT)
		CASE_CODE(WSAESTALE)
		CASE_CODE(WSAEREMOTE)
		CASE_CODE(WSASYSNOTREADY)
		CASE_CODE(WSAVERNOTSUPPORTED)
		CASE_CODE(WSANOTINITIALISED)
		CASE_CODE(WSAEDISCON)
		CASE_CODE(WSAENOMORE)
		CASE_CODE(WSAECANCELLED)
		CASE_CODE(WSAEINVALIDPROCTABLE)
		CASE_CODE(WSAEINVALIDPROVIDER)
		CASE_CODE(WSAEPROVIDERFAILEDINIT)
		CASE_CODE(WSASYSCALLFAILURE)
		CASE_CODE(WSASERVICE_NOT_FOUND)
		CASE_CODE(WSATYPE_NOT_FOUND)
		CASE_CODE(WSA_E_NO_MORE)
		CASE_CODE(WSA_E_CANCELLED)
		CASE_CODE(WSAEREFUSED)
		CASE_CODE(WSAHOST_NOT_FOUND)
		CASE_CODE(WSATRY_AGAIN)
		CASE_CODE(WSANO_RECOVERY)
		CASE_CODE(WSANO_DATA)
		CASE_CODE(WSA_QOS_RECEIVERS)
		CASE_CODE(WSA_QOS_SENDERS)
		CASE_CODE(WSA_QOS_NO_SENDERS)
		CASE_CODE(WSA_QOS_NO_RECEIVERS)
		CASE_CODE(WSA_QOS_REQUEST_CONFIRMED)
		CASE_CODE(WSA_QOS_ADMISSION_FAILURE)
		CASE_CODE(WSA_QOS_POLICY_FAILURE)
		CASE_CODE(WSA_QOS_BAD_STYLE)
		CASE_CODE(WSA_QOS_BAD_OBJECT)
		CASE_CODE(WSA_QOS_TRAFFIC_CTRL_ERROR)
		CASE_CODE(WSA_QOS_GENERIC_ERROR)
		CASE_CODE(WSA_QOS_ESERVICETYPE)
		CASE_CODE(WSA_QOS_EFLOWSPEC)
		CASE_CODE(WSA_QOS_EPROVSPECBUF)
		CASE_CODE(WSA_QOS_EFILTERSTYLE)
		CASE_CODE(WSA_QOS_EFILTERTYPE)
		CASE_CODE(WSA_QOS_EFILTERCOUNT)
		CASE_CODE(WSA_QOS_EOBJLENGTH)
		CASE_CODE(WSA_QOS_EFLOWCOUNT)
		CASE_CODE(WSA_QOS_EUNKOWNPSOBJ)
		CASE_CODE(WSA_QOS_EPOLICYOBJ)
		CASE_CODE(WSA_QOS_EFLOWDESC)
		CASE_CODE(WSA_QOS_EPSFLOWSPEC)
		CASE_CODE(WSA_QOS_EPSFILTERSPEC)
		CASE_CODE(WSA_QOS_ESDMODEOBJ)
		CASE_CODE(WSA_QOS_ESHAPERATEOBJ)
		CASE_CODE(WSA_QOS_RESERVED_PETYPE)
		}
#undef CASE_CODE
	}

	if(FormatMessageA( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			winErr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPSTR) &lpMsgBuf,
			0,
			NULL 
		) == 0)
	{
		toAppend.append("No description aviable");
	}
	else
	{
		toAppend.append((LPCSTR)lpMsgBuf);
		LocalFree( lpMsgBuf );
	}
}



//////////////////////////////////////////////////////////////////////////////
#endif