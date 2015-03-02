// 这是主 DLL 文件。

#include "stdafx.h"

#include "recognize.h"
#define BORDER 1

recognize::recognize(){
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
	//if( mKNN ) {
	//	mKNN->save();
	//}
}
int recognize::classify(IplImage* img){
	CvMat dat;
	IplImage* p;
	CvMat* nearest=cvCreateMat(1,K,CV_32FC1);

	p = preprocessing(img);

	cvGetSubRect( p, &dat, cvRect(0,0,mNormSize,mNormSize) );
	CvMat row_header, *row1;
	row1 = cvReshape( &dat, &row_header, 0, 1 );

	float result = mKNN->find_nearest(row1,K,0,0,nearest,0);
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

vector<Piece> recognize::identify(IplImage* img) {
	vector<Piece> result;

	//cvSmooth(img,img,CV_GAUSSIAN,3,0,0,0);
	//cvThreshold(img, img, 0, 255, CV_THRESH_BINARY);  
	//imwrite( "Smooth.jpg", Mat(img,0) ); 

	Mat input(img, 0), gray, binary, erosion, dilation, element, markers;
	cvtColor(input, gray, CV_BGR2GRAY); 
	imwrite( "input.jpg", input ); 
	threshold(gray, binary, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);/*binary image*/

	element = getStructuringElement(MORPH_RECT/*erosion_type*/, Size(2,2), Point(-1,-1) ); 
	erode(binary, erosion, element);
	imwrite( "erode.jpg", erosion );  
	element = getStructuringElement(MORPH_RECT, Size(2,2), Point(-1,-1));
	dilate(erosion, dilation, element, Point(-1,-1), 6);
	//threshold(dilation,dilation,0,255,THRESH_BINARY_INV);
	imwrite( "dilation.jpg", dilation );
	//markers = erosion + dilation;
	//imwrite( "dilation2.jpg", markers );
	/////////split////////
	vector< vector< Point> > contours;    
	findContours(erosion, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);   


	vector<Rect> boundRect(contours.size());    
	for (unsigned int i = 0; i < contours.size(); ++i) {    
		//Scalar color = Scalar(0, 0, 0);/*bb outline*/    
		boundRect[i] = boundingRect(Mat(contours[i]));    
		//rectangle(erosion, boundRect[i].tl(), boundRect[i].br(), color, 0.2, 8, 0);    

		CvRect roi = CvRect(boundRect[i]);    
		IplImage orig = erosion;    
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