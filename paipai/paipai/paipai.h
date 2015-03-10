#pragma once

#include "resource.h"
#include "vector"
#include "string"
#include "winio.h"
#include "time.h"

using namespace std;


extern bool _stdcall InitializeWinIo();
extern void _stdcall ShutdownWinIo();
extern bool _stdcall GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
extern bool _stdcall SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);

const static short scan_code[10][2] = {
		{0x0b, 0x8b}, //0
		{0x02, 0x82}, //1
		{0x03, 0x83}, //2
		{0x04, 0x84}, //3
		{0x05, 0x85}, //4
		{0x06, 0x86}, //5
		{0x07, 0x87}, //6
		{0x08, 0x88}, //7
		{0x09, 0x89}, //8
		{0x0a, 0x8a}, //9
};

int GetOSVer()  
{  
	OSVERSIONINFO   osver;     
	osver.dwOSVersionInfoSize   =   sizeof(OSVERSIONINFO);     
	GetVersionEx(&osver);     
	if(osver.dwPlatformId == 2)  
	{  
		if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)  
		{  
			//printf("xp\n");  
			return(2);  
		}  
		if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)  
		{  
			//printf("windows 2003\n");  
			return(3);  
		}  
		if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 0)  
		{  
			//printf("vista and 2008\n");  
			return(4);  
		}  
		if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 1)  
		{  
			//printf("2008 R2 and Windows 7\n");  
			return(5);  
		}  
	}  
	return 0;  
} 

static int g_ver = 0;

static void* g_dll = 0;

class utils {
public:
	inline static void mouseClick( int x, int y ) {
		POINT screen, old, now;
		screen.x = 65535.0f / ( GetSystemMetrics( SM_CXSCREEN )-1 ); 
		screen.y =  65535.0f / ( GetSystemMetrics( SM_CYSCREEN )-1 );
		now.x = x * screen.x;
		now.y = y * screen.y;
		GetCursorPos(&old);
		old.x = old.x * screen.x;
		old.y = old.y * screen.y;
		ShowCursor(FALSE);
		INPUT i[4];
		SecureZeroMemory(i, sizeof(i));
		i[0].type = INPUT_MOUSE;
		i[0].mi.dx = now.x;
		i[0].mi.dy = now.y;
		i[0].mi.mouseData = 0;
		i[0].mi.time = 0;
		i[0].mi.dwExtraInfo = 0;
		i[0].mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
		i[1].type = INPUT_MOUSE;
		i[1].mi.dx = now.x;
		i[1].mi.dy = now.y;
		i[1].mi.mouseData = 0;
		i[1].mi.time = 0;
		i[1].mi.dwExtraInfo = 0;
		i[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		i[2].type = INPUT_MOUSE;
		i[2].mi.dx = now.x;
		i[2].mi.dy = now.y;
		i[2].mi.mouseData = 0;
		i[2].mi.time = 0;
		i[2].mi.dwExtraInfo = 0;
		i[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
		i[3].type = INPUT_MOUSE;
		i[3].mi.dx = old.x;
		i[3].mi.dy = old.y;
		i[3].mi.mouseData = 0;
		i[3].mi.time = 0;
		i[3].mi.dwExtraInfo = 0;
		i[3].mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
		SendInput(4, i, sizeof(INPUT));
		ShowCursor(TRUE);
	};

	inline static void keyClick( const char* keys ) {
		//Sleep(100);
		int len = strlen(keys);
		vector<INPUT> in;
		for( int k = 0; k < len; k ++ ) {
			int c = keys[k]-'0';
			INPUT i[2];
			SecureZeroMemory(i, sizeof(i));
			i[0].type = INPUT_KEYBOARD;
			i[0].ki.dwFlags = KEYEVENTF_SCANCODE ;
			i[0].ki.wScan = scan_code[c][0];
			i[1].type = INPUT_KEYBOARD;
			i[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			i[1].ki.wScan = scan_code[c][1];
			in.push_back(i[0]);
			in.push_back(i[1]);
		}

		SendInput(in.size(), in.data(), sizeof(INPUT));
		//Sleep(1);
	};
	inline static time_t fmt_time_t(char * szTime)  
	{  
		struct tm tm1, *curr1;
		time_t time1, curr2;  
		time( &curr2 ); 
		curr1 = localtime( &curr2 );
		memcpy( &tm1, curr1, sizeof(tm1));
		sscanf(szTime, "%2d:%2d:%2d",      
			&tm1.tm_hour,   
			&tm1.tm_min,  
			&tm1.tm_sec);  
		time1 = mktime(&tm1);  
		return time1;  
	};
	inline static time_t curr_time_t()  
	{  
		time_t curr2;  
		time( &curr2 ); 
		return curr2;  
	};
	inline static time_t totime_t(char * szTime)  
	{  
		struct tm tm1;  
		time_t time1;  
		sscanf(szTime, "%4d:%2d:%2d:%2d:%2d:%2d",      
			&tm1.tm_year,   
			&tm1.tm_mon,   
			&tm1.tm_mday,   
			&tm1.tm_hour,   
			&tm1.tm_min,  
			&tm1.tm_sec);  
		tm1.tm_year -= 1900;  
		tm1.tm_mon --; 
		tm1.tm_isdst=-1;  
		time1 = mktime(&tm1);  
		return time1;  
	};

	class dll {
	public :
		typedef bool (_stdcall *InitializeWinIo_FUNC)();
		typedef void (_stdcall *ShutdownWinIo_FUNC)();
		typedef bool (_stdcall *GetPortVal_FUNC)(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
		typedef bool (_stdcall *SetPortVal_FUNC)(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);
		dll(): mInitializeWinIo(0),mShutdownWinIo(0),mGetPortVal(0),mSetPortVal(0),mCode(0) {
			/*char var1[10] = {"0"}, var2[10] = {"0"};
			GetEnvironmentVariableA("PROCESSOR_ARCHITECTURE", (LPSTR)var1, 10);
			GetEnvironmentVariableA("PROCESSOR_ARCHITEW6432", (LPSTR)var2, 10);
			string a(var1), b(var2), dll, amd64("amd64");
			if( a == amd64 || b == amd64 ) {
				dll = ".//lib//WinIo64.dll";
			}else {
				dll = ".//lib//WinIo32.dll";
			}
			mDll = LoadLibraryA((LPSTR)dll.c_str());*/
			{
				mInitializeWinIo = InitializeWinIo;//GetProcAddress(mDll, "InitializeWinIo");
				mShutdownWinIo = ShutdownWinIo;//GetProcAddress(mDll, "ShutdownWinIo");
				mGetPortVal = GetPortVal;//GetProcAddress(mDll, "GetPortVal");
				mSetPortVal = SetPortVal;//GetProcAddress(mDll, "SetPortVal");
			}
		};
		void Initialize() {
			if( mInitializeWinIo ) {
				mCode = mInitializeWinIo();
			}
		};
		void iShutdown() {
			if( mShutdownWinIo ) {
				mShutdownWinIo();
			}
		};
		void GetPort(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize) {
			if( mGetPortVal ) {
				mCode = mGetPortVal(wPortAddr, pdwPortVal, bSize);
			}
		};
		void SetPort(WORD wPortAddr, DWORD dwPortVal, BYTE bSize) {
			if( mSetPortVal ) {
				mCode = mSetPortVal(wPortAddr, dwPortVal, bSize);
			}
		};
		void KbcWait4IBE()
		{
			DWORD dwRegVal=0;
			do
			{	
				GetPort(0x64,&dwRegVal,1);
			}
			while(dwRegVal & 0x2);
		}
		void KeyPress(DWORD KCode) {
			int code = MapVirtualKey(KCode,0);

			KbcWait4IBE();//Wait for KBC input buffer empty
			SetPort(0x64,0xD2,1);//Send data back to the system command

			KbcWait4IBE();//Wait for KBC input buffer empty
			SetPort(0x60,code,1);//Send the key down scancode

			Sleep(10);

			KbcWait4IBE();//Wait for KBC input buffer empty
			SetPort(0x64,0xD2,1);//Send data back to the system command

			KbcWait4IBE();//Wait for KBC input buffer empty
			SetPort(0x60,(code | 0x80),1);//Send the key up scancode
		};
		void keyClick( const char* keys ) {
			int len = strlen(keys);
			Sleep(100);
			for( int k = 0; k < len; k ++ ) {
				KeyPress(keys[k]);
			}
		}
		~dll() {
			if( mDll ) {
				FreeLibrary( mDll );
			}
		};
	protected:
		InitializeWinIo_FUNC mInitializeWinIo;
		ShutdownWinIo_FUNC mShutdownWinIo;
		GetPortVal_FUNC mGetPortVal;
		SetPortVal_FUNC mSetPortVal;
		HINSTANCE mDll;
		int mCode;
	};

	inline static void open_dll() {
		close_dll();
		g_dll = new dll();
		if( g_dll )
			((dll*)g_dll)->Initialize();
	};

	inline static void close_dll() {
		if( g_dll )
			delete ((dll*)g_dll);
	};

	inline static void keyClicks( const char* keys ) {
		if( g_dll ) {
			((dll*)g_dll)->keyClick( keys );
		}
	};

};
