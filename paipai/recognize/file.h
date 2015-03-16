#include "windows.h"
#include "cv.h"
#include "highgui.h"
#include "ml/ml.hpp"
#include "map"
#include "vector"
#include "fstream"

using namespace std;
using namespace cv;

class binary{
public:
	inline binary(): mName(0){
	};
	inline binary( const char* name): mName(name){
	};
	inline void trunc(){
		ofstream of(mName, ios_base::binary|ios_base::trunc);
		of.close();
	}
	inline void save( vector<unsigned char> buf ) {
		ofstream of(mName, ios_base::binary|ios_base::app);
		char str[3] = {0};
		for ( vector<unsigned char>::iterator itr = buf.begin(); itr != buf.end(); itr++ ) {
			memset(str, 0, 3);
			sprintf(str, "%02x", *itr);
			of << str;
		}
		of << '\n';
		of.close();
	};
	inline vector<unsigned char>& read() {
		ifstream is(mName, ios_base::binary);
		unsigned int h, l;
		if ( is.is_open() ) {
			while ( !is.eof() )  {
				h = is.get();
				l = is.get();
				h = h < 'a' ?  h-'0' : h-'a'+10;
				l = l < 'a' ?  l-'0' : l-'a'+10;
				mOut.push_back( (h<<4)+l );
			}
		}
		return mOut;
	};
	inline vector<unsigned char>&  read( const char* in ) {
		int len = strlen(in);
		unsigned int h, l;
		for ( int i = 0; i < len; i ++ ) {
			h = in[i] < 'a' ?  in[i]-'0' : in[i]-'a'+10;
			i++;
			l = in[i] < 'a' ?  in[i]-'0' : in[i]-'a'+10;
			mOut.push_back( (h<<4)+l );
		}
		return mOut;
	};
private:
	const char* mName;
	vector<unsigned char> mOut;
};