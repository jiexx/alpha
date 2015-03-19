#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "map"
#include "vector"
#include "loader.h"
#include "rectangle.h"
#include "portrait.h"

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
		mParams.kernel_type = CvSVM::LINEAR ;
		mParams.degree      = 10;
		mParams.gamma       = 5.383;
		mParams.coef0       = 1;
		mParams.C           = 2.67;
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
						c->convertTo(m, CV_32FC1, 1.0f/255.0f);
						m = m.reshape(0, 1);
						m.copyTo( tra.row( i*samples + j ) );
					}
				}
			}
		}
		return new svmWrapper( tra, cls );
	}
	inline wrapper* getSC( loader& l, int radius ) {
		if( l.type() != ORIGIN )
			return 0;
		int fonts = l.getCountOfFonts();
		int chars = l.getCountOfChars();

		Vector<float> lables;
		Vector<bin> samples;
		portrait    potr(radius);
		
		for( int i = 0 ; i < chars ; i ++ ) { // chars
			for( int j = 0 ; j < fonts ; j ++ ) { //font
				vector<Mat*>* f = l.getFontCharSet(j);
				if( f ) {
					Mat* c = (*f)[i];
					if( c ) {
						Mat m;
						c->convertTo(m, CV_32FC1, 1.0f/255.0f);
						vector<bin>& b = potr.getPortrait(m);
						for( unsigned int k = 0 ; k < b.size() ; k ++ ) {
							lables.push_back( (float)i );
							samples.push_back(b[i]);
						}
					}
				}
			}
		}
		lables.push_back( -1.0f );
		samples.push_back( bin(1.0f) );

		lables.push_back( -2.0f );
		samples.push_back( bin(0.0f) );

		Mat cls( lables.size(), 1, CV_32FC1, lables.begin() );
		Mat tra( samples.size(), BIN_THETA*BIN_R, CV_32FC1, samples.begin() );
		
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
			unit.convertTo(row, CV_32FC1, 1.0f/255.0f);

			int result = wp.find(row);
			out.push_back((char)result);
		}
		return out;
	}
	inline const vector<char> findBySC( wrapper& wp, Mat& img, int radius ) {
		portrait    potr(radius, false);
		int result;
		vector<char> out;

		vector<bin>& b = potr.getPortrait(img);
		for( unsigned int i = 0 ; i < b.size() ; i ++ ) {
			Mat row( b[i].size(), 1, CV_32FC1, b[i].ptr() );
			result = wp.find(row);
			if( result > 0 )
				out.push_back((char)result);
		}
		return out;
	}
	inline const vector<char> findByKNN( wrapper& wp, Mat& img ) {
		rects rs;
		Mat m;

		byChanales(img, 0, m);
		vector<Rect>& r = rs.getNormalRects(m);
		if( rs.isValidRects(r) ) {
			return findByBondRects( wp, m, r );
		}

		byChanales(img, 1, m);
		r = rs.getNormalRects(m);
		if( rs.isValidRects(r) ) {
			return findByBondRects( wp, m, r );
		}

		byChanales(img, 2, m);
		imwrite("byChanales0.png",m);
		r = rs.getNormalRects(m);
		imwrite("getNormalRects0.png",m);
		if( rs.isValidRects(r) ) {
			return findByBondRects( wp, m, r );
		}

		byErode( img, m );
		imwrite("byErode.png",m);
		r = rs.getBoundRects(m);
		thinning(m);
		imwrite("thinning.png",m);
		if( rs.isValidRects(r) ) {
			return findByBondRects( wp, m, r );
		}
	}

};