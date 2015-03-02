// tool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "recognize.h"

int _tmain(int argc, _TCHAR* argv[])
{
	recognize reco;
	const char imglist[10][8] ={
		{"0.png"},
		{"1.png"},
		{"2.png"},
		{"3.png"},
		{"4.png"},
		{"5.png"},
		{"6.png"},
		{"7.png"},
		{"8.png"},
		{"9.png"}
	};
	reco.load(imglist,10);
	reco.prepare();
	const char f[18] = {"test.png"};
	reco.test( f );
	return 0;
}

