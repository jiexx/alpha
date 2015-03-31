#pragma once 

#define NO_SYS						0
#define LWIP_CHKSUM_ALGORITHM 		3

#define LWIP_SOCKET					1
#define	LWIP_ARP					1
#define LWIP_NETIF_API				1
#define LWIP_HAVE_LOOPIF			1
#define LWIP_DHCP					1

#define MEMP_NUM_SYS_TIMEOUT        5

#include <errno.h>
#define ENOBUFS			10042
#define EWOULDBLOCK		10043
#define EHOSTUNREACH	10044
#define EINPROGRESS		10045
#define EADDRINUSE		10045
#define EALREADY		10046
#define ECONNABORTED	10047
#define ECONNRESET		10048
#define ENOTCONN		10049
#define	EMSGSIZE		10050
#define EOPNOTSUPP		10051
#define EAFNOSUPPORT	10052
#define ENOPROTOOPT		10053

# define LWIP_DEBUG		1

#ifdef __cplusplus
extern "C" {
#endif
extern	void _lwip_dbgtextout(const char *format,...);
#define LWIP_PLATFORM_DIAG(x) _lwip_dbgtextout x
#ifdef __cplusplus
}
#endif

#ifdef _DEBUG
# define TCP_DEBUG      LWIP_DBG_ON
# define NETIF_DEBUG	LWIP_DBG_ON//LWIP_DBG_OFF
# define ETHARP_DEBUG	LWIP_DBG_OFF
# define DHCP_DEBUG		LWIP_DBG_ON
# define IP_DEBUG		LWIP_DBG_ON//LWIP_DBG_OFF
# define TCPIP_DEBUG	LWIP_DBG_ON
# define API_LIB_DEBUG	LWIP_DBG_ON
#else
# define DHCP_DEBUG		LWIP_DBG_ON
#endif

// Could interact badly with VC CRT
#define LWIP_COMPAT_SOCKETS				0
#define LWIP_POSIX_SOCKETS_IO_NAMES     0