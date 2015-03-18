#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "map"
#include "vector"
#include "loader.h"
#include "rectangle.h"

using namespace std;
using namespace cv;

enum KNN_ARGS{
	K = 1
};

class wrapper {
public:
	virtual int find(Mat& m) = 0;
};

class svmWrapper : public wrapper {
public:
	inline svmWrapper(const Mat& tra, const Mat& cls){
		CvSVMParams mParams;
		mParams.svm_type    = CvSVM::C_SVC;
		mParams.kernel_type = CvSVM::RBF ;
		mParams.degree      = 10;
		mParams.gamma       = 8;
		mParams.coef0       = 1;
		mParams.C           = 10;
		mParams.nu          = 0.5;
		mParams.p           = 0.1;
		mParams.term_crit   = cvTermCriteria(CV_TERMCRIT_EPS, 1000,  FLT_EPSILON);

		mSVM = new CvSVM(tra, cls, Mat(), Mat(), mParams);
	}
	inline int find(Mat& m) {
		if( mSVM )
			return (int) mSVM->predict(m);
		return -1;
	};
protected:
	CvSVM* mSVM;
};

class knnWrapper : public wrapper {
public:
	inline knnWrapper(const Mat& tra, const Mat& cls){

		mKNN = new CvKNearest( tra, cls, Mat(), false, 1 );
	}
	inline int find(Mat& m) {
		Mat nearest(1, K, CV_32FC1);
		if( mKNN )
			return (int) mKNN->find_nearest(m, K, 0, 0, &nearest, 0);
		return -1;
	};
protected:
	CvKNearest* mKNN;
};

class classifier {
public:
	inline wrapper* getKNN( loader& l ) {
		int samples = l.getCountOfFonts();
		int classes = l.getCountOfChars();
		Mat tra( classes*samples, W*H, CV_32FC1 );
		Mat cls( classes*samples, 1, CV_32FC1 );
		for( int i = 0 ; i < classes ; i ++ ) { // chars
			for( int j = 0 ; j < samples ; j ++ ) { //font
				cls.row( i*samples + j ) = i;
				vector<Mat*>* f = l.getFontCharSet(j);
				if( f ) {
					Mat* c = (*f)[i];
					if( c ) {
						Mat m;
						c->convertTo(m, CV_32FC1);
						m = m.reshape(0, 1);
						m.copyTo( tra.row( i*samples + j ) );
					}
				}
			}
		}
		return new knnWrapper( tra, cls );
	}
	inline wrapper* getSVM( loader& l ) {
		int samples = l.getCountOfFonts();
		int classes = l.getCountOfChars();
		Mat tra( classes*samples, W*H, CV_32FC1 );
		Mat cls( classes*samples, 1, CV_32FC1 );
		for( int i = 0 ; i < classes ; i ++ ) { // chars
			for( int j = 0 ; j < samples ; j ++ ) { //font
				cls.row( i*samples + j ) = i;
				vector<Mat*>* f = l.getFontCharSet(j);
				if( f ) {
					Mat* c = (*f)[i];
					if( c ) {
						Mat m;
						c->convertTo(m, CV_32FC1);
						m = m.reshape(0, 1);
						m.copyTo( tra.row( i*samples + j ) );
					}
				}
			}
		}
		return new svmWrapper( tra, cls );
	}
	inline void byChanales( const Mat& img, int bgr, Mat& out ) {
		vector<Mat> bgra(img.channels());
		split(img, bgra);
		bgra[0] = bgra[bgr];
		bgra[1] = bgra[bgr];
		bgra[2] = bgra[bgr];
		
		merge(bgra, out);

		cvtColor(out, out, CV_BGR2GRAY); 
		threshold(out, out, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	}
	inline void byErode( const Mat& img, Mat& out ) {
		Mat e1;
		e1 = getStructuringElement(MORPH_RECT, Size(2,2), Point(-1,-1) ); 

		cvtColor(img, out, CV_BGR2GRAY); 
		threshold(out, out, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

		erode(out, out, e1);
	}
	inline vector<char> findByBondRects( wrapper& wp, Mat& img, vector<Rect>& r ){
		vector<char> out;
		for (unsigned int i = 0; i < r.size(); ++i) { 
			Mat nearest(1, K, CV_32FC1);
			Mat row;

			Mat roi(img, r[i]);
			Mat unit = Mat::zeros(W, H, CV_8UC1);
			imwrite("roi.png",roi);

			adjustThinning(roi, unit, W, H);
			imwrite("unit.png",unit);

			unit = unit.reshape(0, 1);
			unit.convertTo(row, CV_32FC1);

			float result = wp.find(row);
			out.push_back((char)result);
		}
		return out;
	}
	inline const vector<char> findByKNN( wrapper& wp, Mat& img ) {
		rects rs;
		Mat m;

		byChanales(img, 0, m);
		imwrite("byChanales0.png",m);
		vector<Rect>& r = rs.getNormalRects(m);
		imwrite("getNormalRects0.png",m);
		if( r.size() == 6 ) {
			return findByBondRects( wp, m, r );
		}

		byChanales(img, 1, m);
		r = rs.getNormalRects(m);
		if( r.size() == 6 ) {
			return findByBondRects( wp, m, r );
		}

		byChanales(img, 2, m);
		r = rs.getNormalRects(m);
		if( r.size() == 6 ) {
			return findByBondRects( wp, m, r );
		}

		byErode( img, m );
		imwrite("byErode.png",m);
		r = rs.getBoundRects(m);
		imwrite("getBoundRects.png",m);
		if( r.size() == 6 ) {
			return findByBondRects( wp, m, r );
		}
	}

};