/**************************************************************************
 *
 * IP Host Demo
 * ....................................
 * Precompiled header 
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
//=============================================================================
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

//.... Microsoft VC++ extras ..................................................
#pragma warning(push)
#pragma warning(disable:4995)	//< warning C4995: 'wcscpy': name was marked as #pragma deprecated
#include <comdef.h>			// COM types
#include <atlbase.h>		// ATL base template
#pragma warning(pop)

//.............................................................................
//> Ensure link of Windows API needed libraries
#pragma comment(lib,"ole32.lib")	// COM/OLE
#pragma comment(lib,"oleaut32.lib")	// COM/OLE Automation

//.............................................................................
//> Map the "_beginthreadex" to the native CreateThread call
inline HANDLE StartThread(
			LPSECURITY_ATTRIBUTES lpThreadAttributes,
			DWORD dwStackSize,
			LPTHREAD_START_ROUTINE lpStartAddress,
			LPVOID lpParameter,
			DWORD dwCreationFlags,
			LPDWORD lpThreadId )
{
	typedef unsigned ( __stdcall *start_address )( void * );

	return (HANDLE)_beginthreadex(
			(void *)lpThreadAttributes,
			(unsigned)dwStackSize,
			(start_address)lpStartAddress,
			(void *)lpParameter,
			(unsigned)dwCreationFlags, 
			(unsigned *)lpThreadId );
}

//.............................................................................
//> Critical Section handler
class CriticalSection
{
	CRITICAL_SECTION *_cs;
public:
	CriticalSection(CRITICAL_SECTION &cs) : _cs(&cs)
	{ EnterCriticalSection(_cs); }
	~CriticalSection()
	{ LeaveCriticalSection(_cs); }
};