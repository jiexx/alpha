
#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "map"

using namespace std;
using namespace cv;

#ifndef __LOADER_H__
#define __LOADER_H__

enum HANDLER_TYPE{
	RESIZE,
	ORIGIN,
	THINNING,
};

class loader {
public:
	loader();
	~loader();
	void addFont( const wchar_t* file, const wchar_t* font );
	void addChar( const wchar_t* str );

	inline int getCountOfFonts() const{
		return mFontRows.size();
	};
	inline int getCountOfChars() const{
		return mCharCols.size();
	}
	vector<Mat*>* getFontCharSet( int index );
	void saveBinary();
	void saveImage();
	void handle( HANDLER_TYPE ht = RESIZE );
	inline HANDLER_TYPE type() const {
		return mHt;
	}
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
			:font(f){ wmemset(name, 0, LF_FACESIZE ); wcscpy_s( name, desc );  }
	};
	typedef vector<wchar_t> Col;
	typedef vector<FontRows> Row;
	Row mFontRows;
	Col mCharCols;
	vector<vector<Mat*>*> mFontCharTable;//font - mat(0,1,2...)
	HBRUSH mClearBrush;
	HANDLER_TYPE mHt;

};

#endif