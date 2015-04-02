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
#include <tchar.h>
#include <conio.h>
#include <time.h>
#include <locale.h>
#include <malloc.h>
#include <stdarg.h>

//.... STL Library ............................................................
#include <exception>
#include <vector>
#include <list>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <limits>
#include <memory>
#include <sstream>
#include <algorithm>

//.............................................................................
//> Windows API includes
#include <windows.h>		// Windows main include
#include <Strsafe.h>		// Safe string functions
#include <ole2.h>			// OLE-2 Main Header
#include <olectl.h>			// OLE-2 Control interfaces
#include <objbase.h>		// COM Base API
#include <objidl.h>			// OLE-2 IDL definition