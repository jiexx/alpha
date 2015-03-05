// paipai.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "paipai.h"
#include "recognize.h"

#define MAX_LOADSTRING 100
#define WM_TRAY (WM_USER + 100) 
#define WM_TASKBAR_CREATED RegisterWindowMessage(TEXT("TaskbarCreated")) 
NOTIFYICONDATA nid;     //��������  
HMENU hMenu;            //���̲˵�  

recognize* g_reco;

#define ID_PAI_1   0x5001
#define ID_PAI_2   0x5002
#define ID_PAI_3   0x5003
#define ID_PAI   0x5000
#define ID_QUIT         0X6000 

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PAIPAI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAIPAI));

	g_reco = new recognize();
	if( g_reco != NULL ) {
		g_reco->load();
		g_reco->prepare();
	}
	//utils::open_dll();
	g_ver = GetOSVer();

	
	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if( g_reco != NULL ) {
		delete g_reco;
		g_reco = NULL;
	}
	//utils::close_dll();

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, szWindowClass, szTitle, WS_POPUP, CW_USEDEFAULT,  
	CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL); //CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	InitTray(hInst, hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	BOOL bRes  = RegisterHotKey(hWnd, ID_PAI, MOD_CONTROL, ' ');// ctrl+alt+0(С���̵�0) 
	bRes  = RegisterHotKey(hWnd, ID_QUIT, MOD_CONTROL, 'q'); //ctrl+alt+1(С���̵�1) 
	bRes  = RegisterHotKey(hWnd, ID_PAI_1,  0, VK_F1); 
	bRes  = RegisterHotKey(hWnd, ID_PAI_2,  0, VK_F2); 
	bRes  = RegisterHotKey(hWnd, ID_PAI_3,  0, VK_F3); 
	return TRUE;
}

LRESULT CALLBACK TrayWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(lParam)  
	{  
		case WM_RBUTTONDOWN:  
		{  
			//��ȡ�������  
			POINT pt; GetCursorPos(&pt);  
  
			//����ڲ˵��ⵥ������˵�����ʧ������  
			SetForegroundWindow(hWnd);  
  
			//ʹ�˵�ĳ����  
			//EnableMenuItem(hMenu, ID_SHOW, MF_GRAYED);      
  
			//��ʾ����ȡѡ�еĲ˵�  
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
	HWND h = FindWindow(L"TImageForm", L"���Ͷ������ͻ���");
	WINDOWINFO wi;
	GetWindowInfo(h, &wi);
	if ( h && wi.dwWindowStatus == WS_ACTIVECAPTION  ) {
		RECT r;
		GetWindowRect(h, &r);
		SwitchToThisWindow(h, TRUE);
		///////////////////////////////identify///////////////////////////////
		if( g_reco ) {
			const char* str;
			if( g_ver == 2 ) 
				str = g_reco->characterize( h, 284, 155, 111, 30);
			else
				str = g_reco->characterize( h, 283, 150, 113, 31);
			SwitchToThisWindow(h, TRUE);
			utils::keyClick( str );
			//__asm 
			//{
			//	OUT 0x64,0xD2
			//}
		}
		///////////////////////////////ok/////////////////////////////////////
		Sleep(1);
		utils::mouseClick(r.left+160, r.top+250);
		return 0;
	}
	h = FindWindow(L"TErrorBoxForm", L"���Ͷ������ͻ���");
	GetWindowInfo(h, &wi);
	if( h && wi.dwWindowStatus == WS_ACTIVECAPTION ) {
		RECT r;
		GetWindowRect(h, &r);
		SwitchToThisWindow(h, TRUE);
		utils::mouseClick(r.left+250, r.top+250);
		return 0;
	}
	h = FindWindow(L"TMainForm", NULL);
	GetWindowInfo(h, &wi);
	if( h && wi.dwWindowStatus == WS_ACTIVECAPTION ) {
		RECT r;
		GetWindowRect(h, &r);
		SwitchToThisWindow(h, TRUE);
		utils::mouseClick(r.left+580, r.top+440);
		return 0;
	}
	return 0;
}
void Pai123( int select ){
	int x, y;
	switch(select){
	case 1:
		x = 822;y = 388;
		break;
	case 2:
		x = 738;y = 388;
		break;
	case 3:
		x = 658;y = 388;
		break;
	}
	HWND h = FindWindow(L"TMainForm", NULL);
	WINDOWINFO wi;
	GetWindowInfo(h, &wi);
	if( h && wi.dwWindowStatus == WS_ACTIVECAPTION ) {
		RECT r;
		GetWindowRect(h, &r);
		SwitchToThisWindow(h, TRUE);
		utils::mouseClick(r.left+x, r.top+y);
		utils::mouseClick(r.left+810, r.top+438);
	}
}
//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		case ID_PAI_1:
			Pai123(1);
			break;
		case ID_PAI_2:
			Pai123(2);
			break;
		case ID_PAI_3:
			Pai123(3);
			break;
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
		// TODO: �ڴ���������ͼ����...
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



//ʵ��������  
void InitTray(HINSTANCE hInstance, HWND hWnd)  
{  
    nid.cbSize = sizeof(NOTIFYICONDATA);  
    nid.hWnd = hWnd;  
    nid.uID = 0;  
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;  
    nid.uCallbackMessage = WM_TRAY;  
    nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAIPAI));  
    lstrcpy(nid.szTip, szWindowClass);  
  
    hMenu = CreatePopupMenu();//�������̲˵�  
    //Ϊ���̲˵��������ѡ��  
    AppendMenu(hMenu, MF_STRING, ID_SHOW, TEXT("hint"));  
    AppendMenu(hMenu, MF_STRING, ID_EXIT, TEXT("exit"));  
  
    Shell_NotifyIcon(NIM_ADD, &nid);  
}  
  
//��ʾ������������  
void ShowTrayMsg()  
{  
    lstrcpy(nid.szInfoTitle, szWindowClass);  
    lstrcpy(nid.szInfo, TEXT("�յ�һ����Ϣ��"));  
    nid.uTimeout = 1000;  
    Shell_NotifyIcon(NIM_MODIFY, &nid);  
}  