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

class fixSVM : public CvSVM {
	virtual bool set_params( const CvSVMParams& _params )
	{
		bool ok = false;

		CV_FUNCNAME( "CvSVM::set_params" );

		__BEGIN__;

		int kernel_type, svm_type;

		params = _params;

		kernel_type = params.kernel_type;
		svm_type = params.svm_type;

		if( kernel_type != LINEAR && kernel_type != POLY &&
			kernel_type != SIGMOID && kernel_type != RBF )
			CV_ERROR( CV_StsBadArg, "Unknown/unsupported kernel type" );

		if( kernel_type == LINEAR )
			params.gamma = 1;
		else if( params.gamma <= 0 )
			CV_ERROR( CV_StsOutOfRange, "gamma parameter of the kernel must be positive" );

		if( kernel_type != SIGMOID && kernel_type != POLY )
			params.coef0 = 0;
		else if( params.coef0 < 0 )
			CV_ERROR( CV_StsOutOfRange, "The kernel parameter <coef0> must be positive or zero" );

		if( kernel_type != POLY )
			params.degree = 0;
		else if( params.degree <= 0 )
			CV_ERROR( CV_StsOutOfRange, "The kernel parameter <degree> must be positive" );

		if( svm_type != C_SVC && svm_type != NU_SVC &&
			svm_type != ONE_CLASS && svm_type != EPS_SVR &&
			svm_type != NU_SVR )
			CV_ERROR( CV_StsBadArg, "Unknown/unsupported SVM type" );

		if( svm_type == ONE_CLASS || svm_type == NU_SVC )
			params.C = 0;
		else if( params.C <= 0 )
			CV_ERROR( CV_StsOutOfRange, "The parameter C must be positive" );

		if( svm_type == C_SVC || svm_type == EPS_SVR )
			params.nu = 0;
		else if( params.nu <= 0 || params.nu >= 1 )
			CV_ERROR( CV_StsOutOfRange, "The parameter nu must be between 0 and 1" );

		if( svm_type != EPS_SVR )
			params.p = 0;
		else if( params.p <= 0 )
			CV_ERROR( CV_StsOutOfRange, "The parameter p must be positive" );

		if( svm_type != C_SVC )
			params.class_weights = 0;

		params.term_crit.epsilon = MAX( params.term_crit.epsilon, DBL_EPSILON );
		ok = true;

		__END__;

		return ok;
	}
};

class svmWrapper : public wrapper {
public:
	inline svmWrapper(const Mat& tra, const Mat& cls){
		CvSVMParams mParams;
		mParams.svm_type    = CvSVM::C_SVC;
		mParams.kernel_type = CvSVM::LINEAR ;
		//mParams.degree      = 10;
		//mParams.gamma       = 5.383;
		//mParams.coef0       = 1;
		mParams.C           = 1000;//2.67;
		//mParams.nu          = 0.5;
		//mParams.p           = 0.1;
		mParams.term_crit   = cvTermCriteria(CV_TERMCRIT_EPS, 10000,  FLT_EPSILON);

		mSVM = new fixSVM();
		if( mSVM ) {
			mSVM->train(tra, cls, Mat(), Mat(), mParams);
		}
	};
	inline ~svmWrapper() {
		if( mSVM ) {
			delete mSVM;
		}
	};
	inline int find(Mat& m) {
		if( mSVM )
			return (int) mSVM->predict(m);
		return -1;
	};
	inline int find(bin& b) {
		Mat row( 1, b.size(), CV_32FC1, b.ptr() );
		return find(row);
	};
protected:
	fixSVM* mSVM;
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
	inline int find(bin& b) {
		return 0;
	};
protected:
	CvKNearest* mKNN;
};
bool sortc(const char &v1, const char &v2){
		return v1 < v2;
	}
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
		portrait    potr(radius, POR_WHITE);


		Mat m1 = Mat::zeros(radius*2, radius*2, CV_8UC3);
		m1 = Scalar(255,255,255,255);
		
		bin& bw = potr.getCenterPointPortrait(m1, radius, radius);
		lables.push_back( -1.0f );
		imwrite("toMat.png", bw.toMat());
		samples.push_back(bw);

		lables.push_back( -2.0f );
		samples.push_back(bin(0.0f));

		for( int i = 0 ; i < chars ; i ++ ) { // chars
			for( int j = 0 ; j < fonts ; j ++ ) { //font
				vector<Mat*>* f = l.getFontCharSet(j);
				if( f ) {
					Mat* c = (*f)[i];
					if( c ) {
						vector<bin>& b = potr.getPortrait(*c);
						for( unsigned int k = 0 ; k < b.size() ; k ++ ) {
							lables.push_back( (float)i );
							samples.push_back(b[k]);
						}
					}
				}
			}
		}
		

		Mat cls( lables.size(), 1, CV_32FC1, lables.begin() );
		Mat tra( samples.size(), bin::size(), CV_32FC1, samples.begin() );
		imwrite("tra.png",tra);
		return new knnWrapper( tra, cls );
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

	inline int eqColor( Vec3b& hls1, Vec3b& hls2 ) {
		const int threshold = 10;
		int h = abs(hls1[0] - hls2[0]), l = abs(hls1[1] - hls2[1]);
		if( h < threshold && l < 2*threshold )
			return 1; //same color
		return 0;
	}

	inline void addColor( vector<Vec3b>& ca, Vec3b clr ) {
		unsigned int i = 0;
		while( i < ca.size() && !eqColor( ca[i], clr ) ) 
			i++;
		if( i != ca.size() )
			return;
		ca.push_back(clr);
	}

	inline int findColor( vector<Vec3b>& ca, Vec3b clr ) {
		unsigned int i;
		for( i = 0 ; i < ca.size() && !eqColor( ca[i], clr ) ; i ++ ) ;
		if( i != ca.size() )
			return i;
		return -1;
	}

	inline void byColor( Mat& img, Mat& output ) {
		Mat ero, hls;
		byErode( img, ero );
		cvtColor( img, hls, CV_RGB2HLS);

		uchar *mask;
		Vec3b *color, *dst;
		vector<Vec3b> clrs;
		vector<Point> pts;
		for( int i = 0 ; i < ero.rows ; i ++ ) {
			mask = ero.ptr<uchar>(i);
			color = hls.ptr<Vec3b>(i);
			for( int j = 0 ; j < ero.cols ; j ++ ) {
				if( mask[j] > 0  ) {
					addColor( clrs, color[j] );
					pts.push_back(Point(j,i));
				}
			}
		}

		vector<int> hull; 
		vector<Point> contours;
		convexHull(Mat(pts), hull, true);
		for( unsigned int i = 0 ; i < hull.size() ; i ++ ) {
			contours.push_back(pts[hull[i]]);
		}

		int clr;
		for( int i = 0 ; i < ero.rows ; i ++ ) {
			color = hls.ptr<Vec3b>(i);
			dst = output.ptr<Vec3b>(i);
			for( int j = 0 ; j < ero.cols ; j ++ ) {
				if( (clr = findColor( clrs, color[j] )) >= 0
					&& pointPolygonTest( contours, Point2f((float)j,(float)i), false) >= 0 ) {
					dst[j] = clrs[clr];
				}
			}
		}
	}

	inline int findStartX( Mat& img ) {
		int startx = img.cols;
		uchar* p;
		for( int i = 0 ; i < img.rows ; i ++ ) {
			p = img.ptr<uchar>(i);
			for( int j = 0 ; j < img.cols ; j ++ ) {
				if( p[j] > 0  && startx > j ) {
					startx = j;
				}
			}
		}
		return startx;
	}
	inline const vector<char> findBySC( wrapper& wp, Mat& img, int radius ) {
		portrait    potr(radius, POR_POINT, &wp);
		vector<char> out;
		
		Mat ero;
		byErode( img, ero );
		int startx = findStartX( ero );

		Mat dst;
		cvtColor(ero, dst, CV_GRAY2RGB);

		int rs[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, r = -1;
		
		for( int n = 0 ; n < 6 && startx < img.cols ; n ++ ) {
			Mat roi(dst, Rect(startx, 0, 18, img.rows)); 
			vector<bin>& b = potr.getPortrait(roi);
			for( unsigned int i = 0 ; i < b.size() ; i ++ ) {
				Mat row( 1, b[i].size(), CV_32FC1, b[i].ptr() );
				int result = wp.find(row);
				if( result >= 0 ) {
					rs[result] ++;
				}
			}
			for( int j = 0 ; j < 10 ; j ++ ) {
				if( rs[j] > 0 && rs[j] > r ) 
					r = rs[j];
				rs[j] = 0;
			}
			if( r == 1 ) 
				startx += 10;
			out.push_back(r+'0');
			r = -1;
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