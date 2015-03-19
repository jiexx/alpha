#include "stdafx.h"
#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "map"
#include "vector"
#include "fstream"

using namespace std;
using namespace cv;

#ifndef __THINNER_H__
#define __THINNER_H__


/**
* Perform one thinning iteration.
* Normally you wouldn't call this function directly from your code.
*
* @param  im    Binary image with range = 0-1
* @param  iter  0=even, 1=odd
*/
inline void thinningIteration(Mat& im, int iter)
{
	Mat marker = Mat::zeros(im.size(), CV_8UC1);

	for (int i = 1; i < im.rows-1; i++)
	{
		for (int j = 1; j < im.cols-1; j++)
		{
			uchar p2 = im.at<uchar>(i-1, j);
			uchar p3 = im.at<uchar>(i-1, j+1);
			uchar p4 = im.at<uchar>(i, j+1);
			uchar p5 = im.at<uchar>(i+1, j+1);
			uchar p6 = im.at<uchar>(i+1, j);
			uchar p7 = im.at<uchar>(i+1, j-1);
			uchar p8 = im.at<uchar>(i, j-1);
			uchar p9 = im.at<uchar>(i-1, j-1);

			int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) + 
				(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) + 
				(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
				(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
			int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
			int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
			int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

			if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				marker.at<uchar>(i,j) = 1;
		}
	}

	im &= ~marker;
}

/**
* Function for thinning the given binary image
*
* @param  im  Binary image with range = 0-255
*/
inline void thinning(Mat& im) 
{
	im /= 255;

	Mat prev = Mat::zeros(im.size(), CV_8UC1);
	Mat diff;

	do {
		thinningIteration(im, 0);
		thinningIteration(im, 1);
		absdiff(im, prev, diff);
		im.copyTo(prev);
	} 
	while (countNonZero(diff) > 0);

	im *= 255;
}
/* in CV_8UC1
 * out 
 */
inline void adjustThinning(Mat& in, Mat& out, int width, int height ) { 
	
	if( in.channels() > 1 ) {
		cvtColor(in, in, CV_BGR2GRAY);
		threshold(in, in, 0, 255, CV_THRESH_BINARY);
	}
	resize( in, out, Size(width, height), 0, 0 );
	return;
	Mat ss = Mat::zeros(width-2, height-2, in.type());
	resize( in, ss, Size(width-2, height-2), 0, 0 );
	out = 0;
	Mat roi(out, Rect(1, 1, width-2, height-2));
	ss.copyTo(roi);

	thinning(out);
}

#endif