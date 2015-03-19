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
		memset(data, 0, BIN_R*BIN_THETA);
	};
	inline bin(bin& b){
		if( data )
			memcpy(data, b.data, BIN_R*BIN_THETA);
	};
	inline ~bin(){
		memset(data, 0, BIN_R*BIN_THETA);
	};
	inline void clear() {
		memset(data, 0, BIN_R*BIN_THETA);
	};
	inline float& operator()(int i, int j) {
		return data[i][j];
	};
	inline float* ptr() {
		return (float*)data;
	};
	inline int size() {
		return BIN_R*BIN_THETA;
	}
protected:
	float data[BIN_THETA][BIN_R];
};
class portrait {
public:
	inline portrait( int radius ) {
		mRadius = radius;
		mPolar = Mat::zeros(2*radius, 2*radius, CV_32FC1);
	};

	inline bool fe( float c1, float c2 ) const {
		return ( c1 - c2 >= -0.000001f && c1 - c2 <= 0.000001f );
	};

	inline bin& getCenterPointPortrait( const Mat& square ) { //CV_32FC1
		mPortraits.clear();
		mPolar = 0;

		cvLogPolar(&square, &mPolar, cvPoint2D32f(mRadius, mRadius), 1);
		float center = square.at<float>(mRadius, mRadius);
		Point bin(0,0), cycle(0,0);
		Size binsz;
		binsz.width = ceil((float)mPolar.cols / BIN_R);
		binsz.height = ceil((float)mPolar.rows / BIN_THETA);
		for( int i = 0 ; i < mPolar.rows ; i ++, cycle.y ++ ) {
			if( cycle.y == binsz.width ) { //ceil for ==, floor for >
				bin.y ++;
				cycle.y = 0;
			}
			for( int j = 0 ; j < mPolar.cols ; j ++, cycle.x ++ ) {
				if( cycle.x == binsz.width ) {
					bin.x ++;
					cycle.x = 0;
				}
				if( fe( center, mPolar.at<float>(i,j) ) ) {
					mPortraits(i,j) ++;
				}
			}
		}
		return mPortraits;
	};
	inline vector<bin>& getPortrait(Mat& src) {//CV_32FC1
		Mat in = Mat::zeros(src.rows+2*mRadius, src.cols+2*mRadius, CV_32FC1);
		mOutput.clear();

		Mat dst(in,Rect(mRadius, mRadius, src.rows, src.cols));
		src.copyTo(dst);
		for( int i = 0 ; i < src.rows ; i ++ ) {
			for( int j = 0 ; j < src.cols ; j ++ ) {
				Mat roi(in, Rect(mRadius+i, mRadius+j, mRadius, mRadius));
				getCenterPointPortrait( roi );
				mOutput.push_back(mPortraits);
			}
		}
		return mOutput;
	};
	inline int step() {
		return mStep;
	}
private:
	Mat mPolar;
	int mStep;
	vector<bin> mOutput;
	int mRadius;
	bin mPortraits;

};


#endif