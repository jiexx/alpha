#include "stdafx.h"
#include "loader.h"
#include "rectangle.h"
#include "file.h"


loader::loader() {
	mClearBrush = CreateSolidBrush(RGB(0,0,0)); 
}

loader::~loader(){
	unsigned int i;
	for( i = 0 ; i < mFontRows.size() ; i ++ ) {
		HFONT hfont = mFontRows.at(i).font;
		DeleteObject(hfont);
	}
	mFontRows.clear();
	map<wstring, vector<Mat*>*>::iterator it;
	for( it = mFontCharTable.begin() ; it != mFontCharTable.end() ; it ++ ) {
		vector<Mat*>* val = it->second;
		if(val) {
			for( unsigned int i = 0 ; i < val->size() ; i ++ ) {
				Mat* m = (*val)[i];
				if( m ) 
					delete m;
			}
			delete val;
		}
	}
	mFontCharTable.clear();
}

void loader::addFont( const wchar_t* file, const wchar_t* fontname ){

	if( AddFontResource( file ) )  	{  
		LOGFONT lf;  
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfHeight = H;  
		lf.lfWidth = 0;  
		lf.lfEscapement = 0;  
		lf.lfOrientation = 0;  
		lf.lfWeight = FW_NORMAL;   
		lf.lfItalic = 0;   
		lf.lfUnderline = 0;  
		lf.lfStrikeOut = 0;   
		lf.lfCharSet = GB2312_CHARSET;   
		lf.lfOutPrecision = OUT_STROKE_PRECIS;   
		lf.lfClipPrecision = CLIP_STROKE_PRECIS;  
		lf.lfQuality = ANTIALIASED_QUALITY;  
		lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;   
		wcscpy_s(lf.lfFaceName, fontname); // 这里就是字体名   
		HFONT hfont = CreateFontIndirect(&lf);  
		mFontRows.push_back( FontRows(fontname, hfont) );
	}  
}

void loader::addChar( const wchar_t* str ){
	int len = wcslen(str);
	for( int i = 0 ; i < len ; i ++ ) {
		mCharCols.push_back( str[i] );
	}
}

vector<Mat*>* loader::getFontCharSet( int index ){
	/*map<wstring, vector<Mat*>*>::iterator it = mFontCharTable.end();
	for( it = mFontCharTable.begin() ; it != mFontCharTable.end() && index != 0; it ++ )
		index --;
	if( it == mFontCharTable.end() )
		return 0;
	return it->second;*/
	return mFontCharTable[index];
}

void loader::clear( HDC hdc, HBITMAP hbmp ){
	BITMAP bmp;
	GetObject(hbmp, sizeof(bmp), &bmp);

	SelectObject(hdc, mClearBrush);
	Rectangle( hdc, 0, 0, bmp.bmWidth, bmp.bmHeight );
	SetBkColor(hdc, RGB(0, 0, 0));
}

void loader::draw( HDC hdc, HFONT hfont, HBITMAP hbmp, const wchar_t c, int x, int y ){

	HFONT old = (HFONT)SelectObject(hdc, hfont);
	SetTextColor(hdc, RGB(255, 255, 255) );

	RECT pos = {x, y, W, H};
	DrawText(hdc, &c, 1, &pos, DT_CENTER);

	//SelectObject(hdc, old);
}
Mat* loader::getMat( HDC hdc, HBITMAP hbmp, HANDLE hDib ){
	BITMAP bmp;
	GetObject(hbmp, sizeof(bmp), &bmp);

	BITMAPINFOHEADER bi = {0};  
	bi.biSize = sizeof(BITMAPINFOHEADER);  
	bi.biWidth = bmp.bmWidth;
	bi.biHeight = -bmp.bmHeight; //revert graphic by vert 
	bi.biPlanes = 1;  
	bi.biBitCount = bmp.bmBitsPixel;  
	bi.biCompression = BI_RGB;  

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
		*lpbi = bi;  

		GetDIBits(hdc, hbmp, 0, bmp.bmHeight, (BYTE*)lpbi + sizeof(BITMAPINFOHEADER), (BITMAPINFO*)lpbi, DIB_RGB_COLORS);  

		Mat* img = new Mat(bmp.bmHeight, bmp.bmWidth, CV_8UC3);
		if( img ) 
			memcpy(img->data, (BYTE*)lpbi + sizeof(BITMAPINFOHEADER), img->total()*img->elemSize());
	GlobalUnlock(hDib); 
	
	return img;
}
void loader::splitMat( const wchar_t* fontname, Mat* m ){
	rects r;
	if( m ) {
		vector<Mat*>* mats = r.getThinMats(*m); // to CV_8UC1
		imwrite("getNormalMats.png", *m);
		m->release();
		delete m;

		if( mats && mats->size() == mCharCols.size() ) {
			mFontCharTable.push_back(mats);  //   CV_8UC3 to CV_8UC1
		}
	}
}

void loader::handle(){
	HDC dc0 = GetDC(0);
	HDC hdc = CreateCompatibleDC(dc0);

	BYTE *pbase;
	int len = mCharCols.size();
	BITMAPINFO bmpinfo;
	memset(&bmpinfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = W * len;
	bmpinfo.bmiHeader.biHeight = H;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	HBITMAP hbmp = CreateDIBSection( hdc, &bmpinfo, DIB_RGB_COLORS,(void**)&pbase, 0, 0 );
	HBITMAP old = (HBITMAP)SelectObject( hdc, hbmp );

	DWORD dwSize = ((W * len * 24 + 31) / 32) * 4 * H;  
	HANDLE hDib = GlobalAlloc(GHND, dwSize + sizeof(BITMAPINFOHEADER));  

	for( unsigned int i = 0 ; i < mFontRows.size() ; i ++ ) {
		HFONT hfont = mFontRows[i].font;
		clear( hdc, hbmp );
		int pos = 0;
		for( unsigned int j = 0 ; j < mCharCols.size() ; j ++ ) {
			draw( hdc, hfont, hbmp, mCharCols[j], pos, 0 );
			pos += W + 20;
		}
		splitMat( mFontRows[i].name, getMat( hdc, hbmp, hDib ) );
	}
	GlobalFree(hDib);  
	DeleteObject(hbmp);
	ReleaseDC(0,hdc);
}

void loader::saveBinary() {
	vector<vector<Mat*>*>::iterator it;
	unsigned int i = 0, j = 0;
	for( it = mFontCharTable.begin() ; it != mFontCharTable.end() ; it ++ ) {
		vector<Mat*>* img = *it;
		if( img ) {
			for( j = 0 ; j < img->size(); j ++ ) {
				stringstream si, sj;
				si << i;
				sj << j;
				binary bf( (si.str()+sj.str()+string(".bin")).c_str() );
				vector<unsigned char> buf;
				buf = Mat_<Point3i>(*((*img)[j]));
				bf.save( buf );
			}
		}
	}
}
void loader::saveImage() {
	vector<vector<Mat*>*>::iterator it;
	unsigned int i = 0, j = 0;
	for( it = mFontCharTable.begin() ; it != mFontCharTable.end() ; it ++, i ++ ) {
		vector<Mat*>* img = *it;
		if( img ) {
			for( j = 0 ; j < img->size(); j ++ ) {
				stringstream si, sj;
				si << i;
				sj << j;
				imwrite( (si.str()+sj.str()+string(".png")).c_str(), *((*img)[j]) );
			}
		}
	}
}
