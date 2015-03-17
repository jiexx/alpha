#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "map"
#include "vector"

using namespace std;
using namespace cv;

#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

enum GA {
	COUNT_FIXED,
};
enum RSIZE {
	W = 32,
	H = 32
};
inline bool sort_rect(const Rect &v1, const Rect &v2) {
	return v1.x < v2.x;
}
class rects {
public:
	inline rects(){ 
		mRects.clear(); 
	};
	~rects() {
		mRects.clear();
	};
	inline vector<Rect>& getNormalRects( Mat& m ) {
		vector< vector< Point> > contours;
		Mat mat;
		cvtColor( m, mat, CV_BGR2GRAY);
		findContours(mat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		for (unsigned int i = 0; i < contours.size(); ++i) {      
			mRects.push_back( boundingRect(Mat(contours[i])) ); 
		}
		sort( mRects.begin(), mRects.end(), sort_rect );

		/*for( unsigned int i = 0; i < mRects.size(); i ++ ) {
			rectangle( m, mRects[i].tl(), mRects[i].br(), Scalar(255, 0, 0));
		}*/
		return mRects;
	};
	inline vector<Mat*>* getNormalMats( Mat& m ) {
		getNormalRects( m );

		vector<Mat*>* out = new vector<Mat*>();

		for (unsigned int i = 0; i < mRects.size(); ++i) {
			Mat roi(m, mRects[i]);

			Mat* o = new Mat(((int)W), ((int) H), m.type());
			if( o ) {
				resize( roi, *o, o->size(), 0, 0 );
			
				if( out )
					out->push_back(o);
			}
		}
		return out;
	}
protected:
	
	vector<Rect> mRects;

};

#endif