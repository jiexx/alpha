#include "stdafx.h"
#include <fstream>
using namespace std;

#pragma comment(linker,"/export:accept=syswsock32.accept,@1")
#pragma comment(linker,"/export:bind=syswsock32.bind,@2")
#pragma comment(linker,"/export:closesocket=syswsock32.closesocket,@3")
#pragma comment(linker,"/export:connect=syswsock32.connect,@4")
#pragma comment(linker,"/export:getpeername=syswsock32.getpeername,@5")
#pragma comment(linker,"/export:getsockname=syswsock32.getsockname,@6")
#pragma comment(linker,"/export:getsockopt=syswsock32.getsockopt,@7")
#pragma comment(linker,"/export:htons=syswsock32.htons,@9")
#pragma comment(linker,"/export:htonl=syswsock32.htonl,@8")
#pragma comment(linker,"/export:inet_addr=syswsock32.inet_addr,@10")
#pragma comment(linker,"/export:inet_ntoa=syswsock32.inet_ntoa,@11")
#pragma comment(linker,"/export:ioctlsocket=syswsock32.ioctlsocket,@12")
#pragma comment(linker,"/export:listen=syswsock32.listen,@13")
#pragma comment(linker,"/export:ntohl=syswsock32.ntohl,@14")
#pragma comment(linker,"/export:ntohs=syswsock32.ntohs,@15")
#pragma comment(linker,"/export:recv=_recv@16,@16")
#pragma comment(linker,"/export:recvfrom=_recvfrom@24,@17")
#pragma comment(linker,"/export:select=syswsock32.select,@18")
#pragma comment(linker,"/export:send=_send@16,@19")
#pragma comment(linker,"/export:sendto=_sendto@24,@20")
#pragma comment(linker,"/export:setsockopt=syswsock32.setsockopt,@21")
#pragma comment(linker,"/export:socket=syswsock32.socket,@23")
#pragma comment(linker,"/export:shutdown=syswsock32.shutdown,@22")
#pragma comment(linker,"/export:MigrateWinsockConfiguration=syswsock32.MigrateWinsockConfiguration,@24")

#pragma comment(linker,"/export:gethostbyname=syswsock32.gethostbyname,@52")
#pragma comment(linker,"/export:gethostbyaddr=syswsock32.gethostbyaddr,@51")
#pragma comment(linker,"/export:getprotobynumber=syswsock32.getprotobynumber,@54")
#pragma comment(linker,"/export:getprotobyname=syswsock32.getprotobyname,@53")
#pragma comment(linker,"/export:getservbyport=syswsock32.getservbyport,@56")
#pragma comment(linker,"/export:getservbyname=syswsock32.getservbyname,@55")
#pragma comment(linker,"/export:gethostname=syswsock32.gethostname,@57")

#pragma comment(linker,"/export:WSAAsyncSelect=syswsock32.WSAAsyncSelect,@101")
#pragma comment(linker,"/export:WSAAsyncGetHostByName=syswsock32.WSAAsyncGetHostByName,@103")
#pragma comment(linker,"/export:WSAAsyncGetHostByAddr=syswsock32.WSAAsyncGetHostByAddr,@102")
#pragma comment(linker,"/export:WSAAsyncGetProtoByNumber=syswsock32.WSAAsyncGetProtoByNumber,@104")
#pragma comment(linker,"/export:WSAAsyncGetProtoByName=syswsock32.WSAAsyncGetProtoByName,@105")
#pragma comment(linker,"/export:WSAAsyncGetServByPort=syswsock32.WSAAsyncGetServByPort,@106")
#pragma comment(linker,"/export:WSAAsyncGetServByName=syswsock32.WSAAsyncGetServByName,@107")
#pragma comment(linker,"/export:WSACancelAsyncRequest=syswsock32.WSACancelAsyncRequest,@108")
#pragma comment(linker,"/export:WSASetBlockingHook=syswsock32.WSASetBlockingHook,@109")
#pragma comment(linker,"/export:WSAUnhookBlockingHook=syswsock32.WSAUnhookBlockingHook,@110")
#pragma comment(linker,"/export:WSAGetLastError=syswsock32.WSAGetLastError,@111")
#pragma comment(linker,"/export:WSASetLastError=syswsock32.WSASetLastError,@112")
#pragma comment(linker,"/export:WSACancelBlockingCall=syswsock32.WSACancelBlockingCall,@113")
#pragma comment(linker,"/export:WSAIsBlocking=syswsock32.WSAIsBlocking,@114")
#pragma comment(linker,"/export:WSAStartup=_WSAStartup@8,@115")
#pragma comment(linker,"/export:WSACleanup=_WSACleanup@0,@116")
#pragma comment(linker,"/export:___WSAFDIsSet=syswsock32.__WSAFDIsSet,@151")


#pragma comment(linker,"/export:WSARecvEx=syswsock32.WSARecvEx,@1107")
#pragma comment(linker,"/export:WSApSetPostRoutine=syswsock32.WSApSetPostRoutine,@1000")
#pragma comment(linker,"/export:WEP=syswsock32.WEP,@500")
#pragma comment(linker,"/export:TransmitFile=syswsock32.TransmitFile,@1140")
#pragma comment(linker,"/export:SetServiceW=syswsock32.SetServiceW,@1118")
#pragma comment(linker,"/export:SetServiceA=syswsock32.SetServiceA,@1117")
#pragma comment(linker,"/export:sethostname=syswsock32.sethostname,@1105")

#pragma comment(linker,"/export:s_perror=syswsock32.s_perror,@1108")
#pragma comment(linker,"/export:rresvport=syswsock32.rresvport,@1104")
#pragma comment(linker,"/export:rexec=syswsock32.rexec,@1103")
#pragma comment(linker,"/export:rcmd=syswsock32.rcmd,@1102")
#pragma comment(linker,"/export:NPLoadNameSpaces=syswsock32.NPLoadNameSpaces,@1130")

#pragma comment(linker,"/export:inet_network=syswsock32.inet_network,@1100")
#pragma comment(linker,"/export:GetTypeByNameW=syswsock32.GetTypeByNameW,@1114")
#pragma comment(linker,"/export:GetTypeByNameA=syswsock32.GetTypeByNameA,@1113")
#pragma comment(linker,"/export:GetServiceW=syswsock32.GetServiceW,@1120")
#pragma comment(linker,"/export:GetServiceA=syswsock32.GetServiceA,@1119")
#pragma comment(linker,"/export:getnetbyname=syswsock32.getnetbyname,@1101")
#pragma comment(linker,"/export:GetNameByTypeW=syswsock32.GetNameByTypeW,@1116")
#pragma comment(linker,"/export:GetNameByTypeA=syswsock32.GetNameByTypeA,@1115")
#pragma comment(linker,"/export:GetAddressByNameW=syswsock32.GetAddressByNameW,@1110")
#pragma comment(linker,"/export:GetAddressByNameA=syswsock32.GetAddressByNameA,@1109")
#pragma comment(linker,"/export:GetAcceptExSockaddrs=syswsock32.GetAcceptExSockaddrs,@1142")
#pragma comment(linker,"/export:EnumProtocolsW=syswsock32.EnumProtocolsW,@1112")
#pragma comment(linker,"/export:EnumProtocolsA=syswsock32.EnumProtocolsA,@1111")
#pragma comment(linker,"/export:dn_expand=syswsock32.dn_expand,@1106")
#pragma comment(linker,"/export:AcceptEx=syswsock32.AcceptEx,@1141")



typedef int (WINAPI *PFUN)(int s,const char * buf,int len,int flags);
PFUN mySend,myRecv;

typedef int (WINAPI *PFUN2)(int s,char *buf,int len,int flags,int to,int tolen);
PFUN2 mySendto,myRecvfrom;
void SendData(int cmd,int len,char *pbuffer,int sendORrecv);

HINSTANCE hws2_32;
HWND ServerHwnd;
fstream g_log;

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved	)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hws2_32=LoadLibrary(L"syswsock32.dll");
		mySend=(PFUN)GetProcAddress(hws2_32,"send");
		myRecv=(PFUN)GetProcAddress(hws2_32,"recv");
		mySendto=(PFUN2)GetProcAddress(hws2_32,"sendto");
		myRecvfrom=(PFUN2)GetProcAddress(hws2_32,"recvfrom");
		g_log.open("log.txt", ios::out);
		g_log << "attach ..." << endl;
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) int WINAPI send(int s,char *buf,int len,int flags)
{
	g_log.open("log.txt", ios::out);
	g_log << "send." << endl;
	return mySend(s,buf,len,flags);
}
extern "C" __declspec(dllexport) int WINAPI sendto(int s,char *buf,int len,int flags,int to,int tolen)
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
extern "C" __declspec(dllexport) int WINAPI WSAStartup(
  _In_   WORD wVersionRequested,
  _Out_  LPWSADATA lpWSAData
)
{
	g_log.open("log.txt", ios::out);
	g_log << "WSAStartup." << endl;
	return 0;
}
extern "C" __declspec(dllexport) int WINAPI WSACleanup(void)
{
	g_log.open("log.txt", ios::out);
	g_log << "WSACleanup." << endl;
	return 0;
}
