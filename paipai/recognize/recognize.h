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
class recognize
{
public:
	recognize();
	~recognize();
	void load( const char imageslist[][8], int size );
	IplImage* preprocessing(IplImage* src);
	void prepare();
	int classify(IplImage* img);
	vector<Piece> identify(IplImage* input);
	vector<Piece> test(const char* file);
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
#define RESULT_SIZE 16
	char mResult[RESULT_SIZE];
};