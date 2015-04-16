#pragma once


//.............................................................................
//> Check for Multi-Threaded runtime
#ifndef  _MT
#error "Configuration: Multithreading is not activated !"
#endif

//.............................................................................
//> Set Win32 API to 5.2 (Win2K3/WinXP SP2)
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0502

//.............................................................................
//> Ensure UNICODE runtime
#ifndef		_UNICODE
# define	_UNICODE
#endif //<  _UNICODE
#ifndef		UNICODE
# define	UNICODE
#endif //<  UNICODE

//.............................................................................
//> Don't include unused API
#define NOCOMM				// Remove serial port API
#define NOMCX				// Remove MODEM API

//.............................................................................
//> C++ CRT
#include <stdio.h>

//.............................................................................
//> Windows API includes
#define _WINSOCKAPI_
#include <windows.h>		// Windows main include
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
typedef UINT_PTR        SOCKET;
struct  hostent {
        char    * h_name;           /* official name of host */
        char    ** h_aliases;  /* alias list */
        short   h_addrtype;             /* host address type */
        short   h_length;               /* length of address */
        char    ** h_addr_list; /* list of addresses */
#define h_addr  h_addr_list[0]          /* address, for backward compat */
};
typedef struct in_addr {
        union {
                struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { USHORT s_w1,s_w2; } S_un_w;
                ULONG S_addr;
        } S_un;
#define s_addr  S_un.S_addr /* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2    // host on imp
#define s_net   S_un.S_un_b.s_b1    // network
#define s_imp   S_un.S_un_w.s_w2    // imp
#define s_impno S_un.S_un_b.s_b4    // imp #
#define s_lh    S_un.S_un_b.s_b3    // logical host
} IN_ADDR, *PIN_ADDR, FAR *LPIN_ADDR;
struct sockaddr {
        unsigned short sa_family;              /* address family */
        char    sa_data[14];            /* up to 14 bytes of direct address */
};