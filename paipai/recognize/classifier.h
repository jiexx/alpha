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
		int samples = l.getCountOfChars();
		int classes = l.getCountOfFonts();
		Mat tra( W*H, samples*classes,  CV_32FC1 );
		Mat cls( 1, samples*classes, CV_32FC1 );
		for( int i = 0 ; i < samples ; i ++ ) {
			for( int j = 0 ; j < classes ; j ++ ) {
				cls.row( i*samples + j ) = Scalar(i);
				vector<Mat*>* f = l.getFontCharSet(j);
				if( f ) {
					Mat* c = (*f)[i];
					if( c ) {
						Mat m;
						c->convertTo(m, CV_32FC1);
						m.reshape(0, 1);
						tra.row( i*samples + j ) = m.clone();
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
		threshold(out, out, 0, 255, CV_THRESH_BINARY);
	}
	inline void byErode( const Mat& img, Mat& out ) {
		Mat e1;
		e1 = getStructuringElement(MORPH_RECT, Size(2,2), Point(-1,-1) ); 

		cvtColor(img, out, CV_BGR2GRAY); 
		threshold(out, out, 0, 255, CV_THRESH_BINARY);

		erode(out, out, e1);
	}
	inline vector<char> findByBondRects( CvKNearest& knn, Mat& img, vector<Rect>& r ){
		vector<char> out;
		for (unsigned int i = 0; i < r.size(); ++i) { 
			Mat nearest(1, K, CV_32FC1);
			Mat row;

			Mat roi(img, r[i]);
			Mat unit = Mat::zeros(((int)W), ((int) H), img.type());
			resize( roi, unit, Size(W,H), 0, 0 );

			cvtColor( unit, unit, CV_BGR2GRAY);
			threshold(unit, unit, 0, 255, CV_THRESH_BINARY);

			thinning(unit);

			unit.reshape(0, 1);
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
		vector<Rect>& r = rs.getNormalRects(m);
		if( r.size() == 6 ) {
			return findByBondRects( knn, img, r );
		}

		byChanales(img, 1, m);
		r = rs.getNormalRects(m);
		if( r.size() == 6 ) {
			return findByBondRects( knn, img, r );
		}

		byChanales(img, 2, m);
		r = rs.getNormalRects(m);
		if( r.size() == 6 ) {
			return findByBondRects( knn, img, r );
		}

		byErode( img, m );
		r = rs.getNormalRects(m);
		if( r.size() == 6 ) {
			return findByBondRects( knn, img, r );
		}
	}

}