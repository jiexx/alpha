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
	inline class step {
	public:
		int x;
		int y;
		inline step() :x(0),y(0),ix(0) {
		};
		inline void make( int w, int h ) {
			//guard_xe = ceil((float)w / BIN_R);
			guard_y = (int)ceil((float)h / BIN_THETA);
			guard_x[0] = 2;
			guard_x[1] = 4;
			guard_x[2] = 8;
			guard_x[3] = 16;
			guard_x[4] = 32;
		};
		inline void inc_x(){
			if( ++x == guard_x[ix] ) { //ceil for ==, floor for >
				ix = (ix++) & 0x00000007;
				x = 0;
			}
		};
		inline void inc_y(){
			if( ++y == guard_y ) {
				y = 0;
			}
		};
		inline void reset(){
			ix = 0;
			x = 0;
			y = 0;
		};
	protected:
		int guard_x[BIN_R];
		int ix;
		//int guard_xe;
		int guard_y;
	};
protected:
	float data[BIN_THETA][BIN_R];
};

class portrait {
public:
	inline portrait( int radius, bool clr = true ) {
		mRadius = radius;
		mPolar = Mat::zeros(2*radius, 2*radius, CV_32FC1);
		mAvailableColor = clr;
		mColor = 255.0f;
		mStep.make(2*radius, 2*radius);
	};

	inline bool fe( float c1, float c2 ) const {
		return ( c1 - c2 >= -0.000001f && c1 - c2 <= 0.000001f );
	};

	inline bin& getCenterPointPortrait( const Mat& square, float center ) { //CV_32FC1

		mPortraits.clear();
		mPolar = 0;
		mStep.reset();

		
		cvLogPolar(&square, &mPolar, cvPoint2D32f(mRadius, mRadius), 1);
		
		for( int i = 0 ; i < mPolar.rows ; i ++, mStep.inc_x() ) {
			for( int j = 0 ; j < mPolar.cols ; j ++, mStep.inc_y() ) {
				if( fe( center, mPolar.at<float>(i,j) ) ) {
					mPortraits(mStep.x,mStep.y) ++;
				}
			}
		}
		Mat debug(BIN_R, BIN_THETA, CV_32FC1, mPortraits.ptr() );
		imwrite("debug",debug);
		return mPortraits;
	};
	inline vector<bin>& getPortrait(Mat& src) {//src CV_8UC3 //only 8uc1 can be ROI!!!
		Mat ss = Mat::zeros(W, H, CV_8UC1);

		Mat in = Mat::zeros(src.rows+2*mRadius, src.cols+2*mRadius, CV_8UC1);
		
		resize( src, ss, Size(src.rows, src.cols), 0, 0 );
		Mat roi(in, Rect(1, 1, src.rows, src.cols));
		ss.copyTo(roi);
		imwrite("in0.png",in);

		mOutput.clear();
		imwrite("src.png",ss);
		Mat dst(in, Rect(mRadius, mRadius, src.rows, src.cols));  
		ss.copyTo(dst);
		imwrite("in.png",in);
		float center;
		for( int i = 0 ; i < src.rows ; i ++ ) {
			for( int j = 0 ; j < src.cols ; j ++ ) {
				Mat roi(in, Rect(j, i, 2*mRadius, 2*mRadius));
				center = roi.at<float>(mRadius, mRadius);
				if( !mAvailableColor || fe(mColor, center) ){
					getCenterPointPortrait( roi, center );
					mOutput.push_back(mPortraits);
				}
			}
		}
		return mOutput;
	};
private:
	Mat mPolar;
	vector<bin> mOutput;
	int mRadius;
	bin mPortraits;
	bool mAvailableColor;
	float mColor;
	bin::step mStep;
};


#endif