#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "map"
#include "vector"
#include "iterator"

using namespace std;
using namespace cv;

#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "thinner.h"

enum GA {
	COUNT_FIXED,
};
enum RSIZE {
	W = 32,
	H = 32,
	DW = 14,
	DW0 = 3,
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
	inline Rect mergeRects( const vector<Rect>& rects ) {
		unsigned int minx = 0, maxx = 0, miny = 0, maxy = 0;
		for( unsigned int i = 0; i < rects.size(); ++i ) {
			if( rects[minx].tl().x > rects[i].tl().x ) minx = i;
			if( rects[maxx].br().x < rects[i].br().x ) maxx = i;
			if( rects[miny].tl().y > rects[i].tl().y ) miny = i;
			if( rects[maxy].br().y < rects[i].br().y ) maxy = i;
		}
		int x = rects[minx].tl().x;
		int y = rects[miny].tl().y;
		int width = rects[maxx].br().x - rects[minx].tl().x;
		int height = rects[maxy].br().y - rects[miny].tl().y;
		return Rect(x, y, width, height);
	}
	inline Rect mergeRects( const Rect& a, const Rect& b ) {
		vector<Rect> r;
		r.push_back(a);
		r.push_back(b);
		return mergeRects(r);
	}
	inline vector<Rect>& getBoundRects( Mat& m ) {
		vector<Rect>& br = getNormalRects( m );

		vector<Rect> splits;
		splits.push_back(br[0]);
		int last;
		Rect merge_r;
		for( unsigned int i = 0; i < br.size(); ++i ) {
			//try merge last one
			last = splits.size()-1;
			Rect r = mergeRects(splits[last], br[i]);
			if( r.width <= DW ) {
				splits[last] = r;
			}else {
				splits.push_back(br[i]);
			}
		}
		mRects.clear();
		for( unsigned int i = 0; i < splits.size(); ++i ) {
			mRects.push_back( splits[i] );
		}
		return mRects;
	}
	inline bool isValidRects( vector<Rect>& r ) {
		if( r.size() != 6 )
			return false;
		for( unsigned int i = 0 ; i < r.size() ; i ++ ) {
			if( r[i].width <= DW0 ) 
				return false;
		}
		return true;
	}
	inline void debugRects( vector<Rect>& r, Mat& m ){
		for( unsigned int i = 0; i < r.size(); i ++ ) {
			rectangle( m, r[i].tl(), r[i].br(), Scalar(128, 0, 0));
		}
		//imshow("debug",m);
		imwrite("debug.png",m);
	}
	inline vector<Rect>& getNormalRects( Mat& m ) {
		mRects.clear();
		vector< vector< Point> > contours;
		Mat mat;
		if( m.channels() != 1 ) 
			cvtColor( m, mat, CV_BGR2GRAY);
		else 
			m.copyTo(mat);
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
	inline vector<Mat*>* getThinMats( Mat& m ) {  // only used for loader
		getNormalRects( m );

		vector<Mat*>* out = new vector<Mat*>();

		for (unsigned int i = 0; i < mRects.size(); ++i) {
			Rect& r = mRects[i];
			Mat roi(m, r);

			Mat* o = new Mat(((int)W), ((int) H), CV_32FC1);
			if( o ) {

				adjustThinning(roi, *o, W, H);

				if( out )
					out->push_back(o);
			}
		}
		return out;
	}
	inline vector<Mat*>* getOriMats( Mat& m ) {  // only used for loader
		getNormalRects( m );

		vector<Mat*>* out = new vector<Mat*>();

		for (unsigned int i = 0; i < mRects.size(); ++i) {
			Rect& r = mRects[i];
			Mat roi(m, r);

			Mat* o = new Mat(roi.cols, roi.rows, CV_8UC3);
			if( o ) {

				roi.copyTo( *o );
				//cvtColor(*o, *o, CV_BGR2GRAY);
				threshold(*o, *o, 0, 255, CV_THRESH_BINARY);

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