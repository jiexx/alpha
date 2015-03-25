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
		for(int i = 0 ; i < BIN_THETA*BIN_R ; i ++ ) 
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
	inline static int size() {
		return BIN_THETA*BIN_R;
	};
	inline Mat toMat() {
		return Mat(BIN_THETA, BIN_R, CV_32FC1, ptr());
	};
protected:
	float data[BIN_THETA][BIN_R];
};

enum POR {
	POR_WHITE,
	POR_POINT,
};

class wrapper {
public:
	virtual int find(Mat& m) = 0;
	virtual int find(bin& b) = 0;
};

class portrait {
public:
	inline portrait( int r,  POR mode = POR_WHITE, wrapper* wr = 0 ) {
		mWidth = BIN_R;
		mHeight = BIN_THETA;
		mRadius = r;
		mPolar = Mat::zeros(mWidth, mHeight, CV_8UC3);
		mROI = Mat::zeros(mRadius*2, mRadius*2, CV_8UC3);
		mMode = mode;
		mColor = Vec3b(255,255,255);
		mWrapper = wr;
	};
	inline bool fe( Vec3b& c1, Vec3b& c2 ) const {
		return ( c1[0] - c2[0] == 0 && c1[1] - c2[1] == 0 && c1[2] - c2[2] == 0 );
	};
	inline bool re( Vec3b& c1, Vec3b& c2 ) const {
		return ( abs(c1[0] - c2[0]) < 15 && abs(c1[1] - c2[1]) < 15 && abs(c1[2] - c2[2]) < 15 );
	};

	inline bin& getCenterPointPortrait( const Mat& square, int x, int y ) { //CV_32FC1

		mPortraits.clear();
		mPolar = 0;
		CvMat s = square, d = mPolar;
		cvLogPolar(&s, &d, cvPoint2D32f(x, y), 1, CV_INTER_LINEAR);
		for( int i = 0 ; i < mPolar.rows ; i ++ ) {
			for( int j = 0 ; j < mPolar.cols ; j ++ ) {
				mPortraits(i, j) = mPolar.at<Vec3b>(i,j)[0];
			}
		}
		return mPortraits;
	};
	inline void copyColor( Mat& dst, Mat& src, Vec3b& v3 ) {
		Vec3b *p, *to;
		for( int i = 0 ; i < src.rows ; i ++ ) {
			p = src.ptr<Vec3b>(i);
			to = dst.ptr<Vec3b>(i);
			for( int j = 0 ; j < src.cols ; j ++ ) {
				if( re( v3, p[j] ) ) {
					to[j] = mColor;
				} else {
					to[j][0] = 0;
					to[j][1] = 0;
					to[j][2] = 0;
				}
			}
		}
	};
	inline void doPortraitBase(Mat& src) {
		Vec3b center;
		for( int i = 0 ; i < src.rows ; i ++ ) {
			for( int j = 0 ; j < src.cols ; j ++ ) {
				center = src.at<Vec3b>(i, j);
				if( fe(mColor, center) ){
					getCenterPointPortrait( src, j, i );
					mOutput.push_back(mPortraits);
					//stringstream sk;
					//sk<< j;
					//imwrite( (sk.str()+string("-key.png")).c_str(), mOutput[mOutput.size()-1].toMat() );
				}
			}
		}
	};
	inline vector<bin>& getPortrait(Mat& src) {//src CV_8UC3 //only 8uc1 8uc3 can be ROI!!! 32fc1 not
		mOutput.clear();
		Vec3b center;
		if( POR_WHITE == mMode ) {
			doPortraitBase( src );
		}else if( POR_POINT == mMode ) {
			Mat dst = Mat::zeros(src.rows+mRadius*2, src.cols+mRadius*2, CV_8UC3);
			Mat c(dst, Rect(mRadius, mRadius, src.cols, src.rows));
			src.copyTo(c);
			for( int i = 0 ; i < src.cols ; i ++ ) {
				for( int j = 0 ; j < src.rows ; j ++ ) {
					center = dst.at<Vec3b>(j+mRadius, i+mRadius);
					Mat roi(dst, Rect(i, j, mRadius*2, mRadius*2));
					copyColor(mROI, roi, center);
					getCenterPointPortrait( mROI, mRadius, mRadius );
					mOutput.push_back(mPortraits);
					mPoints.push_back(Point(i,j));
				}
			}
		}
		return mOutput;
	};
	inline vector<Point>& getPoints() {
		return mPoints;
	};
private:
	Mat mPolar;
	Mat mROI;
	vector<bin> mOutput;
	vector<Point> mPoints;
	int mWidth;
	int mHeight;
	int mRadius;
	bin mPortraits;
	int mMode;
	wrapper* mWrapper;
	Vec3b mColor;
	//bin::step mStep;
};


#endif