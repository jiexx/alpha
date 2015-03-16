
#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "map"

using namespace std;
using namespace cv;

void ThinSubiteration1(Mat & pSrc, Mat & pDst) {
	int rows = pSrc.rows;
	int cols = pSrc.cols;
	pSrc.copyTo(pDst);
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			if(pSrc.at<float>(i, j) == 1.0f) {
				/// get 8 neighbors
				/// calculate C(p)
				int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
				int neighbor1 = (int) pSrc.at<float>( i-1, j);
				int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
				int neighbor3 = (int) pSrc.at<float>( i, j+1);
				int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
				int neighbor5 = (int) pSrc.at<float>( i+1, j);
				int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
				int neighbor7 = (int) pSrc.at<float>( i, j-1);
				int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
					int(~neighbor3 & ( neighbor4 | neighbor5)) +
					int(~neighbor5 & ( neighbor6 | neighbor7)) +
					int(~neighbor7 & ( neighbor0 | neighbor1));
				if(C == 1) {
					/// calculate N
					int N1 = int(neighbor0 | neighbor1) +
						int(neighbor2 | neighbor3) +
						int(neighbor4 | neighbor5) +
						int(neighbor6 | neighbor7);
					int N2 = int(neighbor1 | neighbor2) +
						int(neighbor3 | neighbor4) +
						int(neighbor5 | neighbor6) +
						int(neighbor7 | neighbor0);
					int N = min(N1,N2);
					if ((N == 2) || (N == 3)) {
						/// calculate criteria 3
						int c3 = ( neighbor1 | neighbor2 | ~neighbor4) & neighbor3;
						if(c3 == 0) {
							pDst.at<float>( i, j) = 0.0f;
						}
					}
				}
			}
		}
	}
}


void ThinSubiteration2(Mat & pSrc, Mat & pDst) {
	int rows = pSrc.rows;
	int cols = pSrc.cols;
	pSrc.copyTo( pDst);
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			if (pSrc.at<float>(i, j) == 1.0f) {
				/// get 8 neighbors
				/// calculate C(p)
				int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
				int neighbor1 = (int) pSrc.at<float>( i-1, j);
				int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
				int neighbor3 = (int) pSrc.at<float>( i, j+1);
				int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
				int neighbor5 = (int) pSrc.at<float>( i+1, j);
				int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
				int neighbor7 = (int) pSrc.at<float>( i, j-1);
				int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
					int(~neighbor3 & ( neighbor4 | neighbor5)) +
					int(~neighbor5 & ( neighbor6 | neighbor7)) +
					int(~neighbor7 & ( neighbor0 | neighbor1));
				if(C == 1) {
					/// calculate N
					int N1 = int(neighbor0 | neighbor1) +
						int(neighbor2 | neighbor3) +
						int(neighbor4 | neighbor5) +
						int(neighbor6 | neighbor7);
					int N2 = int(neighbor1 | neighbor2) +
						int(neighbor3 | neighbor4) +
						int(neighbor5 | neighbor6) +
						int(neighbor7 | neighbor0);
					int N = min(N1,N2);
					if((N == 2) || (N == 3)) {
						int E = (neighbor5 | neighbor6 | ~neighbor0) & neighbor7;
						if(E == 0) {
							pDst.at<float>(i, j) = 0.0f;
						}
					}
				}
			}
		}
	}
}

Mat& MorphologicalThinning(Mat& src) { 
	bool bDone = false; 
	int rows = src.rows; 
	int cols = src.cols; 
	/// pad source 
	Mat enlarged_src = Mat::zeros(rows + 2, cols + 2, CV_32FC1); 
 
	float *pSrc, *pDst; 
	for(int i = 0; i < rows; i++) { 
		pSrc = src.ptr<float>(i);  
		pDst = enlarged_src.ptr<float>(i);
		for(int j = 0; j < cols; j++) { 
			if ( pSrc[j] >= 0.5f ) { 
				pDst[j+1] = 1.0f; 
			} else {
				pDst[j+1] = 0.0f; 
			}
		} 
	} 
	/// start to thin 
	Mat thinMat1 = Mat::zeros(rows + 2, cols + 2, CV_32FC1); 
	Mat thinMat2 = Mat::zeros(rows + 2, cols + 2, CV_32FC1); 
	Mat cmp      = Mat::zeros(rows + 2, cols + 2, CV_8UC1); 
	while (bDone != true) { 
		/// sub-iteration 1 
		ThinSubiteration1(enlarged_src, thinMat1); 
		/// sub-iteration 2 
		ThinSubiteration2(thinMat1, thinMat2); 
		/// compare 
		compare(enlarged_src, thinMat2, cmp, CMP_EQ); 
		/// check 
		int num_non_zero = countNonZero(cmp); 
		if(num_non_zero == (rows + 2) * (cols + 2)) { 
			bDone = true; 
		} 
		/// copy 
		thinMat2.copyTo(enlarged_src); 
	}
	/// copy result 
	Mat result = Mat::zeros(rows, cols, CV_32FC1); 
	for(int i = 0; i < rows; i++) { 
		pSrc = enlarged_src.ptr<float>(i+1);  
		pDst = result.ptr<float>(i);
		for(int j = 0; j < cols; j++) { 
			pDst[j] = pSrc[j+1]; 
		} 
	} 
	/// clean memory 
	enlarged_src.release();
	thinMat1.release();
	thinMat2.release();
	cmp.release();
	return result;
} 
class loader {
public:
	loader();
	~loader();
	void addFont( const wchar_t* file, const wchar_t* font );
	void addChar( const wchar_t* str );

	int getCountOfChars() const;
	vector<Mat&>* getCharsMatSet( int index );
protected:
	void clear( HDC hdc, HBITMAP hbmp );
	void draw( HDC hdc, HFONT hfont, HBITMAP hbmp, const wchar_t c, int x, int y );
	Mat& getMat( HDC hdc, HBITMAP bmp, HANDLE hDib );
	void splitMat( Mat& m );
	void handle();
private:
	typedef struct FontDesc {
	public:
		wchar_t name[LF_FACESIZE];
		HFONT font;
		FontDesc( const wchar_t* desc, HFONT f )
			:font(f){ wmemset(name, 0, LF_FACESIZE ); wcscpy( name, desc );  }
	}FontCols;
	typedef vector<FontCols> FontCol;
	FontCol mFontCols;
	map<wchar_t, vector<Mat&>*> mCharMatTable;
	HBRUSH mClearBrush;

};