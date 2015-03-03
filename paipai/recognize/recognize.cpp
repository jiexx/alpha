// 这是主 DLL 文件。

#include "stdafx.h"

#include "recognize.h"
#define BORDER 1
#define COUNT_DIGT 6

recognize::recognize(){
	K = 1;
	mNormSize = 32;
	mKNN = NULL;
	mNumOfCls = 10;
	mNumOfSamples = 1;
	mTrainData = cvCreateMat( mNumOfSamples*mNumOfCls, mNormSize*mNormSize, CV_32FC1 );// rows, col, type
	mTrainClasses = cvCreateMat( mNumOfSamples*mNumOfCls, 1, CV_32FC1 );
}
recognize::~recognize(){
	if( mArrImg ) {
		delete[] mArrImg;
	}
	if( mKNN )
		delete mKNN;
}
void recognize::load( const char imageslist[][8], int size ){
	if( size != mNumOfCls*mNumOfSamples ) 
		return;
	mArrImg = (IplImage**)new IplImage[mNumOfCls*mNumOfSamples];
	for(int i = 0; i < mNumOfCls; i++){
		for(int j = 0; j< mNumOfSamples; j++){
			mArrImg[i*mNumOfSamples + j] = cvLoadImage(imageslist[i*mNumOfSamples + j],0);
		}
	}
}
IplImage* recognize::preprocessing(IplImage* src){
	IplImage* img = cvCreateImage( cvSize( mNormSize, mNormSize ), IPL_DEPTH_32F, 1 );
	//convert 8 bits image to 32 float image
	cvConvertScale(src, img, 1, 0);
	imwrite( "src.jpg", Mat(src) );
	imwrite( "img.jpg", Mat(img) );
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
	imwrite( "mTrainData.jpg", Mat(mTrainData) );
	imwrite( "mTrainClasses.jpg", Mat(mTrainClasses) );
	//if( mKNN ) {
	//	mKNN->save();
	//}
}
int recognize::classify(IplImage* img){
	CvMat dat;
	IplImage* p;
	CvMat* nearest=cvCreateMat(1,K,CV_32FC1);

	p = preprocessing(img);
	imwrite( "p.jpg", Mat(p) );

	cvGetSubRect( p, &dat, cvRect(0,0,mNormSize,mNormSize) );
	CvMat row_header, *row1;
	row1 = cvReshape( &dat, &row_header, 0, 1 );
	imwrite( "row1.jpg", Mat(row1) );

	float result = mKNN->find_nearest(row1,K,0,0,nearest,0);
	imwrite( "nearest.jpg", Mat(nearest) );
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
	int heigh=bi->bmiHeader.biHeight;

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
	bi.biHeight = bitmap.bmHeight;  
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

	return result;  
} 

IplImage* focus(HWND hWnd, const CvRect& rect) {
	IplImage* ipli = capture(hWnd);
	CvMat* mat = cvGetSubRect(ipli, cvCreateMatHeader(rect.height, rect.width, CV_8UC1), rect);
	IplImage *header = cvCreateImageHeader(cvSize(rect.width, rect.height), ipli->depth, ipli->nChannels);
	return cvGetImage(mat, header);
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

int filterChanales( const Mat& input, Mat& output, int bgr, vector<vector<Point>>& contours ) {
	Mat ccc, gray, binary;
	vector<Mat> bgra(input.channels());
	split(input, bgra);
	bgra[0] = bgra[bgr];
	bgra[1] = bgra[bgr];
	bgra[2] = bgra[bgr];
	merge(bgra, ccc);

	cvtColor(ccc, gray, CV_BGR2GRAY); 
	threshold(gray, binary, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

	binary.copyTo(output);
   
	findContours(binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	if( contours.size() == COUNT_DIGT ) {
		return 1;
	}
	contours.clear();
	return 0;
}

vector<Piece> recognize::identify(IplImage* img) {
	vector<Piece> result;

	//cvSmooth(img,img,CV_GAUSSIAN,3,0,0,0);
	//cvThreshold(img, img, 0, 255, CV_THRESH_BINARY);  
	//imwrite( "Smooth.jpg", Mat(img,0) ); 
	

	//Mat input(img, 0), bbb, gray, binary, erosion, dilation, er, ec;
	//vector<Mat> bgra(input.channels());
	//split(input, bgra);
	//bgra[0] = bgra[0];
	//bgra[1] = bgra[0];
	//bgra[2] = bgra[0];
	//merge(bgra, bbb);
	//imwrite( "bbb.jpg", bbb ); 
	//cvtColor(bbb, gray, CV_BGR2GRAY); 
	//imwrite( "gray.jpg", gray ); 
	//threshold(gray, binary, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);/*binary image*/
	//imwrite( "binary.jpg", binary ); 
	

	//er = getStructuringElement(MORPH_RECT/*erosion_type*/, Size(2,2), Point(-1,-1) ); 
	//erode(binary, erosion, er);
	//close = pickoutLines(&IplImage(erosion));

	//Size dsize = Size(erosion.cols*4,erosion.rows*4);
	//resize(erosion, close, dsize, CV_INTER_LINEAR);
	//dilate(erosion, dilation, ec, Point(-1,-1), 2);
	////threshold(dilation,dilation,0,255,THRESH_BINARY_INV);
	//dilation = close + erosion;

	//imwrite( "dilation.jpg", dilation );
	////markers = erosion + dilation;
	//erode(dilation, erosion, er, Point(-1,-1), 4);
	//imwrite( "dilation2.jpg", erosion );
	/////////split////////
	Mat input(img, 0), output;
	vector< vector< Point> > contours;
	if( !filterChanales( input, output, 0, contours ) ) 
		if( !filterChanales( input, output, 1, contours ) ) 
			if( !filterChanales( input, output, 2, contours ) ) ;


	vector<Rect> boundRect(contours.size());    
	for (unsigned int i = 0; i < contours.size(); ++i) {    
		//Scalar color = Scalar(0, 0, 0);/*bb outline*/    
		boundRect[i] = boundingRect(Mat(contours[i]));    
		//rectangle(erosion, boundRect[i].tl(), boundRect[i].br(), color, 0.2, 8, 0);    

		CvRect roi = CvRect(boundRect[i]);    
		IplImage orig = output; 
		IplImage *src = cvCreateImage(cvSize(roi.width, roi.height), orig.depth, orig.nChannels);   
		cvSetImageROI(&orig, roi);    
		cvCopy(&orig, src);    
		cvResetImageROI(&orig);    

		imwrite( "roi.jpg", Mat(src,0) ); 

		IplImage *one;    
		one = cvCreateImage(cvSize(mNormSize, mNormSize), IPL_DEPTH_8U, 1);    
		cvResize(src, one);    
		cvThreshold(one, one, 0, 255, CV_THRESH_BINARY);  

		imwrite( "Threshold.jpg", Mat(one,0) ); 

		int ret = classify(one);       
		result.push_back( Piece(boundRect[i].tl().x,ret) );
	}

	////////sort////////////
	sort( result.begin(), result.end(), sortx );
	return result;
}

vector<Piece> recognize::test(const char* file) {
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

const char* recognize::characterize(HWND hWnd, const CvRect& rect) {
	memset(mResult, 0, RESULT_SIZE);
	IplImage* ipl = focus(hWnd, rect);
	vector<Piece> list = identify(ipl);
	for(unsigned int i = 0; i < list.size(); i ++ ) {
		Piece r = list.at(i);
		mResult[i] = r.c;
	}
	return mResult;
}