#include "stdafx.h"
#include <fstream>
#include <windows.h>

using namespace std;

#pragma comment (linker, "/export:FreeAddrInfoW=original.FreeAddrInfoW,@27")
#pragma comment (linker, "/export:GetAddrInfoW=original.GetAddrInfoW,@24")
#pragma comment (linker, "/export:GetNameInfoW=original.GetNameInfoW,@25")
#pragma comment (linker, "/export:WEP=original.WEP,@500")
#pragma comment (linker, "/export:WPUCompleteOverlappedRequest=original.WPUCompleteOverlappedRequest,@28")
#pragma comment (linker, "/export:WSAAccept=original.WSAAccept,@29")
#pragma comment (linker, "/export:WSAAddressToStringA=original.WSAAddressToStringA,@30")
#pragma comment (linker, "/export:WSAAddressToStringW=original.WSAAddressToStringW,@31")
#pragma comment (linker, "/export:WSAAsyncGetHostByAddr=original.WSAAsyncGetHostByAddr,@102")
#pragma comment (linker, "/export:WSAAsyncGetHostByName=original.WSAAsyncGetHostByName,@103")
#pragma comment (linker, "/export:WSAAsyncGetProtoByName=original.WSAAsyncGetProtoByName,@105")
#pragma comment (linker, "/export:WSAAsyncGetProtoByNumber=original.WSAAsyncGetProtoByNumber,@104")
#pragma comment (linker, "/export:WSAAsyncGetServByName=original.WSAAsyncGetServByName,@107")
#pragma comment (linker, "/export:WSAAsyncGetServByPort=original.WSAAsyncGetServByPort,@106")
//#pragma comment (linker, "/export:WSAAsyncSelect=original.WSAAsyncSelect,@101")
#pragma comment (linker, "/export:WSACancelAsyncRequest=original.WSACancelAsyncRequest,@108")
#pragma comment (linker, "/export:WSACancelBlockingCall=original.WSACancelBlockingCall,@113")
//#pragma comment (linker, "/export:WSACleanup=original.WSACleanup,@116")
#pragma comment (linker, "/export:WSACloseEvent=original.WSACloseEvent,@32")
#pragma comment (linker, "/export:WSAConnect=original.WSAConnect,@33")
#pragma comment (linker, "/export:WSACreateEvent=original.WSACreateEvent,@34")
#pragma comment (linker, "/export:WSADuplicateSocketA=original.WSADuplicateSocketA,@35")
#pragma comment (linker, "/export:WSADuplicateSocketW=original.WSADuplicateSocketW,@36")
#pragma comment (linker, "/export:WSAEnumNameSpaceProvidersA=original.WSAEnumNameSpaceProvidersA,@37")
#pragma comment (linker, "/export:WSAEnumNameSpaceProvidersW=original.WSAEnumNameSpaceProvidersW,@38")
#pragma comment (linker, "/export:WSAEnumNetworkEvents=original.WSAEnumNetworkEvents,@39")
#pragma comment (linker, "/export:WSAEnumProtocolsA=original.WSAEnumProtocolsA,@40")
#pragma comment (linker, "/export:WSAEnumProtocolsW=original.WSAEnumProtocolsW,@41")
#pragma comment (linker, "/export:WSAEventSelect=original.WSAEventSelect,@42")
#pragma comment (linker, "/export:WSAGetLastError=original.WSAGetLastError,@111")
#pragma comment (linker, "/export:WSAGetOverlappedResult=original.WSAGetOverlappedResult,@43")
#pragma comment (linker, "/export:WSAGetQOSByName=original.WSAGetQOSByName,@44")
#pragma comment (linker, "/export:WSAGetServiceClassInfoA=original.WSAGetServiceClassInfoA,@45")
#pragma comment (linker, "/export:WSAGetServiceClassInfoW=original.WSAGetServiceClassInfoW,@46")
#pragma comment (linker, "/export:WSAGetServiceClassNameByClassIdA=original.WSAGetServiceClassNameByClassIdA,@47")
#pragma comment (linker, "/export:WSAGetServiceClassNameByClassIdW=original.WSAGetServiceClassNameByClassIdW,@48")
#pragma comment (linker, "/export:WSAHtonl=original.WSAHtonl,@49")
#pragma comment (linker, "/export:WSAHtons=original.WSAHtons,@50")
#pragma comment (linker, "/export:WSAInstallServiceClassA=original.WSAInstallServiceClassA,@58")
#pragma comment (linker, "/export:WSAInstallServiceClassW=original.WSAInstallServiceClassW,@59")
#pragma comment (linker, "/export:WSAIoctl=original.WSAIoctl,@60")
#pragma comment (linker, "/export:WSAIsBlocking=original.WSAIsBlocking,@114")
#pragma comment (linker, "/export:WSAJoinLeaf=original.WSAJoinLeaf,@61")
#pragma comment (linker, "/export:WSALookupServiceBeginA=original.WSALookupServiceBeginA,@62")
#pragma comment (linker, "/export:WSALookupServiceBeginW=original.WSALookupServiceBeginW,@63")
#pragma comment (linker, "/export:WSALookupServiceEnd=original.WSALookupServiceEnd,@64")
#pragma comment (linker, "/export:WSALookupServiceNextA=original.WSALookupServiceNextA,@65")
#pragma comment (linker, "/export:WSALookupServiceNextW=original.WSALookupServiceNextW,@66")
#pragma comment (linker, "/export:WSANSPIoctl=original.WSANSPIoctl,@67")
#pragma comment (linker, "/export:WSANtohl=original.WSANtohl,@68")
#pragma comment (linker, "/export:WSANtohs=original.WSANtohs,@69")
#pragma comment (linker, "/export:WSAProviderConfigChange=original.WSAProviderConfigChange,@70")
#pragma comment (linker, "/export:WSARecv=original.WSARecv,@71")
#pragma comment (linker, "/export:WSARecvDisconnect=original.WSARecvDisconnect,@72")
#pragma comment (linker, "/export:WSARecvFrom=original.WSARecvFrom,@73")
#pragma comment (linker, "/export:WSARemoveServiceClass=original.WSARemoveServiceClass,@74")
#pragma comment (linker, "/export:WSAResetEvent=original.WSAResetEvent,@75")
#pragma comment (linker, "/export:WSASend=original.WSASend,@76")
#pragma comment (linker, "/export:WSASendDisconnect=original.WSASendDisconnect,@77")
#pragma comment (linker, "/export:WSASendTo=original.WSASendTo,@78")
#pragma comment (linker, "/export:WSASetBlockingHook=original.WSASetBlockingHook,@109")
#pragma comment (linker, "/export:WSASetEvent=original.WSASetEvent,@79")
#pragma comment (linker, "/export:WSASetLastError=original.WSASetLastError,@112")
#pragma comment (linker, "/export:WSASetServiceA=original.WSASetServiceA,@80")
#pragma comment (linker, "/export:WSASetServiceW=original.WSASetServiceW,@81")
#pragma comment (linker, "/export:WSASocketA=original.WSASocketA,@82")
#pragma comment (linker, "/export:WSASocketW=original.WSASocketW,@83")
//#pragma comment (linker, "/export:WSAStartup=original.WSAStartup,@115")
#pragma comment (linker, "/export:WSAStringToAddressA=original.WSAStringToAddressA,@84")
#pragma comment (linker, "/export:WSAStringToAddressW=original.WSAStringToAddressW,@85")
#pragma comment (linker, "/export:WSAUnhookBlockingHook=original.WSAUnhookBlockingHook,@110")
#pragma comment (linker, "/export:WSAWaitForMultipleEvents=original.WSAWaitForMultipleEvents,@86")
#pragma comment (linker, "/export:WSApSetPostRoutine=original.WSApSetPostRoutine,@26")
#pragma comment (linker, "/export:WSCDeinstallProvider=original.WSCDeinstallProvider,@87")
#pragma comment (linker, "/export:WSCEnableNSProvider=original.WSCEnableNSProvider,@88")
#pragma comment (linker, "/export:WSCEnumProtocols=original.WSCEnumProtocols,@89")
#pragma comment (linker, "/export:WSCGetProviderPath=original.WSCGetProviderPath,@90")
#pragma comment (linker, "/export:WSCInstallNameSpace=original.WSCInstallNameSpace,@91")
#pragma comment (linker, "/export:WSCInstallProvider=original.WSCInstallProvider,@92")
#pragma comment (linker, "/export:WSCUnInstallNameSpace=original.WSCUnInstallNameSpace,@93")
#pragma comment (linker, "/export:WSCUpdateProvider=original.WSCUpdateProvider,@94")
#pragma comment (linker, "/export:WSCWriteNameSpaceOrder=original.WSCWriteNameSpaceOrder,@95")
#pragma comment (linker, "/export:WSCWriteProviderOrder=original.WSCWriteProviderOrder,@96")
#pragma comment (linker, "/export:__WSAFDIsSet=original.__WSAFDIsSet,@151")
//#pragma comment (linker, "/export:accept=original.accept,@1")
//#pragma comment (linker, "/export:bind=original.bind,@2")
//#pragma comment (linker, "/export:closesocket=original.closesocket,@3")
//#pragma comment (linker, "/export:connect=original.connect,@4")
#pragma comment (linker, "/export:freeaddrinfo=original.freeaddrinfo,@97")
#pragma comment (linker, "/export:getaddrinfo=original.getaddrinfo,@98")
#pragma comment (linker, "/export:gethostbyaddr=original.gethostbyaddr,@51")
//#pragma comment (linker, "/export:gethostbyname=original.gethostbyname,@52")
#pragma comment (linker, "/export:gethostname=original.gethostname,@57")
#pragma comment (linker, "/export:getnameinfo=original.getnameinfo,@99")
//#pragma comment (linker, "/export:getpeername=original.getpeername,@5")
#pragma comment (linker, "/export:getprotobyname=original.getprotobyname,@53")
#pragma comment (linker, "/export:getprotobynumber=original.getprotobynumber,@54")
#pragma comment (linker, "/export:getservbyname=original.getservbyname,@55")
#pragma comment (linker, "/export:getservbyport=original.getservbyport,@56")
//#pragma comment (linker, "/export:getsockname=original.getsockname,@6")
#pragma comment (linker, "/export:getsockopt=original.getsockopt,@7")
//#pragma comment (linker, "/export:htonl=original.htonl,@8")
//#pragma comment (linker, "/export:htons=original.htons,@9")
//#pragma comment (linker, "/export:inet_addr=original.inet_addr,@11")
//#pragma comment (linker, "/export:inet_ntoa=original.inet_ntoa,@12")
//#pragma comment (linker, "/export:ioctlsocket=original.ioctlsocket,@10")
#pragma comment (linker, "/export:listen=original.listen,@13")
#pragma comment (linker, "/export:ntohl=original.ntohl,@14")
//#pragma comment (linker, "/export:ntohs=original.ntohs,@15")
//#pragma comment (linker, "/export:recv=original.recv,@16")
//#pragma comment (linker, "/export:recvfrom=original.recvfrom,@17")
#pragma comment (linker, "/export:select=original.select,@18")
//#pragma comment (linker, "/export:send=original.send,@19")
//#pragma comment (linker, "/export:sendto=original.sendto,@20")
#pragma comment (linker, "/export:setsockopt=original.setsockopt,@21")
#pragma comment (linker, "/export:shutdown=original.shutdown,@22")
//#pragma comment (linker, "/export:socket=original.socket,@23")



typedef int				(__stdcall *PFUNC_WSAStartup)		(WORD wVersionRequested, LPWSADATA lpWSAData);
typedef SOCKET			(__stdcall *PFUNC_socket)			(int af, int type, int protocol);
typedef int				(__stdcall *PFUNC_connect)			(SOCKET s, const struct sockaddr *name, int namelen);
typedef int				(__stdcall *PFUNC_send)				(SOCKET s, const char* buf, int len, int flags);
typedef int				(__stdcall *PFUNC_sendto)			(SOCKET s, const char* buf, int len, int flags, const struct sockaddr *to, int tolen);
typedef int				(__stdcall *PFUNC_recv)				(SOCKET s, char FAR * buf, int len, int flags);
typedef int				(__stdcall *PFUNC_recvfrom)			(SOCKET s, char FAR * buf, int len, int flags, struct sockaddr* from, int* fromlen);
typedef int				(__stdcall *PFUNC_bind)				(SOCKET s, const struct sockaddr* addr, int namelen);
typedef int				(__stdcall *PFUNC_WSAAsyncSelect)	(SOCKET s, HWND hWnd, unsigned int wMsg, long lEvent);
typedef SOCKET			(__stdcall *PFUNC_accept)			(SOCKET s, struct sockaddr* addr, int* addrlen);
typedef int				(__stdcall *PFUNC_getpeername)		(SOCKET s, struct sockaddr* name, int* namelen);
typedef int				(__stdcall *PFUNC_getsockname)		(SOCKET s, struct sockaddr* name, int* namelen);
typedef struct hostent*	(__stdcall *PFUNC_gethostbyname)	(const char* name);
typedef unsigned long	(__stdcall *PFUNC_htonl)			(unsigned long hostlong);
typedef unsigned short	(__stdcall *PFUNC_htons)			(unsigned short hostshort);
typedef unsigned long	(__stdcall *PFUNC_inet_addr)		(const char* cp);
typedef char*			(__stdcall *PFUNC_inet_ntoa)		(__in struct in_addr in);
typedef int				(__stdcall *PFUNC_ioctlsocket)		(SOCKET s, long cmd, unsigned long* argp);
typedef unsigned short	(__stdcall *PFUNC_ntohs)			(unsigned short netshort);
typedef int				(__stdcall *PFUNC_closesocket)		(SOCKET s);
typedef int				(__stdcall *PFUNC_WSACleanup)		(void);

PFUNC_WSAStartup		_WSAStartup;
PFUNC_socket			_socket;
PFUNC_connect			_connect;
PFUNC_send				_send;
PFUNC_sendto			_sendto;
PFUNC_recv				_recv;
PFUNC_recvfrom			_recvfrom;
PFUNC_bind				_bind;
PFUNC_WSAAsyncSelect	_WSAAsyncSelect;
PFUNC_accept			_accept;
PFUNC_getpeername		_getpeername;
PFUNC_getsockname		_getsockname;
PFUNC_gethostbyname		_gethostbyname;
PFUNC_htonl				_htonl;
PFUNC_htons				_htons;
PFUNC_inet_addr			_inet_addr;
PFUNC_inet_ntoa			_inet_ntoa;
PFUNC_ioctlsocket		_ioctlsocket;
PFUNC_ntohs				_ntohs;
PFUNC_closesocket		_closesocket;
PFUNC_WSACleanup		_WSACleanup;


HINSTANCE hws2_32;
fstream g_log;

BOOL WINAPI DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved	)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hws2_32=LoadLibrary(L"original.dll");
		_WSAStartup     = (PFUNC_WSAStartup    )GetProcAddress(hws2_32,"WSAStartup");
		_socket         = (PFUNC_socket        )GetProcAddress(hws2_32,"socket");    
		_connect        = (PFUNC_connect       )GetProcAddress(hws2_32,"connect");   
		_send           = (PFUNC_send          )GetProcAddress(hws2_32,"send");      
		_sendto         = (PFUNC_sendto        )GetProcAddress(hws2_32,"sendto");
		_recv           = (PFUNC_recv          )GetProcAddress(hws2_32,"recv");  
		_recvfrom       = (PFUNC_recvfrom      )GetProcAddress(hws2_32,"recvfrom");  
		_bind           = (PFUNC_bind          )GetProcAddress(hws2_32,"bind");      
		_WSAAsyncSelect = (PFUNC_WSAAsyncSelect)GetProcAddress(hws2_32,"WSAAsyncSelect");
		_accept         = (PFUNC_accept        )GetProcAddress(hws2_32,"accept");    
		_getpeername    = (PFUNC_getpeername   )GetProcAddress(hws2_32,"getpeername");
		_getsockname    = (PFUNC_getsockname   )GetProcAddress(hws2_32,"getsockname"); 
		_gethostbyname  = (PFUNC_gethostbyname )GetProcAddress(hws2_32,"gethostbyname");
		_htonl          = (PFUNC_htonl         )GetProcAddress(hws2_32,"htonl");     
		_htons          = (PFUNC_htons         )GetProcAddress(hws2_32,"htons");     
		_inet_addr      = (PFUNC_inet_addr     )GetProcAddress(hws2_32,"inet_addr"); 
		_inet_ntoa      = (PFUNC_inet_ntoa     )GetProcAddress(hws2_32,"inet_ntoa"); 
		_ioctlsocket    = (PFUNC_ioctlsocket   )GetProcAddress(hws2_32,"ioctlsocket"); 
		_ntohs          = (PFUNC_ntohs         )GetProcAddress(hws2_32,"ntohs");     
		_closesocket    = (PFUNC_closesocket   )GetProcAddress(hws2_32,"closesocket");
		_WSACleanup     = (PFUNC_WSACleanup    )GetProcAddress(hws2_32,"WSACleanup");
		g_log.open("c:\\log.txt", ios::out);
		g_log << "attach ..." << endl;
		break;
	case DLL_PROCESS_DETACH:
		FreeLibrary(hws2_32);
		g_log << "DLL_PROCESS_DETACH." << endl;
		g_log.flush();
		g_log.close();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

extern "C" void __stdcall log(const char* fmt, ...) 
{

}

extern "C" int __stdcall  WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
{
	g_log << "WSAStartup: " << endl;
	return _WSAStartup(wVersionRequested, lpWSAData);
}
extern "C"  SOCKET __stdcall socket (int af, int type, int protocol)
{
	g_log << "socket: " << endl;
	return _socket (af, type, protocol);
}
extern "C" int __stdcall connect(SOCKET s, const struct sockaddr* name, int namelen)
{
	g_log << "connect: ";
	g_log.write(name->sa_data, 14);
	g_log << endl;
	return _connect(s, name, namelen);
}
extern "C" int __stdcall send (SOCKET s, const char* buf, int len, int flags)
{
	g_log << "sendto: ";
	g_log.write(buf, len);
	g_log << endl;
	return _send(s, buf, len, flags);
}
extern "C" int __stdcall sendto (SOCKET s, const char* buf, int len, int flags, const struct sockaddr *to, int tolen)
{
	g_log << "sendto. ";
	g_log.write(buf, len);
	g_log << endl;
	return _sendto(s, buf, len, flags, to, tolen);
}
extern "C" int __stdcall recv (SOCKET s, char* buf, int len, int flags)
{
	int r = _recv (s, buf, len, flags);
	g_log << "recv: " << endl;
	return r;
}
extern "C" int __stdcall recvfrom (SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen)
{ 
	int r = _recvfrom (s, buf, len, flags, from, fromlen);
	g_log << "recvfrom: " << endl;
	return r;
}
extern "C" int __stdcall bind (SOCKET s, const struct sockaddr* addr, int namelen)
{
	g_log << "bind: " << endl;
	return _bind (s, addr, namelen);
}
extern "C" int __stdcall WSAAsyncSelect(SOCKET s, HWND hWnd, unsigned int wMsg, long lEvent)
{
	int r = _WSAAsyncSelect(s, hWnd, wMsg, lEvent);
	g_log << "WSAAsyncSelect: " << endl;
	return r;
}
extern "C" SOCKET __stdcall accept (SOCKET s, struct sockaddr* addr, int* addrlen)
{
	int r = _accept (s, addr, addrlen);
	g_log << "accept: " << endl;
	return r;
}
extern "C" int __stdcall getpeername(SOCKET s, struct sockaddr* name, int* namelen)
{
	g_log << "getpeername: " << endl;
	return _getpeername(s, name, namelen);
}
extern "C" int __stdcall getsockname(SOCKET s, struct sockaddr* name, int* namelen)
{
	g_log << "getsockname: " << endl;
	return _getsockname(s, name, namelen);
}
extern "C" struct hostent* __stdcall gethostbyname(const char* name)
{
	g_log << "gethostbyname: " << name << endl;
	return _gethostbyname(name);
}
extern "C" unsigned long __stdcall htonl(unsigned long hostlong)
{
	g_log << "htonl: " << hostlong << endl;
	return _htonl(hostlong);
}
extern "C" unsigned short __stdcall htons (unsigned short hostshort)
{
	g_log << "htons: " << hostshort << endl;
	return _htons(hostshort);
}
extern "C" unsigned long __stdcall inet_addr (const char* cp)
{
	g_log << "inet_addr: " << cp << endl;
	return _inet_addr(cp);
}
extern "C" char* __stdcall inet_ntoa (struct in_addr in)
{
	g_log << "inet_ntoa: " << endl;
	return _inet_ntoa(in);
}
extern "C" int __stdcall ioctlsocket (SOCKET s, long cmd, unsigned long* argp)
{
	g_log << "ioctlsocket: " << cmd << endl;
	return _ioctlsocket(s, cmd, argp);
}
extern "C" unsigned short __stdcall ntohs (unsigned short netshort)
{
	g_log << "ntohs: " << netshort << endl;
	return _ntohs(netshort);
}
extern "C" int __stdcall closesocket ( IN SOCKET s)
{
	g_log << "closesocket: " << endl;
	return _closesocket(s);
}
extern "C" int __stdcall WSACleanup(void)
{
	g_log << "WSACleanup: " << endl;
	return _WSACleanup();
}