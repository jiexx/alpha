// recognize.h

#pragma once
#include "opencv\cv.h"
#include "opencv\opencv2\ml\ml.hpp"

using namespace System;
using namespace cv;

namespace recognize {

	public ref class R
	{
		// TODO: �ڴ˴���Ӵ���ķ�����
		public CvKNearest generate( const char[][] images );

	};
}
