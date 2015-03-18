// tool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "recognize.h"
#include "loader.h"
#include "classifier.h"

int _tmain(int argc, _TCHAR* argv[])
{
	recognize reco(10,1);
	const char imglist[20][8] ={
		{"0.png"},{"0a.png"},
		{"1.png"},{"1a.png"},
		{"2.png"},{"2a.png"},
		{"3.png"},{"3a.png"},
		{"4.png"},{"4a.png"},
		{"5.png"},{"5a.png"},
		{"6.png"},{"6a.png"},
		{"7.png"},{"7a.png"},
		{"8.png"},{"8a.png"},
		{"9.png"},{"9a.png"},
	};
	const char imglist2[10][8] ={
		{"0.png"},
		{"1.png"},
		{"2.png"},
		{"3.png"},
		{"4.png"},
		{"5.png"},
		{"6.png"},
		{"7.png"},
		{"8.png"},
		{"9.png"},
	};
	loader lo;
	lo.addFont(L"华文彩云.ttf",L"华文彩云");
	lo.addFont(L"微软vista宋体.ttf",L"SimSun-ExtB");
	lo.addChar(L"0123456789");
	lo.handle();
	lo.getFontCharSet(0);
	lo.saveImage();

	classifier clsr;
	CvKNearest* knn = clsr.getKNN( lo );
	vector<char> str;
	if( knn ) {
		Mat m = imread("test1.png");
		Mat roi(m, Rect(1,1,m.cols-2,m.rows-2));
		str = clsr.findByKNN( *knn, roi );
		m = imread("test6.png");
		Mat roi2(m, Rect(1,1,m.cols-2,m.rows-2));
		str = clsr.findByKNN( *knn, roi2 );
	}

	reco.load2(imglist2,10,true);
	reco.prepare2();
	const char f[4][18] = {"test.png","test1.png","test2.png","test3.png"};
	const char* str1 = reco.test( f[0] );
	printf("%s \n",str1);
	const char* str2 = reco.test( f[1] );
	printf("%s \n",str2);
	const char* str3 = reco.test( f[2] );
	printf("%s \n",str3);
	const char* str4 = reco.test( f[3] );
	printf("%s \n",str4);
	return 0;
}

