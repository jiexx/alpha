#include "stdafx.h"
#include "loader.h"

#define FONT_HEIGHT 32
#define FONT_WIDTH  32
#define FONT_COUNT 2

const wchar_t g_font[FONT_COUNT][8] = {
	{L"宋体"},
	{L"华文彩云"},
};

loader::loader() {
	mClearBrush = CreateSolidBrush(RGB(0,0,0)); 
}

loader::~loader(){
	int i;
	for( i = 0 ; i < mFontCols.size() ; i ++ ) {
		HFONT hfont = mFontCols.at(i).font;
		DeleteObject(hfont);
	}
	mFontCols.clear();
	map<wchar_t, vector<Mat&>*>::iterator it;
	for( it = mCharMatTable.begin() ; it != mCharMatTable.end() ; it ++ ) {
		vector<Mat&>* val = it->second;
		if(val)
			delete val;
	}
	mCharMatTable.clear();
}

void loader::addFont( const wchar_t* file, const wchar_t* fontname ){

	if( AddFontResource( file ) )  	{  
		LOGFONT lf;  
		lf.lfHeight = 28;  
		lf.lfWidth = 0;  
		lf.lfEscapement = 0;  
		lf.lfOrientation = 0;  
		lf.lfWeight = FW_DONTCARE;   
		lf.lfItalic = 0;   
		lf.lfUnderline = 0;  
		lf.lfStrikeOut = 0;   
		lf.lfCharSet = DEFAULT_CHARSET;   
		lf.lfOutPrecision = 0;   
		lf.lfClipPrecision = CLIP_STROKE_PRECIS;  
		lf.lfQuality = 0;  
		lf.lfPitchAndFamily = 0;   
		wcscpy(lf.lfFaceName, fontname); // 这里就是字体名   
		HFONT hfont = CreateFontIndirect(&lf);  
		mFontCols.push_back( FontDesc(fontname, hfont) );
	}  
}

void loader::addChar( const wchar_t* str ){
	int len = wcslen(str);
	for( int i = 0 ; i < len ; i ++ ) {
		vector<Mat&>* p = new vector<Mat&>();
		if( p ) 
			mCharMatTable[str[i]] =  p;
	}
}

int loader::getCountOfChars() const{
	return mCharMatTable.size();
}

vector<Mat&>* loader::getCharsMatSet( int index ){
	map<wchar_t, vector<Mat&>*>::iterator it = mCharMatTable.end();
	for( it = mCharMatTable.begin() ; it != mCharMatTable.end() && index != 0; it ++ )
		index --;
	if( it == mCharMatTable.end() )
		return 0;
	return it->second;
}

void loader::clear( HDC hdc, HBITMAP hbmp ){
	BITMAP bmp;
	GetObject(hbmp, sizeof(bmp), &bmp);

	SelectObject(hdc, mClearBrush);
	Rectangle( hdc, 0, 0, bmp.bmWidth, bmp.bmHeight );
}

void loader::draw( HDC hdc, HFONT hfont, HBITMAP hbmp, const wchar_t c, int x, int y ){

	HFONT old = (HFONT)SelectObject(hdc, hfont);
	SetTextColor(hdc, RGB(255, 255, 255) );

	RECT pos = {x, y, 0, 0};
	DrawText(hdc, &c, 1, &pos, DT_CENTER);

	SelectObject(hdc, old);
}
Mat& loader::getMat( HDC hdc, HBITMAP hbmp, HANDLE hDib ){
	BITMAP bmp;
	GetObject(hbmp, sizeof(bmp), &bmp);

	BITMAPINFOHEADER bi = {0};  
	bi.biSize = sizeof(BITMAPINFOHEADER);  
	bi.biWidth = bmp.bmWidth;
	bi.biHeight = bmp.bmHeight; //revert graphic by vert 
	bi.biPlanes = 1;  
	bi.biBitCount = bmp.bmBitsPixel;  
	bi.biCompression = BI_RGB;  

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
		*lpbi = bi;  

		GetDIBits(hdc, hbmp, 0, bmp.bmHeight, (BYTE*)lpbi + sizeof(BITMAPINFOHEADER), (BITMAPINFO*)lpbi, DIB_RGB_COLORS);  

		Mat img = Mat::zeros(bmp.bmHeight, bmp.bmWidth, CV_32FC1);
		memcpy(img.data, (BYTE*)lpbi + sizeof(BITMAPINFOHEADER), img.total());
	GlobalUnlock(hDib);  
	return img;
}
void loader::splitMat( Mat& m ){
}

void loader::handle(){
	HDC dc0 = GetDC(0);
	HDC hdc = CreateCompatibleDC(dc0);

	BYTE *pbase;
	int len = getCountOfChars();
	BITMAPINFO bmpinfo;
	memset(&bmpinfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = FONT_WIDTH * len;
	bmpinfo.bmiHeader.biHeight = FONT_HEIGHT;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	HBITMAP hbmp = CreateDIBSection( hdc, &bmpinfo, DIB_RGB_COLORS,(void**)&pbase, 0, 0 );
	HBITMAP old = (HBITMAP)SelectObject( hdc, hbmp );

	DWORD dwSize = ((FONT_WIDTH * len * 24 + 31) / 32) * 4 * FONT_HEIGHT;  
	HANDLE hDib = GlobalAlloc(GHND, dwSize + sizeof(BITMAPINFOHEADER));  

	for( int i = 0 ; i < mFontCols.size() ; i ++ ) {
		HFONT hfont = mFontCols[i].font;
		clear( hdc, hbmp );
		map<wchar_t, vector<Mat&>*>::iterator it;
		int pos = 0;
		for( it = mCharMatTable.begin() ; it != mCharMatTable.end() ; it ++ ) {
			draw( hdc, hfont, hbmp, it->first, pos+2, 2 );
			pos += FONT_WIDTH;
		}
		splitMat( getMat( hdc, hbmp, hDib ) );
	}
	GlobalFree(hDib);  
	DeleteObject(hbmp);
	ReleaseDC(0,hdc);
}
