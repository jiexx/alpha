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
#pragma comment (linker, "/export:WSAAsyncSelect=original.WSAAsyncSelect,@101")
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
#pragma comment (linker, "/export:accept=original.accept,@1")
#pragma comment (linker, "/export:bind=original.bind,@2")
#pragma comment (linker, "/export:closesocket=original.closesocket,@3")
#pragma comment (linker, "/export:connect=original.connect,@4")
#pragma comment (linker, "/export:freeaddrinfo=original.freeaddrinfo,@97")
#pragma comment (linker, "/export:getaddrinfo=original.getaddrinfo,@98")
#pragma comment (linker, "/export:gethostbyaddr=original.gethostbyaddr,@51")
#pragma comment (linker, "/export:gethostbyname=original.gethostbyname,@52")
#pragma comment (linker, "/export:gethostname=original.gethostname,@57")
#pragma comment (linker, "/export:getnameinfo=original.getnameinfo,@99")
#pragma comment (linker, "/export:getpeername=original.getpeername,@5")
#pragma comment (linker, "/export:getprotobyname=original.getprotobyname,@53")
#pragma comment (linker, "/export:getprotobynumber=original.getprotobynumber,@54")
#pragma comment (linker, "/export:getservbyname=original.getservbyname,@55")
#pragma comment (linker, "/export:getservbyport=original.getservbyport,@56")
#pragma comment (linker, "/export:getsockname=original.getsockname,@6")
#pragma comment (linker, "/export:getsockopt=original.getsockopt,@7")
#pragma comment (linker, "/export:htonl=original.htonl,@8")
#pragma comment (linker, "/export:htons=original.htons,@9")
#pragma comment (linker, "/export:inet_addr=original.inet_addr,@11")
#pragma comment (linker, "/export:inet_ntoa=original.inet_ntoa,@12")
#pragma comment (linker, "/export:ioctlsocket=original.ioctlsocket,@10")
#pragma comment (linker, "/export:listen=original.listen,@13")
#pragma comment (linker, "/export:ntohl=original.ntohl,@14")
#pragma comment (linker, "/export:ntohs=original.ntohs,@15")
#pragma comment (linker, "/export:recv=original.recv,@16")
#pragma comment (linker, "/export:recvfrom=original.recvfrom,@17")
#pragma comment (linker, "/export:select=original.select,@18")
#pragma comment (linker, "/export:send=original.send,@19")
#pragma comment (linker, "/export:sendto=original.sendto,@20")
#pragma comment (linker, "/export:setsockopt=original.setsockopt,@21")
#pragma comment (linker, "/export:shutdown=original.shutdown,@22")
#pragma comment (linker, "/export:socket=original.socket,@23")



typedef int (WINAPI *PFUN)(int s,const char * buf,int len,int flags);
PFUN mySend,myRecv;

typedef int (WINAPI *PFUN2)(int s,char *buf,int len,int flags,int to,int tolen);
PFUN2 mySendto,myRecvfrom;
void SendData(int cmd,int len,char *pbuffer,int sendORrecv);

HINSTANCE hws2_32;
HWND ServerHwnd;
fstream g_log;

BOOL WINAPI DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved	)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hws2_32=LoadLibrary(L"original.dll");
		mySend=(PFUN)GetProcAddress(hws2_32,"send");
		myRecv=(PFUN)GetProcAddress(hws2_32,"recv");
		mySendto=(PFUN2)GetProcAddress(hws2_32,"sendto");
		myRecvfrom=(PFUN2)GetProcAddress(hws2_32,"recvfrom");
		g_log.open("log.txt", ios::out);
		g_log << "attach ..." << endl;
		break;
	case DLL_PROCESS_DETACH:
		FreeLibrary(hws2_32);
		g_log << "DLL_PROCESS_DETACH." << endl;
		if (g_log.is_open())
			g_log.close();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

int WINAPI send_(int s,char *buf,int len,int flags)
{
	g_log.open("log.txt", ios::out);
	g_log << "send." << endl;
	return mySend(s,buf,len,flags);
}
int WINAPI sendto_(int s,char *buf,int len,int flags,int to,int tolen)
{
	g_log.open("log.txt", ios::out);
	g_log << "sendto." << endl;
	return mySendto(s,buf,len,flags,to,tolen);
}
extern "C" __declspec(dllexport) int WINAPI recv(int s,char *buf,int len,int flags)
{
	int rt;
	g_log << "recv." << endl;
	rt=myRecv(s,buf,len,flags);
	return rt;
}
extern "C" __declspec(dllexport) int WINAPI recvfrom(int s,char *buf,int len,int flags,int from,int fromlen)
{
	int rt;
	g_log << "recvfrom." << endl;
	rt=myRecvfrom(s,buf,len,flags,from,fromlen);
	return rt;
}
#define WSADESCRIPTION_LEN      256
#define WSASYS_STATUS_LEN       128

typedef struct WSAData {
        WORD                    wVersion;
        WORD                    wHighVersion;
#ifdef _WIN64
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char FAR *              lpVendorInfo;
        char                    szDescription[WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[WSASYS_STATUS_LEN+1];
#else
        char                    szDescription[WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[WSASYS_STATUS_LEN+1];
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char FAR *              lpVendorInfo;
#endif
} WSADATA;

typedef WSADATA FAR *LPWSADATA;
extern "C" int __stdcall  WSAStartup(
    WORD wVersionRequested,
   LPWSADATA lpWSAData
)
{
	g_log << "WSAStartup." << endl;
	return -1;
}
int WINAPI WSACleanup_(void)
{
	g_log << "WSACleanup." << endl;
	return 0;
}
