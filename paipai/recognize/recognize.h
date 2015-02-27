// recognize.h

#pragma once
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"

using namespace cv;

class recognize
{
public:
	recognize();
	~recognize();
	void load( const char** imageslist, int size );
	IplImage* preprocessing(IplImage* src);
	void prepare();
	int classify(IplImage* img);
protected:
	CvKNearest* mKNN;
	CvMat* mTrainClasses;
	CvMat* mTrainData;
	int mNormSize;
	int mNumOfCls;
	int mNumOfSamples;
	IplImage** mArrImg;
	const int K=10;
};