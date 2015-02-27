// recognize.h

#pragma once
#include "opencv\cv.h"
#include "opencv\opencv2\ml\ml.hpp"

using namespace System;
using namespace cv;

namespace recognize {

	public ref class R
	{
		// TODO: 在此处添加此类的方法。
		public CvKNearest generate( const char[][] images );

	};
}
