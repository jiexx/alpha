
#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "map"

using namespace std;
using namespace cv;

#ifndef __LOADER_H__
#define __LOADER_H__


class loader {
public:
	loader();
	~loader();
	void addFont( const wchar_t* file, const wchar_t* font );
	void addChar( const wchar_t* str );

	int getCountOfFonts() const;
	vector<Mat*>* getFontCharSet( int index );
	void saveBinary();
	void saveImage();
	void handle();
protected:
	void clear( HDC hdc, HBITMAP hbmp );
	void draw( HDC hdc, HFONT hfont, HBITMAP hbmp, const wchar_t c, int x, int y );
	Mat* getMat( HDC hdc, HBITMAP bmp, HANDLE hDib );
	void splitMat( const wchar_t* fontname, Mat* m );
private:
	struct FontRows {
	public:
		wchar_t name[LF_FACESIZE];
		HFONT font;
		FontRows( const wchar_t* desc, HFONT f )
			:font(f){ wmemset(name, 0, LF_FACESIZE ); wcscpy( name, desc );  }
	};
	typedef vector<wchar_t> Col;
	typedef vector<FontRows> Row;
	Row mFontRows;
	Col mCharCols;
	map<wstring, vector<Mat*>*> mFontCharTable;//font - mat(0,1,2...)
	HBRUSH mClearBrush;

};

#endif