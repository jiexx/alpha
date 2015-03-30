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
#include "TapWin32.h"
//.........................................
#include "lwip/tcpip.h"
#include "netif/etharp.h"
//.........................................

static HANDLE		 sg_hevQuit,sg_hevTapOut,sg_hTapThread=NULL;
static unsigned char sg_TapMacAddr[6];
static CRITICAL_SECTION sg_TapOutq;
static std::list<struct pbuf *> sg_Outq;

//-----------------------------------------------------------------------------
// Unused (but mandatory) initialization routine
extern "C" void sys_init() {}

//-----------------------------------------------------------------------------
// Debug text out
extern "C"	void _lwip_dbgtextout(const char *format, ...)
{
	char buffer[4096];
	va_list args;
	va_start (args, format);
	int sz =vsprintf_s (buffer,sizeof(buffer),format, args);
	printf(buffer);
	va_end (args);
}

//-----------------------------------------------------------------------------
// Called by TCP/IP - Send an Ethernet packet
static  err_t netif_tap_linkoutput(struct netif *netif, struct pbuf *p)
{
	CriticalSection cs(sg_TapOutq);

	sg_Outq.push_back( p );	//< Push on the queue
	pbuf_ref (p);			//< Increment the reference count
	SetEvent(sg_hevTapOut);	//< Wake up the TAP interface main thread
	return ERR_OK;
}

//-----------------------------------------------------------------------------
// Push a packet to the TCP/IP stack
static void PushPacket(struct netif *netif,unsigned char *buffer,int len)
{
	DBGINF("Recv Packet {0}") % len; 

	struct pbuf *p, *q;
	p = pbuf_alloc(PBUF_RAW, (u16_t)len, PBUF_POOL);
	unsigned char *pBf = buffer;
	for(q = p; q != NULL; q = q->next) {
		memcpy(q->payload,pBf,min(len-(pBf-buffer),q->len));
		pBf+=q->len;
	}
	
	(*netif->input)(p,netif);
}

//-----------------------------------------------------------------------------
// TAP interface main thread
static DWORD __stdcall TapPollThread(void *param)
{
	struct netif *netif = (struct netif *)param;
	HTAP hTap = (HTAP)netif->state;
	OVERLAPPED ovRead = {0},ovWrite = {0};

	// The TAP driver is not working very well with blocking IO. This (quite complex)
	// OVERLAPPED/hEvent based approach is based on how OpenVPN interact with TAP.

	ovRead.hEvent  = CreateEvent(NULL,FALSE,FALSE,NULL);
	ovWrite.hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	HANDLE hvEvents[] = { ovRead.hEvent, ovWrite.hEvent, sg_hevTapOut, sg_hevQuit };

	unsigned char buffer[1500]={0};

	DWORD readed,written,status=1;
	struct pbuf *pWriteBuf = NULL;

	// Set the first read request
	DBGINF("Init ReadFile>>");
	status = ReadFile((HANDLE)hTap,(LPVOID)buffer,1500,&readed,&ovRead);
	DBGINF("<<Init ReadFile / status={0} / LErr={1}") % status % DBGLASTERROR();
	if(!status && GetLastError()!=ERROR_IO_PENDING)
	{
		DBGERR("Init ReadFile / status={0} / Error={1}") % status % DBGLASTERROR();
		return 0;
	}

	for(;;)
	{
		DWORD ret = WaitForMultipleObjects(sizeof(hvEvents)/sizeof(HANDLE),hvEvents,FALSE,INFINITE);
		switch(ret)
		{

		// A packet has been red from the TAP driver. Push it to the IP stack
		case WAIT_OBJECT_0:
			if(!GetOverlappedResult((HANDLE)hTap,&ovRead,&readed,FALSE))
			{
				DBGERR("ReadFile failed : {0}") % DBGLASTERROR();
				return 0;
			}
			if(readed==0)
			{
				DBGERR("ReadFile failed : End of stream");
				return 0;
			}
			PushPacket(netif,buffer,readed);

			status = ReadFile((HANDLE)hTap,(LPVOID)buffer,1500,&readed,&ovRead);
			if(!status && GetLastError()!=ERROR_IO_PENDING)
			{
				DBGERR("ReadFile / status={0} / Error={1}") % status % DBGLASTERROR();
				return 0;
			}
			break;

		// A packet has been sended. Check the result.
		case WAIT_OBJECT_0+1:
			pbuf_free(pWriteBuf);
			if(!GetOverlappedResult((HANDLE)hTap,&ovWrite,&written,FALSE))
			{
				DBGERR("WriteFile failed : {0}") % DBGLASTERROR();
				return 0;
			}
			if(written==0)
			{
				DBGERR("WriteFile failed : End of stream");
				return 0;
			}
			pWriteBuf=NULL;
		// A packet is ready to be sent or a packet has been queued for send
		case WAIT_OBJECT_0+2:
			// We are sending a packet. Wait for completion.
			if(pWriteBuf!=NULL) break;

			// Check if a packet is on the queue.
			{
				CriticalSection cs(sg_TapOutq);
				if(!sg_Outq.empty())
				{
					pWriteBuf = sg_Outq.front();
					sg_Outq.pop_front();
				}
				else
					pWriteBuf=NULL;
			} 
			// If a packet is ready to process, send it.
			if(pWriteBuf!=NULL)
			{
				DBGINF("Send Packet {0}") % pWriteBuf->len; 
				status = WriteFile((HANDLE)hTap,(LPVOID)pWriteBuf->payload,pWriteBuf->len,&written,&ovWrite);
				if(!status && GetLastError()!=ERROR_IO_PENDING)
				{
					DBGERR("WriteFile / status={0} / Error={1}") % status % DBGLASTERROR();
					return 0;
				}
			}
			break;

		case WAIT_OBJECT_0+3:
			DBGINF("Exit");
			return 0;
		}
	}
}	

//-----------------------------------------------------------------------------
// Initialize the TAP interface (called by lwIP)
static err_t netif_tap_init(struct netif *netif)
{
	HTAP hTap = (HTAP)netif->state;

	NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 10000);

	netif->hwaddr_len= 6;
	memcpy(netif->hwaddr,sg_TapMacAddr,6);
	// Change the MAC
	netif->hwaddr[4]^=0x8A;
	netif->hwaddr[5]^=0x8A;

	netif->name[0] = 'e';
	netif->name[1] = 'n';
	netif->output		= etharp_output;
	netif->linkoutput	= netif_tap_linkoutput;

	netif->mtu	 = 1500;
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP | NETIF_FLAG_UP;

	DBGINF("Enable IF");
	if(!ChangeMediaStatus(hTap,true))
	{
		DBGERR("ChangeMediaStatus(true) failed: {0}") % DBGLASTERROR();
		return ERR_MEM;
	}

	DBGINF("Start thread");
	sg_hTapThread = StartThread(NULL,0,TapPollThread,netif,0,NULL);
	if(sg_hTapThread==NULL)
	{
		DBGERR("StartThread(TapPollThread) failed: {0}") % DBGLASTERROR();
		return ERR_MEM;
	}

	return ERR_OK;
}

#define USE_DHCP

int _tmain(int argc, _TCHAR* argv[])
{
	bool StartEchoServer();

	//- Create master events
	sg_hevQuit	= CreateEvent(NULL,TRUE, FALSE,NULL);
	sg_hevTapOut= CreateEvent(NULL,FALSE,FALSE,NULL);

	DBGINF("Open TAP device");
	HTAP hTap = OpenTapInterface(NULL,sg_TapMacAddr);
	if(hTap==NULL) return -1;

	// Create queue Critical Section
	InitializeCriticalSection(&sg_TapOutq);

	DBGINF("Initialize lwIP");
	tcpip_init(NULL,NULL);	//< This will start the lwIP main thread

	DBGINF("Attach an interface");

	ip_addr_t tapIP, tapNetmask, tapGw;
#ifdef CANC
	IP4_ADDR(&tapIP,		0,0,0,0);
	IP4_ADDR(&tapGw,		0,0,0,0);
	IP4_ADDR(&tapNetmask,	255,255,255,255);
#else
	IP4_ADDR(&tapIP,		192,168,8,2);
	IP4_ADDR(&tapGw,		192,168,8,1);
	IP4_ADDR(&tapNetmask,	255,255,255,0);
#endif

	// Declare our TAP interface
	struct netif tapIf = {0};
	if(netifapi_netif_add(&tapIf, &tapIP, &tapNetmask, &tapGw, 
		hTap, netif_tap_init,tcpip_input)!=ERR_OK)
	{
		DBGERR("Failed to initialize TAP interface");
		return -1;
	}

#ifdef USE_DHCP
	DBGINF("Start DHCP");
	netifapi_dhcp_start(&tapIf);
#endif

	// Start the ECHO server
	StartEchoServer();

	// Now everything happens in threads
	DBGINF("On");
	_getch();
	SetEvent(sg_hevTapOut);
	DBGINF("Quitting");

	// Try to quit "cleanish" (ok, this is just a prototype)
	if(sg_hTapThread!=NULL)
	{
		WaitForSingleObject(sg_hTapThread,1500);
		CloseHandle(sg_hTapThread);
	}
	CloseTap(hTap);

	return 0;
}

