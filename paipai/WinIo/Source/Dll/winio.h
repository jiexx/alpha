#ifndef WINIO_H
#define WINIO_H

#include "..\drv\winio_nt.h"

#ifndef WINIO_DLL
#define WINIO_API _declspec(dllimport)
#else
#define WINIO_API 
#endif

extern "C"
{
  WINIO_API bool _stdcall InitializeWinIo();
  WINIO_API void _stdcall ShutdownWinIo();
  WINIO_API PBYTE _stdcall MapPhysToLin(tagPhysStruct &PhysStruct);
  WINIO_API bool _stdcall UnmapPhysicalMemory(tagPhysStruct &PhysStruct);
  WINIO_API bool _stdcall GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
  WINIO_API bool _stdcall SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal);
  WINIO_API bool _stdcall GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
  WINIO_API bool _stdcall SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);
  WINIO_API bool _stdcall InstallWinIoDriver(PWSTR pszWinIoDriverPath, bool IsDemandLoaded = false);
  WINIO_API bool _stdcall RemoveWinIoDriver();
}

extern HANDLE hDriver;
extern bool IsWinIoInitialized;
extern bool g_Is64BitOS;

bool _stdcall StartWinIoDriver();
bool _stdcall StopWinIoDriver();

#endif
//Microsoft enforces a ¡°Digital Signature Requirement¡± on all users of 64 bit version of Windows 7 and Vista. If you try to use any product driver that was not certified by Microsoft, you can still install it but Windows 7 - 64 bit won¡¯t let you run the driver.
//
//There are two ways to disable it:
//
//Method 1.
//
//Go to Start Menu and type "cmd.exe" in the Search Bar and press <Enter>. (Remember you must be logged on as Administrator)
//
//OR you can go to "All Programs", then "Accessories", right-click on "Command Prompt" and choose "Run as administrator".
//
//1. Now type the following and press <Enter> after each line:
//
//bcdedit.exe -set loadoptions DDISABLE_INTEGRITY_CHECKS
//
//bcdedit.exe -set TESTSIGNING ON
//
//2. Restart your computer
//
//3. This will disable your Windows 7 driver signing.
//
//If this did NOT work for you, here is another way to disable Windows 7 driver signing.
//
//Method 2.
//
//1. During boot-up, continuously press <F8> to get to Advanced Boot Options. Note: On some systems, you need to first press <F2> to go into Boot Menu, then press <F8> to reach Advanced Boot Options.
//
//2. When the menu appears, use the DOWN arrow key to scroll to ¡°Disable Driver Signature Enforcement.¡±
//
//3. Press <Enter>.
//
//4. Continue the boot process.
//
//5. After your computer has completed the boot process, please install the drivers per the included instructions.