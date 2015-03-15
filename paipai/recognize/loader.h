
#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "map"

using namespace std;
class loader {
public:
	loader( HWND hWnd, const wchar_t* str );
	~loader();
	BITMAP& get( wchar_t font, wchar_t c );
protected:
	Mat& getImage( HDC hdc, BITMAP bmp );
private:
	map<wchar_t, BITMAP&> mKB;

};