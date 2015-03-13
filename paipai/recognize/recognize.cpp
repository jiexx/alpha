// 这是主 DLL 文件。

#include "stdafx.h"
#include <fstream>
#include "recognize.h"


#define BORDER 1
#define DEF_WIDTH 14
#define DEF_HEGITH 21
#define MAX_GUARD 100000
#define SC_LOGR 5
#define SC_THETA 12
#define SC_MAX_CORNERS 6

recognize::recognize( int numofcls, int numofsamp ){
	K = 1;
	mNormSize = 32;
	mKNN = NULL;
	mNumOfCls = numofcls;
	mNumOfSamples = numofsamp;
	mTrainData = cvCreateMat( mNumOfSamples*mNumOfCls, mNormSize*mNormSize, CV_32FC1 );// rows, col, type
	mTrainClasses = cvCreateMat( mNumOfSamples*mNumOfCls, 1, CV_32FC1 );
	mTrainData2 = Mat::zeros(Size(mNumOfSamples*mNumOfCls, SC_LOGR*SC_THETA*SC_MAX_CORNERS), CV_32FC1);
	mTrainClasses2 = Mat::zeros(Size(mNumOfSamples*mNumOfCls, 1), CV_32FC1);
}
recognize::~recognize(){
	if( mArrImg ) {
		for( int i = 0; i < mNumOfCls*mNumOfSamples; i ++ ) {
			if( mArrImg[i] ) {
				cvReleaseImage( &mArrImg[i] );
				mArrImg[i] = NULL;
			}
		}
		delete[] mArrImg;
		mArrImg = NULL;
	}
	if( mKNN ) {
		delete mKNN;
		mKNN = NULL;
	}
}

void write( const char* f, vector<unsigned char>& v ) {
	char str[8];
	ofstream of(f, ios_base::binary|ios_base::app);
	for ( vector<unsigned char>::iterator itr=v.begin(); itr!=v.end(); itr++ ) {
		memset(str, 0, 8);
		sprintf(str, "%02x", *itr);
		of << str;
	}
	of << '\n';
}
void read( const char* in, vector<unsigned char>& v ) {
	int len = strlen(in);
	unsigned int h, l;
	for ( int i = 0; i < len; i ++ ) {
		h = in[i] < 'a' ?  in[i]-'0' : in[i]-'a'+10;
		i++;
		l = in[i] < 'a' ?  in[i]-'0' : in[i]-'a'+10;
		v.push_back( (h<<4)+l );
	}
}

string i2a(int x) {
	std::ostringstream o;
	if (o << x)
		return o.str();
	return "";
}

Mat toPolar(Mat image , int centerx, int centery, int m)
{
	if(image.empty())
		return image;
	Mat pImage = Mat(image.size(), CV_8UC1);
	Mat result = Mat::zeros(Size(SC_LOGR, SC_THETA), CV_8UC1);
	IplImage ipl_a = image, ipl_pa = pImage;
	cvLogPolar(&ipl_a, &ipl_pa, cvPoint2D32f(centerx, centery), m);
	
	int theta_y_unit = (int)ceil(((float)pImage.rows)/SC_THETA), 
		logr_x_unit = (int)ceil(((float)pImage.cols)/SC_LOGR),
		theta_y = 0, logr_x = 0, countx = 0, county = 0;
	unsigned char* p, *to;  
	for( int i = 0; i < pImage.rows; i ++ ) {
		if( county == theta_y_unit ) {
			county = 0;
		}else {
			county ++;
		}
		if( !county ) {
			theta_y ++;
			to = result.ptr<unsigned char>(theta_y);  
		}
		p = pImage.ptr<unsigned char>(i);  
		for( int j = 0; j < pImage.cols; j ++ ) {
			if( countx == logr_x_unit ) {
				countx = 0;
			}else {
				countx ++;
			}
			if( !countx ) {
				logr_x ++;
			}
			if( p[j] > 0 ) {
				to[logr_x] ++;
			}
		}
	}
	return result;
}

void recognize::load2( const char imageslist[][8], int size, bool conv_save ){
	if( size != mNumOfCls*mNumOfSamples && size != 0 ) 
		return;
	mArrImg = (IplImage**)new IplImage[mNumOfCls*mNumOfSamples*SC_MAX_CORNERS];
	for(int i = 0; i < mNumOfCls; i++){
		for(int j = 0; j < mNumOfSamples; j++){
			vector<Point> corners;
			Mat image = imread( imageslist[i*mNumOfSamples + j], CV_LOAD_IMAGE_GRAYSCALE ); 
			goodFeaturesToTrack( image, corners, SC_MAX_CORNERS, 0.01, 10, Mat(), 3, false, 0.04 );
			
			Mat conv = Mat::zeros(Size(image.rows, image.cols), CV_8UC3);
			
			for( int y = 0; y < conv.rows; y ++ ){
				unsigned char* inp  = conv.ptr<unsigned char>(y);
				for( int x = 0; x < conv.cols; x ++ ){
					int v = image.at<uchar>(y,x);
					if(  v > 0 ) {
						*inp++ = 255;
						*inp++ = 255 ;
						*inp++ = 255;
					}else {
						inp++;
						inp++;
						inp++;
					}
				}
			}
			for( unsigned int l = 0; l < corners.size(); l++ ) {
				circle( conv, corners[l], 2, Scalar(0, 0, 255));
			}
			imwrite("goodFeaturesToTrack.jpg", conv);

			for( unsigned int k = 0; k < corners.size(); k++ ) {
				Mat result = toPolar(image, corners[k].x, corners[k].y, CV_WARP_FILL_OUTLIERS );
				mArrImg[(i*mNumOfSamples + j)*SC_MAX_CORNERS + k] = cvCloneImage(&(IplImage)image);
				imwrite("mTrainData2.jpg", result);
			}
		}
	}
}

void recognize::load( const char imageslist[][8], int size, bool conv_save ){
	if( size != mNumOfCls*mNumOfSamples && size != 0 ) 
		return;
	
	mArrImg = (IplImage**)new IplImage[mNumOfCls*mNumOfSamples];
	for(int i = 0; i < mNumOfCls; i++){
		for(int j = 0; j< mNumOfSamples; j++){
			if( dat != NULL ) {
				vector<unsigned char> data/*(str, str+strlen(str))*/;
				read(dat[i*mNumOfSamples + j], data);
				Mat image = imdecode(Mat(data), CV_LOAD_IMAGE_GRAYSCALE );
				//ofstream os(i2a(i)+i2a(j)+".png",ios_base::binary|ios_base::out);
				//for ( vector<unsigned char>::iterator itr=data.begin(); itr!=data.end(); itr++ ) {
				//	os << *itr;
				//}
				mArrImg[i*mNumOfSamples + j] =  cvCloneImage(&(IplImage)image);
				//imwrite(i2a(i)+i2a(j)+".png",Mat());
			} else {
				mArrImg[i*mNumOfSamples + j] = cvLoadImage(imageslist[i*mNumOfSamples + j],0);
				if( conv_save ) {
					Mat src = Mat(mArrImg[i*mNumOfSamples + j],0);
					vector<int> p;
					p.push_back(CV_IMWRITE_PNG_STRATEGY_DEFAULT);
					p.push_back(0);//other value have problem issue. must be no compressed.
					vector<unsigned char> buf;
					//imencode(".png", src, buf/*, p*/);
					ifstream is(imageslist[i*mNumOfSamples + j],ios_base::binary|ios_base::in);
					char c;
					while( is.read(&c, 1) ) {
						buf.push_back(c);
					}
					write( "result"/*(i2a(i) + i2a(j)).c_str()*/, buf);
					//Mat image = imdecode(Mat(buf), 0);
					//imwrite(i2a(i)+i2a(j)+".png",image);
				}
			}
		}
	}

	
}
IplImage* recognize::preprocessing(IplImage* src){
	IplImage* img = cvCreateImage( cvSize( mNormSize, mNormSize ), IPL_DEPTH_32F, 1 );
	//convert 8 bits image to 32 float image
	cvConvertScale(src, img, 1, 0);
	return img;
}
void recognize::prepare(){
	CvMat cls, dat;
	IplImage* img;
	for(int i = 0; i < mNumOfCls; i++){
		for(int j = 0; j< mNumOfSamples; j++){
			// Set class label
			cvGetRow( mTrainClasses, &cls, i*mNumOfSamples + j );
			cvSet(&cls, cvRealScalar(i));

			img = preprocessing(mArrImg[i*mNumOfSamples + j]);

			// Set data
			cvGetRow( mTrainData, &cls, i*mNumOfSamples + j);
			//IplImage* ipl = cvCreateImage( cvSize( mNormSize, mNormSize ), IPL_DEPTH_32F, 1 );
			//cvConvertScale(img, ipl, 1, 0);

			cvGetSubRect( img, &dat, cvRect(0,0,mNormSize,mNormSize) );
			CvMat row_header, *row1;
			row1 = cvReshape( &dat, &row_header, 0, 1 );

			cvCopy(row1, &cls, NULL);

		}
	}
	mKNN = new CvKNearest( mTrainData, mTrainClasses, 0, false, K );
	//imwrite( "mTrainData.jpg", Mat(mTrainData) );
	//imwrite( "mTrainClasses.jpg", Mat(mTrainClasses) );
	//if( mKNN ) {
	//	mKNN->save();
	//}
}
void recognize::prepare2() {
	Mat cls;
	IplImage* img;
	for(int i = 0; i < mNumOfCls; i++){
		for(int j = 0; j< mNumOfSamples; j++){

			for( int k = 0; k < SC_MAX_CORNERS; k++ ) {
				// Set class label
				cls = mTrainClasses2.row(i*mNumOfSamples + j );
				cvSet(&cls, cvRealScalar(i));

				img = preprocessing(mArrImg[(i*mNumOfSamples + j)*SC_MAX_CORNERS + k]);

				cls = Mat(img);

				cls.reshape(0, 1);
				cls.copyTo( mTrainData2.row((i*mNumOfSamples + j)*SC_MAX_CORNERS + k).col(SC_LOGR*SC_THETA*k) );
			}
		}
	}
	mKNN2 = new CvKNearest( mTrainData2, mTrainClasses2, Mat(), false, K );
}
int recognize::classify(IplImage* img){
	CvMat dat;
	IplImage* p;
	CvMat* nearest=cvCreateMat(1,K,CV_32FC1);

	p = preprocessing(img);
	//imwrite( "p.jpg", Mat(p) );


	cvGetSubRect( p, &dat, cvRect(0,0,mNormSize,mNormSize) );
	CvMat row_header, *row1;
	row1 = cvReshape( &dat, &row_header, 0, 1 );
	//imwrite( "row1.jpg", Mat(row1) );

	float result = mKNN->find_nearest(row1,K,0,0,nearest,0);
	//imwrite( "nearest.jpg", Mat(nearest) );
	cvReleaseImage(&p);
	return (int) result;
}

bool recognize::sortx(const Piece &v1, const Piece &v2){
	return v1.x < v2.x;
}

IplImage* bmp2ipl(BITMAPINFO* bi, BYTE* data) {
	int channal=(bi->bmiHeader.biBitCount == 1) ? 1:(bi->bmiHeader.biBitCount/8);
	//仅限于二值图像和灰度图像
	//int depth  =(pbmpinfo->bmiHeader.biBitCount == 1) ? IPL_DEPTH_1U : IPL_DEPTH_8U;
	int depth  =  IPL_DEPTH_8U;

	int width=bi->bmiHeader.biWidth;
	int heigh=-bi->bmiHeader.biHeight;

	IplImage *pmg;

	pmg=cvCreateImageHeader(cvSize(width,heigh),depth,channal);
	pmg->imageData=(char*)malloc(pmg->imageSize);
	pmg->origin=1;
	//cvSetData();
	memcpy(pmg->imageData, data, pmg->imageSize);

	return pmg;
}

IplImage* capture(HWND hWnd) {  

	RECT rect;  
	GetWindowRect(hWnd, &rect);  

	int nWidht = rect.right - rect.left;  
	int nHeight = rect.bottom - rect.top;  

	HDC hDC = GetWindowDC(hWnd);  
	HDC hComatibleDC = CreateCompatibleDC(hDC);    
	HBITMAP hBMP = CreateCompatibleBitmap(hDC, nWidht, nHeight);  
	HBITMAP hOldBMP = (HBITMAP)SelectObject(hComatibleDC, hBMP);  
	BitBlt(hComatibleDC, 0, 0, nWidht, nHeight, hDC, 0, 0, SRCCOPY);  

	BITMAP bitmap = {0};  
	GetObject(hBMP, sizeof(BITMAP), &bitmap);  
	BITMAPINFOHEADER bi = {0};  
	BITMAPFILEHEADER bf = {0};  

	CONST int nBitCount = 24;  
	bi.biSize = sizeof(BITMAPINFOHEADER);  
	bi.biWidth = bitmap.bmWidth;  
	bi.biHeight = -bitmap.bmHeight; //revert graphic by vert 
	bi.biPlanes = 1;  
	bi.biBitCount = nBitCount;  
	bi.biCompression = BI_RGB;  
	DWORD dwSize = ((bitmap.bmWidth * nBitCount + 31) / 32) * 4 * bitmap.bmHeight;  

	HANDLE hDib = GlobalAlloc(GHND, dwSize + sizeof(BITMAPINFOHEADER));  
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
	*lpbi = bi;  

	GetDIBits(hComatibleDC, hBMP, 0, bitmap.bmHeight, (BYTE*)lpbi + sizeof(BITMAPINFOHEADER), (BITMAPINFO*)lpbi, DIB_RGB_COLORS);  

	//bf.bfType = 0x4d42;  
	//dwSize += sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
	//bf.bfSize = dwSize;  
	//bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  

	//file.Write((BYTE*)&bf, sizeof(BITMAPFILEHEADER));  
	//file.Write((BYTE*)lpbi, dwSize);  
	IplImage* result = bmp2ipl((BITMAPINFO*)lpbi, (BYTE*)lpbi + sizeof(BITMAPINFOHEADER));

	GlobalUnlock(hDib);  
	GlobalFree(hDib);  

	SelectObject(hComatibleDC, hOldBMP);  
	DeleteObject(hBMP);  
	DeleteDC(hComatibleDC);  
	ReleaseDC(hWnd, hDC);  
	imwrite("bmp2ipl.jpg",Mat(result));
	return result;  
} 

IplImage* focus(HWND hWnd, const CvRect& rect) {
	IplImage* ipli = capture(hWnd);
	CvMat* mat = cvGetSubRect(ipli, cvCreateMatHeader(rect.height, rect.width, CV_8UC1), rect);
	IplImage *header = cvCreateImageHeader(cvSize(rect.width, rect.height), ipli->depth, ipli->nChannels);
	IplImage* p = cvGetImage(mat, header);
	cvReleaseImage(&ipli);
	return p;
}

//Mat pickoutLines( IplImage* img ) {
//	IplImage* lines = cvCreateImage(cvGetSize(img), img->depth, 1);  
//	IplImage* result = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
//
//	CvMemStorage* storage = cvCreateMemStorage();  
//	cvCanny(img, lines, 0, 255, 3); 
//	imwrite( "line1.jpg", Mat(lines,0) ); 
//	//CvSeq* hlines = cvHoughLines2(lines, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 5, 5, 2);  
//	//for (int i=0; i<hlines->total; i++)  
//	//{  
//	//	CvPoint *line = (CvPoint *)cvGetSeqElem(hlines,i);  
//	//	cvLine(result,line[0],line[1],CV_RGB(255,0,0),1,CV_AA);  
//	//}
//	//imwrite( "result.jpg", Mat(result,0) ); 
//	return Mat(lines,0);
//}

int filterChanales( const Mat& input, Mat& output, int bgr, vector<Rect>& bounds ) {
	Mat ccc, gray, binary;
	vector<Mat> bgra(input.channels());
	split(input, bgra);
	bgra[0] = bgra[bgr];
	bgra[1] = bgra[bgr];
	bgra[2] = bgra[bgr];
	merge(bgra, ccc);

	cvtColor(ccc, gray, CV_BGR2GRAY); 
	threshold(gray, binary, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	imwrite("binary.jpg",binary);

	binary.copyTo(output);
	string a;
	a = "filterChanales";
	a += bgr+'0';
	a += ".jpg";
	imwrite(a.c_str(),output);

	vector<vector<Point>> contours;
	findContours(binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	bounds.clear();
	if( contours.size() == COUNT_DIGT ) {
		for (unsigned int i = 0; i < contours.size(); ++i) {      
			bounds.push_back( boundingRect(Mat(contours[i])) ); 
		}
		return 1;
	}
	bounds.clear();
	return 0;
}

Rect mergeRects( const vector<Rect>& rects ) {
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

Rect mergeRects( const Rect& a, const Rect& b ) {
	vector<Rect> r;
	r.push_back(a);
	r.push_back(b);
	return mergeRects(r);
}


bool sort_rect(const Rect &v1, const Rect &v2){
	return v1.x < v2.x;
}

int getBoundRect( vector<vector<Point>>& contours, vector<Rect>& output ) {
	vector<Rect> boundRect;
	for (unsigned int i = 0; i < contours.size(); ++i) {      
		boundRect.push_back( boundingRect(Mat(contours[i])) ); 
	}

	if( boundRect.size() == COUNT_DIGT ) {
		output = boundRect;
		return 1;
	}

	sort( boundRect.begin(), boundRect.end(), sort_rect );

	vector<Rect>& splits = output;
	splits.clear();
	splits.push_back(boundRect[0]);
	int last;
	Rect merge_r;
	for( unsigned int i = 0; i < boundRect.size(); ++i ) {
		//try merge last one
		last = splits.size()-1;
		Rect r = mergeRects(splits[last], boundRect[i]);
		if( r.width <= DEF_WIDTH ) {
			splits[last] = r;
		}else {
			splits.push_back(boundRect[i]);
		}
	}

	if( splits.size() == COUNT_DIGT ) {
		return 1;
	}
	return 0;
}
int filterErode( const Mat& input, Mat& output,  vector<Rect>& bounds ) {
	Mat gray, binary, erosion, dilation, mer, sub, guss, e1, e2;
	e1 = getStructuringElement(MORPH_RECT, Size(2,2), Point(-1,-1) ); 
	e2 = getStructuringElement(MORPH_RECT, Size(2,2), Point(1,1) );

	cvtColor(input, gray, CV_BGR2GRAY); 
	threshold(gray, binary, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	imwrite( "binary.jpg", binary ); 

	erode(binary, erosion, e1);
	//dilate(erosion, dilation, e2, Point(1,1), 1);
	imwrite( "dilation.jpg", erosion ); 
	
	//sub = binary - dilation;
	//imwrite( "sub.jpg", sub ); 

	//GaussianBlur(sub, guss, Size(3, 3), 0, 0); 
	//medianBlur(sub, guss, 3);
	//guss = binary - sub;
	//imwrite( "guss.jpg", guss ); 

	erosion.copyTo(output);
	
	vector< vector< Point> > contours;
	findContours(erosion, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//drawContours(dilation, contours, 0, Scalar(255, 0, 0), 1);
	if( contours.size() > 0 ) {
		if( getBoundRect(contours, bounds) ) {
			for( unsigned int i = 0; i < bounds.size(); i ++ ) {
				rectangle( erosion, bounds[i].tl(), bounds[i].br(), Scalar(255, 0, 0));
			}
			imwrite( "rectangle.jpg", erosion );
			return 1;
		}
	}
	for( unsigned int i = 0; i < bounds.size(); i ++ ) {
		rectangle( erosion, bounds[i].tl(), bounds[i].br(), Scalar(255, 0, 0));
	} 
	imwrite( "rectangle-e.jpg", erosion ); 
	bounds.clear();
	return 0;
	
}

int filterShape( const Mat& input, Mat& output,  vector<Rect>& bounds ) {
	Mat gray, binary;
	cvtColor(input, gray, CV_BGR2GRAY); 
	threshold(gray, binary, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	imwrite( "binary.jpg", binary ); 

	imwrite( "thin.jpg", binary ); 

	return 0;
}

const char* recognize::identify(IplImage* img) {
	vector<Piece> result;

	/////////split////////
	Mat input(img, 0), output;
	vector<Rect> boundRect;
	//if( !filterChanales( input, output, 0, boundRect ) ) 
	//	if( !filterChanales( input, output, 1, boundRect ) ) 
	//		if( !filterChanales( input, output, 2, boundRect ) ) 
	//			if( !filterErode( input, output, boundRect ) )
	//				return NULL;

	filterShape(input, output, boundRect);
	    
	for (unsigned int i = 0; i < boundRect.size(); ++i) {    
		//Scalar color = Scalar(0, 0, 0);/*bb outline*/      
		//rectangle(erosion, boundRect[i].tl(), boundRect[i].br(), color, 0.2, 8, 0);    

		CvRect roi = CvRect(boundRect[i]);    
		IplImage orig = output; 
		IplImage *src = cvCreateImage(cvSize(roi.width, roi.height), orig.depth, orig.nChannels);   
		cvSetImageROI(&orig, roi);    
		cvCopy(&orig, src);  
		//imwrite( "orig.jpg", output ); 
		cvResetImageROI(&orig);    

		//imwrite( "roi.jpg", Mat(src,0) ); 

		IplImage *one;    
		one = cvCreateImage(cvSize(mNormSize, mNormSize), IPL_DEPTH_8U, 1);    
		cvResize(src, one);    
		cvThreshold(one, one, 0, 255, CV_THRESH_BINARY);  

		//imwrite( "Threshold.jpg", Mat(one,0) ); 

		int ret = classify(one);       
		result.push_back( Piece(boundRect[i].tl().x,ret) );
		cvReleaseImage(&src);
		cvReleaseImage(&one);

	}
	
	////////sort////////////
	sort( result.begin(), result.end(), sortx );
	memset(mResult, 0, COUNT_DIGT+1);
	for( int i = 0; i < COUNT_DIGT; i ++ ) {
		mResult[i] = result[i].c + '0';
	}
	return mResult;
}

const char* recognize::test(const char* file) {
	//Mat img = imread(file);
	//Mat dst;
	//img.convertTo(dst, CV_8UC1);
	//IplImage ipl = IplImage(dst); 
	IplImage* img = cvLoadImage(file, -1);
	CvMat* mat = cvGetSubRect(img, 
		cvCreateMatHeader(img->width-2*BORDER, img->height-2*BORDER, CV_8UC1), 
			cvRect(BORDER,BORDER,(img->width-2*BORDER),(img->height-2*BORDER)) );
	//IplImage* ipl = cvCreateImage(cvGetSize(mat),img->depth,img->nChannels);
	Mat dst(mat);
	IplImage ipl = IplImage(dst); 
	return identify(&ipl);
}

const char* recognize::characterize(HWND hWnd, int x, int y, int w, int h) {
	memset(mResult, 0, COUNT_DIGT+1);
	IplImage* ipl = focus(hWnd, cvRect(x,y,w,h));

	identify(ipl);

	imwrite("focus.jpg",Mat(ipl));
	//cvReleaseImage(&ipl);
	return mResult;
}