// paipai.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "paipai.h"

#define MAX_LOADSTRING 100
#define WM_TRAY (WM_USER + 100) 
#define WM_TASKBAR_CREATED RegisterWindowMessage(TEXT("TaskbarCreated")) 
NOTIFYICONDATA nid;     //托盘属性  
HMENU hMenu;            //托盘菜单  


#define ID_PAI   0x5000
#define ID_QUIT         0X6000 

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PAIPAI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAIPAI));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= NULL;//CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAIPAI));
	wcex.hCursor		= NULL;//LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_PAIPAI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, szWindowClass, szTitle, WS_POPUP, CW_USEDEFAULT,  
	CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL); //CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	BOOL bRes  = RegisterHotKey(hWnd, ID_PAI, MOD_CONTROL, ' ');// ctrl+alt+0(小键盘的0) 
	bRes  = RegisterHotKey(hWnd, ID_QUIT, MOD_CONTROL, '1'); //ctrl+alt+1(小键盘的1) 
	return TRUE;
}

LRESULT CALLBACK TrayWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(lParam)  
	{  
		case WM_RBUTTONDOWN:  
		{  
			//获取鼠标坐标  
			POINT pt; GetCursorPos(&pt);  
  
			//解决在菜单外单击左键菜单不消失的问题  
			SetForegroundWindow(hWnd);  
  
			//使菜单某项变灰  
			//EnableMenuItem(hMenu, ID_SHOW, MF_GRAYED);      
  
			//显示并获取选中的菜单  
			int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL);  
			if(cmd == ID_EXIT)   
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);  
		}  
		break; 
	}
	return 0;
}

LRESULT CALLBACK QuitWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0); 
	return 0;
}

LRESULT CALLBACK PaiWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND h = FindWindow(L"TImageForm", L"额度投标网络客户端");
	if ( h ) {
		RECT r;
		GetWindowRect(h, &r);
		SwitchToThisWindow(h, TRUE);
		utils::mouseClick(r.left+160, r.top+250);
		return 0;
	}
	h = FindWindow(L"TErrorBoxForm", L"额度投标网络客户端");
	if( h ) {
		RECT r;
		GetWindowRect(h, &r);
		SwitchToThisWindow(h, TRUE);
		utils::mouseClick(r.left+250, r.top+250);
		return 0;
	}
	h = FindWindow(L"TMainForm", NULL);
	if( h ) {
		RECT r;
		GetWindowRect(h, &r);
		SwitchToThisWindow(h, TRUE);
		utils::mouseClick(r.left+580, r.top+440);
		return 0;
	}
	return 0;
}
//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_HOTKEY:
		switch(wParam) {
		case ID_PAI:
			PaiWndProc(hWnd, message, wParam, lParam);
			break;
		case ID_QUIT:
			QuitWndProc(hWnd, message, wParam, lParam);
			break;
		}
		break;
	case WM_TRAY:
		TrayWndProc(hWnd, message, wParam, lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		Shell_NotifyIcon(NIM_DELETE, &nid);
		PostQuitMessage(0);
		break;		
	}
	if (message == WM_TASKBAR_CREATED){
		Shell_NotifyIcon(NIM_ADD, &nid);  
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}



//实例化托盘  
void InitTray(HINSTANCE hInstance, HWND hWnd)  
{  
    nid.cbSize = sizeof(NOTIFYICONDATA);  
    nid.hWnd = hWnd;  
    nid.uID = 0;  
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;  
    nid.uCallbackMessage = WM_TRAY;  
    nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAIPAI));  
    lstrcpy(nid.szTip, szWindowClass);  
  
    hMenu = CreatePopupMenu();//生成托盘菜单  
    //为托盘菜单添加两个选项  
    AppendMenu(hMenu, MF_STRING, ID_SHOW, TEXT("hint"));  
    AppendMenu(hMenu, MF_STRING, ID_EXIT, TEXT("exit"));  
  
    Shell_NotifyIcon(NIM_ADD, &nid);  
}  
  
//演示托盘气泡提醒  
void ShowTrayMsg()  
{  
    lstrcpy(nid.szInfoTitle, szWindowClass);  
    lstrcpy(nid.szInfo, TEXT("收到一条消息！"));  
    nid.uTimeout = 1000;  
    Shell_NotifyIcon(NIM_MODIFY, &nid);  
}  