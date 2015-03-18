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

class classifier {
public:
	inline CvKNearest* getKNN( loader& l ) {
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
		return new CvKNearest( tra, cls, Mat(), false, 1 );
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
	inline vector<char> findByBondRects( CvKNearest& knn, Mat& img, vector<Rect>& r ){
		vector<char> out;
		for (unsigned int i = 0; i < r.size(); ++i) { 
			Mat nearest(1, K, CV_32FC1);
			Mat row;

			Mat roi(img, r[i]);
			Mat unit = Mat::zeros(((int)W), ((int) H), img.type());
			imwrite("roi.png",roi);
			resize( roi, unit, Size(W,H), 0, 0 );

			thinning(unit);
			imwrite("unit.png",unit);

			unit = unit.reshape(0, 1);
			unit.convertTo(row, CV_32FC1);

			float result = knn.find_nearest(row, K, 0, 0, &nearest, 0);
			out.push_back((char)result);
		}
		return out;
	}
	inline const vector<char> findByKNN( CvKNearest& knn, Mat& img ) {
		rects rs;
		Mat m;

		byChanales(img, 0, m);
		imwrite("byChanales0.png",m);
		vector<Rect>& r = rs.getNormalRects(m);
		imwrite("getNormalRects0.png",m);
		if( r.size() == 6 ) {
			return findByBondRects( knn, m, r );
		}

		byChanales(img, 1, m);
		r = rs.getNormalRects(m);
		if( r.size() == 6 ) {
			return findByBondRects( knn, m, r );
		}

		byChanales(img, 2, m);
		r = rs.getNormalRects(m);
		if( r.size() == 6 ) {
			return findByBondRects( knn, m, r );
		}

		byErode( img, m );
		imwrite("byErode.png",m);
		r = rs.getBoundRects(m);
		imwrite("getBoundRects.png",m);
		if( r.size() == 6 ) {
			return findByBondRects( knn, m, r );
		}
	}

};