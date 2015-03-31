/**************************************************************************
 *
 * IP Host Demo
 * ....................................
 * TAP Interface
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

//=============
// TAP IOCTLs
//=============

#define TAP_CONTROL_CODE(request,method) \
  CTL_CODE (FILE_DEVICE_UNKNOWN, request, method, FILE_ANY_ACCESS)

// Present in 8.1

#define TAP_IOCTL_GET_MAC               TAP_CONTROL_CODE (1, METHOD_BUFFERED)
#define TAP_IOCTL_GET_VERSION           TAP_CONTROL_CODE (2, METHOD_BUFFERED)
#define TAP_IOCTL_GET_MTU               TAP_CONTROL_CODE (3, METHOD_BUFFERED)
#define TAP_IOCTL_GET_INFO              TAP_CONTROL_CODE (4, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_POINT_TO_POINT TAP_CONTROL_CODE (5, METHOD_BUFFERED)
#define TAP_IOCTL_SET_MEDIA_STATUS      TAP_CONTROL_CODE (6, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_DHCP_MASQ      TAP_CONTROL_CODE (7, METHOD_BUFFERED)
#define TAP_IOCTL_GET_LOG_LINE          TAP_CONTROL_CODE (8, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_DHCP_SET_OPT   TAP_CONTROL_CODE (9, METHOD_BUFFERED)

// Added in 8.2

/* obsoletes TAP_IOCTL_CONFIG_POINT_TO_POINT */
#define TAP_IOCTL_CONFIG_TUN            TAP_CONTROL_CODE (10, METHOD_BUFFERED)

int execve (const wchar_t *a, int cnt, ...)
{
	int ret = -1;
	static bool exec_warn = false;

	if (a)
	{
		STARTUPINFOW start_info;
		PROCESS_INFORMATION proc_info;

		WCHAR cl[1024] = {0};
		WCHAR cmd[] = {L"command"};
		TCHAR windir[MAX_PATH];
		GetWindowsDirectory(windir, MAX_PATH);
		wsprintf( cl, L"%s\\system32\\%s", windir, a);
		va_list valist;
		va_start(valist, cnt); 
		for(int i = 0; i < cnt; i++)
		{
			wchar_t* value = va_arg(valist,wchar_t*);
			wcscat( cl, value );
		}
		va_end(valist);

		/* this allows console programs to run, and is ignored otherwise */
		DWORD proc_flags = CREATE_NO_WINDOW;

		memset(&start_info, 0, sizeof(start_info));
		memset(&proc_info, 0, sizeof(proc_info));

		/* fill in STARTUPINFO struct */
		GetStartupInfoW(&start_info);
		start_info.cb = sizeof(start_info);
		start_info.dwFlags = STARTF_USESHOWWINDOW;
		start_info.wShowWindow = SW_HIDE;
		if (ret = CreateProcessW (cmd, cl, NULL, NULL, FALSE, proc_flags, NULL, NULL, &start_info, &proc_info))
		{
			DWORD exit_status = 0;
			CloseHandle (proc_info.hThread);
			WaitForSingleObject (proc_info.hProcess, INFINITE);
			if (GetExitCodeProcess (proc_info.hProcess, &exit_status))
				ret = (int)exit_status;
			else
				DBGERR("execve: GetExitCodeProcess {0} failed") % a;
			CloseHandle (proc_info.hProcess);
		}
		else
		{
			DBGINF("execve: change ip success");
		}
	}
	else
	{
		DBGERR("execve: {0} args invalid.") % a;
	}
	return ret;
}

static wchar_t g_name[64];
inline int get_tap_name( wchar_t* id, wchar_t* name ) {
	wchar_t subKeyName[1024];
	wcscpy(subKeyName,L"SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\");
	wcscat(subKeyName,id);
	wcscat(subKeyName,L"\\Connection");
	DWORD szValue = 64;

	HKEY hkCnxInf;
	if(RegOpenKeyExW(HKEY_LOCAL_MACHINE,subKeyName,0,KEY_READ,&hkCnxInf)) return 1;

	if(RegQueryValueExW(hkCnxInf,L"Name",NULL,NULL,(LPBYTE)name,&szValue)) 
	{
		RegCloseKey(hkCnxInf);
		return 1;
	}
	RegCloseKey(hkCnxInf);
	return 0;
}

HTAP OpenTapInterface(const char *Name,const unsigned char MacAddr[6])
{
	static const wchar_t NetworkDrvs[] = {L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}"};
	HKEY hkNets;
	wchar_t wName[128];

	if(Name && *Name)
	{
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,Name,-1,wName,sizeof(wName)/sizeof(wchar_t));
	}
	else
		wName[0]=0;


	if(RegOpenKeyExW(HKEY_LOCAL_MACHINE,NetworkDrvs,0,KEY_READ,&hkNets))
	{
		DBGERR("Fail to open network connection: {0}") % DBGLASTERROR();
		return NULL;
	}
	wchar_t name[255],value[255],id[255];
	bool found =false;
	for(DWORD i=0;!RegEnumKey(hkNets,i,name,sizeof(name));i++)
	{
		HKEY hkInf;
		if(RegOpenKeyExW(hkNets,name,0,KEY_READ,&hkInf)) continue;

		DWORD szValue = sizeof(value);
		if(RegQueryValueExW(hkInf,L"ComponentId",NULL,NULL,(LPBYTE)value,&szValue)) goto contSearch;
		if(szValue<8 || _wcsnicmp(value,L"tap0901",4)) continue; // Not a TAP

		szValue = sizeof(id);
		if(RegQueryValueEx(hkInf,L"NetCfgInstanceId",NULL,NULL,(LPBYTE)id,&szValue)) goto contSearch;

		if(*wName)
		{
			if(get_tap_name(id, wName))
			{
				goto contSearch;
			}
			if(!_wcsicmp(value, wName)) 
			{
				RegCloseKey(hkInf);
				found =true;
				break;
			}
		}
		else
		{
			RegCloseKey(hkInf);
			found =true;
			break;
		}
contSearch:
		RegCloseKey(hkInf);
	}
	RegCloseKey(hkNets);

	if(!found)
	{
		DBGERR("Unable to find a TAP interface");
		return NULL;
	}

	wchar_t tapPath[MAX_PATH];
	wcscpy(tapPath,L"\\\\.\\Global\\");
	wcscat(tapPath,id);
	wcscat(tapPath,L".tap");

	DBGINF("Tap Interface Path [{0}]") % tapPath;

	HANDLE hTap = CreateFileW(tapPath,GENERIC_WRITE|GENERIC_READ,0,NULL,OPEN_EXISTING,
		FILE_ATTRIBUTE_SYSTEM|FILE_FLAG_OVERLAPPED,NULL);
	if(hTap==INVALID_HANDLE_VALUE)
	{
		DBGERR("Unable to open TAP interface: {0}") % DBGLASTERROR();
		return NULL;
	}

	DWORD ret;
	if(!DeviceIoControl(hTap,TAP_IOCTL_GET_MAC,NULL,0,(LPVOID)MacAddr,6,&ret,NULL))
	{
		DBGERR("Unable to get TAP MAC: {0}") % DBGLASTERROR();
		CloseHandle(hTap);
		return NULL;
	}
	wchar_t tap_name[64] = {0};
	get_tap_name(id, tap_name);
	wsprintf(g_name, L"\"%s\"", tap_name);
	//execve(L"netsh interface ip set address ", 2, wName, L" static 192.168.8.2");

	return (HTAP)hTap;
}

bool ChangeMediaStatus(HTAP hTap,bool status)
{
	ULONG mediaStatus = status ? TRUE:FALSE;
	DWORD ret;
	if(!DeviceIoControl((HANDLE)hTap,TAP_IOCTL_SET_MEDIA_STATUS,&mediaStatus,sizeof(ULONG),NULL,0,&ret,NULL))
	{
		fprintf(stderr,"Unable to change media status\n");
		return false;
	}
	if( g_name[0] ) {
		execve(L"netsh interface ip set address ", 2, g_name, L" static 192.168.8.2");
		g_name[0] = 0;
	}
	return true;
}

void CloseTap(HTAP hTap)
{
	ChangeMediaStatus(hTap,false);
	CloseHandle((HANDLE)hTap);
}


int ReadPacket(HTAP hTap,unsigned char buffer[1500])
{
	DWORD readed;
	if(!ReadFile((HANDLE)hTap,(LPVOID)buffer,1500,&readed,NULL)) 
	{
		DBGERR("ReadFile failed {0}") % DBGLASTERROR();
		return 0;
	}
	return (int)readed;
}

bool SendPacket(HTAP hTap,unsigned char *buffer,int size)
{
	DWORD written;
	if(!WriteFile((HANDLE)hTap,(LPVOID)buffer,size,&written,NULL))
	{
		DBGERR("WriteFile failed {0}") % DBGLASTERROR();
		return false;
	}
	return true;
}
