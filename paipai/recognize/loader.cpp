#include "stdafx.h"
#include "loader.h"

#define BMP_HEIGHT 32
#define BMP_WIDTH  32
#define FONT_COUNT 2

const wchar_t g_font[FONT_COUNT][8] = {
	{L"ËÎÌå"},
	{L"»ªÎÄ²ÊÔÆ"},
};

loader::loader( HWND hWnd, const wchar_t* str ){
	int len = wcslen(str);
	BITMAPINFO bmpinfo;
	memset(&bmpinfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = BMP_WIDTH * len;
	bmpinfo.bmiHeader.biHeight = BMP_HEIGHT;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;

	HDC dc = GetDC(0);
	HDC hdc = CreateCompatibleDC(dc);

	BYTE *pbase;
	HBITMAP bmp = CreateDIBSection( dc, &bmpinfo, DIB_RGB_COLORS,(void**)&pbase, 0, 0 );
	HBITMAP hOldBMP = (HBITMAP)SelectObject( hdc, bmp );
	
	for( int i = 0; i < FONT_COUNT; i ++ ) {
		HFONT font = CreateFont(32, 0, 0, 0, 
			FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, g_font[i]);

		SelectObject(hdc, font);
		RECT pos = {2,2,0,0};
		SetTextColor(hdc, RGB(255,0,0));
		DrawText(hdc, str, -1, &pos, DT_NOCLIP);

		BITMAPINFOHEADER bi = {0};  
		bi.biSize = sizeof(BITMAPINFOHEADER);  
		bi.biWidth = BMP_WIDTH * len;
		bi.biHeight = -BMP_HEIGHT; //revert graphic by vert 
		bi.biPlanes = 1;  
		bi.biBitCount = 24;  
		bi.biCompression = BI_RGB;  

		DWORD dwSize = ((BMP_WIDTH * len * 24 + 31) / 32) * 4 * BMP_HEIGHT;  

		HANDLE hDib = GlobalAlloc(GHND, dwSize + sizeof(BITMAPINFOHEADER));  
		LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
		*lpbi = bi;  

		GetDIBits(hdc, bmp, 0, BMP_HEIGHT, (BYTE*)lpbi + sizeof(BITMAPINFOHEADER), (BITMAPINFO*)lpbi, DIB_RGB_COLORS);  

		int channal=(bi.biBitCount == 1) ? 1:(bi.biBitCount/8);
		//int depth  =(pbmpinfo->bmiHeader.biBitCount == 1) ? IPL_DEPTH_1U : IPL_DEPTH_8U;
		int depth  =  IPL_DEPTH_8U;

		int width=bi.biWidth;
		int heigh=-bi.biHeight;

		IplImage *pmg;
		Mat m;
		m.imageData = 

		pmg=cvCreateImageHeader(cvSize(width,heigh),depth,channal);
		pmg->imageData=(char*)malloc(pmg->imageSize);
		pmg->origin=1;
		//cvSetData();
		memcpy(pmg->imageData, (BYTE*)lpbi + sizeof(BITMAPINFOHEADER), pmg->imageSize);

		GlobalUnlock(hDib);  
		GlobalFree(hDib);  
	}
}
loader::~loader(){
}
BITMAP& loader::get( wchar_t font, wchar_t c ){
}