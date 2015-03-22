#include "stdafx.h"
#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "map"
#include "vector"
#include "fstream"

using namespace std;
using namespace cv;

#ifndef __PORTRAIT_H__
#define __PORTRAIT_H__

enum BIN {
	BIN_R = 5,    //x cols
	BIN_THETA = 12,//y rows
};
class bin {
public:
	inline bin(){
		memset(data, 0, sizeof(data));
	};
	inline bin( float v ){
		float* p = (float*)data;
		for(int i = 0 ; i < sizeof(data) ; i ++ ) 
			*p++ = v;
	};
	inline bin(const bin& b){
		if( data )
			memcpy(data, b.data, sizeof(data));
	};
	inline ~bin(){
		memset(data, 0, sizeof(data));
	};
	inline void clear() {
		memset(data, 0, sizeof(data));
	};
	inline float& operator()(int i, int j) {
		return data[i][j];
	};
	inline float* ptr() {
		return (float*)data;
	};
	inline int size() {
		return sizeof(data);
	};
protected:
	float data[BIN_THETA][BIN_R];
};

class portrait {
public:
	inline portrait( int r, bool clr = true ) {
		mWidht = BIN_R;
		mHeight = BIN_THETA;
		mRadius = r;
		mPolar = Mat::zeros(mHeight, mWidht, CV_8UC3);
		mAvailableColor = clr;
		mColor = Vec3b(255,255,255);
		//mStep.make(2*radius, 2*radius);
	};

	inline bool fe( Vec3b& c1, Vec3b& c2 ) const {
		return ( c1[0] - c2[0] == 0 && c1[1] - c2[1] == 0 && c1[2] - c2[2] == 0 );
	};

	inline bin& getCenterPointPortrait( const Mat& square, Vec3b center, int x, int y ) { //CV_32FC1

		mPortraits.clear();
		mPolar = 0;
		//mStep.reset();

		//Mat debug = imread("cvLogPolar0.png");
		CvMat s = square, d = mPolar;
		cvLogPolar(&s, &d, cvPoint2D32f(x, y), 1, CV_INTER_LINEAR);
		//cvLinearPolar(&s, &d, cvPoint2D32f(x, y), mWidht, CV_INTER_LINEAR);
		//imwrite("cvLogPolar2.png",mPolar);
		//Mat t = Mat::zeros(square.rows, square.cols,square.type()); 
		//CvMat m = t;
		//cvLinearPolar(&d, &m, cvPoint2D32f(x, y), 60, CV_INTER_LINEAR|CV_WARP_INVERSE_MAP );
		//imwrite("cvLogPolar3.png",t);
		for( int i = 0 ; i < mPolar.rows ; i ++ ) {
			for( int j = 0 ; j < mPolar.cols ; j ++ ) {
				mPortraits(i, j) = mPolar.at<Vec3b>(i,j)[0];
			}
		}
		return mPortraits;
	};
	inline vector<bin>& getPortrait(Mat& src) {//src CV_8UC3 //only 8uc1 8uc3 can be ROI!!! 32fc1 not
		//Mat in = Mat::zeros(src.rows+2*mRadius+1, src.cols+2*mRadius+1, CV_8UC3);
		//Mat dst(in, Rect(mRadius, mRadius, src.cols, src.rows));
		//src.copyTo(dst);
		mOutput.clear();
		Vec3b center;
		for( int i = 0 ; i < src.rows ; i ++ ) {
			for( int j = 0 ; j < src.cols ; j ++ ) {
				//Mat roi(in, Rect(j, i, 2*mRadius, 2*mRadius));
				center = src.at<Vec3b>(i, j);
				if( !mAvailableColor || fe(mColor, center) ){
					getCenterPointPortrait( src, center, 11, 12 );
					mOutput.push_back(mPortraits);
				}
			}
		}
		return mOutput;
	};
private:
	Mat mPolar;
	vector<bin> mOutput;
	int mWidht;
	int mHeight;
	int mRadius;
	bin mPortraits;
	bool mAvailableColor;
	Vec3b mColor;
	//bin::step mStep;
};


#endif