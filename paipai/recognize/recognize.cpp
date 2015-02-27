// 这是主 DLL 文件。

#include "stdafx.h"

#include "recognize.h"

recognize::recognize(){
	mNormSize = 32;
	mKNN = NULL;
	mNumOfCls = 10;
	mNumOfSamples = 1;
	mTrainData = cvCreateMat( mNumOfSamples*mNumOfCls, mNormSize*mNormSize, CV_32FC1 );// rows, col, type
	mTrainClasses = cvCreateMat( mNumOfSamples*mNumOfCls, 1, CV_32FC1 );
}
recognize::~recognize(){
	if( mArrImg )
		delete[] mArrImg;
	if( mKNN )
		delete mKNN;
}
void recognize::load( const char** imageslist, int size ){
	if( size != mNumOfCls*mNumOfSamples ) 
		return;
	mArrImg = (IplImage**)new IplImage[mNumOfCls*mNumOfSamples];
	for(int i = 0; i < mNumOfCls; i++){
		for(int j = 0; j< mNumOfSamples; j++){
			mArrImg[i*mNumOfSamples + j] = cvLoadImage(imageslist[i*mNumOfSamples + j],0);
		}
	}
}
IplImage* recognize::preprocessing(IplImage* src){
	return src;
}
void recognize::prepare(){
	CvMat cls, dat;
	IplImage* img;
	for(int i = 0; i < mNumOfCls; i++){
		for(int j = 0; j< mNumOfSamples; j++){
			// Set class label
			cvGetRow( mTrainClasses, &cls, i*mNumOfSamples + j );
			cvSet(&cls, cvRealScalar(i));

			img = preprocessing(mArrImg[i*mNumOfSamples + j]);

			// Set data
			cvGetRow( mTrainData, &cls, i*mNumOfSamples + j);
			cvGetSubRect( img, &dat, cvRect(0,0,mNormSize,mNormSize) );

			CvMat row_header, *row1;
			row1 = cvReshape( &dat, &row_header, 0, 1 );
			cvCopy(row1, &cls, NULL);

		}
	}
	mKNN = new CvKNearest( mTrainData, mTrainClasses, 0, false, K );
	if( mKNN ) {
		mKNN->save();
	}
}
int recognize::classify(IplImage* img){
	CvMat cls, dat;
	IplImage* p;
	CvMat* nearest=cvCreateMat(1,K,CV_32FC1);

	p = preprocessing(img);

	cvGetSubRect( p, &dat, cvRect(0,0,mNormSize,mNormSize) );
	CvMat row_header, *row1;
	row1 = cvReshape( &dat, &row_header, 0, 1 );

	float result = mKNN->find_nearest(row1,K,0,0,nearest,0);
	return (int) result;
}