// recognize.h

#pragma once
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "windows.h"

using namespace cv;
using namespace std;
struct Piece{
	int x;
	char c;
	inline Piece( int a, char b ) : x(a), c(b){};
};

#define COUNT_DIGT 6

class recognize
{
public:
	recognize();
	~recognize();
	void load( const char imageslist[][8], int size, bool conv_save = false );
	IplImage* preprocessing(IplImage* src);
	void prepare();
	int classify(IplImage* img);
	const char* identify(IplImage* input);
	const char* test(const char* file);
	const char* characterize(HWND hWnd, const CvRect& rect);
protected:
	static bool sortx(const Piece &v1, const Piece &v2);
protected:
	CvKNearest* mKNN;
	CvMat* mTrainClasses;
	CvMat* mTrainData;
	int mNormSize;
	int mNumOfCls;
	int mNumOfSamples;
	IplImage** mArrImg;
	int K;
	char mResult[COUNT_DIGT+1];
};