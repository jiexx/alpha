// paipai.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "paipai.h"

#define MAX_LOADSTRING 100
#define WM_TRAY (WM_USER + 100) 
#define WM_TASKBAR_CREATED RegisterWindowMessage(TEXT("TaskbarCreated")) 
NOTIFYICONDATA nid;     //��������  
HMENU hMenu;            //���̲˵�  


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

	// ����Ϣѭ��:
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

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	BOOL bRes  = RegisterHotKey(hWnd, ID_PAI, MOD_CONTROL, ' ');// ctrl+alt+0(С���̵�0) 
	bRes  = RegisterHotKey(hWnd, ID_QUIT, MOD_CONTROL, '1'); //ctrl+alt+1(С���̵�1) 
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
	if ( h ) {
		RECT r;
		GetWindowRect(h, &r);
		SwitchToThisWindow(h, TRUE);
		utils::mouseClick(r.left+160, r.top+250);
		return 0;
	}
	h = FindWindow(L"TErrorBoxForm", L"���Ͷ������ͻ���");
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