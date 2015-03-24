/**************************************************************************
 *
 * IP Host Demo
 * ....................................
 * Main log implementation
 * 
 *
 * ------------------------------------------------------------------------
 * (c) copyright 2011 Laurent Dupuis
 * ........................................................................
 * < This program is free software: you can redistribute it and/or modify
 * < it under the terms of the GNU General Public License as published by
 * < the Free Software Foundation, either version 3 of the License, or
 * < (at your option) any later version.
 * < 
 * < This program is distributed in the hope that it will be useful,
 * < but WITHOUT ANY WARRANTY; without even the implied warranty of
 * < MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * < GNU General Public License for more details.
 * < 
 * < You should have received a copy of the GNU General Public License
 * < along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ........................................................................
 *
 */
//.........................................
#include "stdafx.h"
#include "log.h"
//.........................................

#ifdef _DEBUG

using namespace debug;

//////////////////////////////////////////////////////////////////////////////
static class internalLogConsole
{
	CRITICAL_SECTION _textCs;
	HANDLE _hConsole;
public:
	internalLogConsole();

	void WriteLine(WORD tAttrLow,WORD tAttrHigh,const char *text,size_t len);

} logConsole;

internalLogConsole::internalLogConsole() : _hConsole(INVALID_HANDLE_VALUE)
{
	_hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	COORD szScreen = { 255,2048 };
	SetConsoleScreenBufferSize(_hConsole,szScreen);
	InitializeCriticalSection(&_textCs);
}

void internalLogConsole::WriteLine(WORD tAttrLow,WORD tAttrHigh,const char *text,size_t len)
{
	CriticalSection cs(_textCs);
	DWORD dummy;
	if(_hConsole==INVALID_HANDLE_VALUE) return;

	SetConsoleTextAttribute(_hConsole,tAttrLow);
	WriteFile(_hConsole,(LPCVOID)text,40,&dummy,NULL);
	SetConsoleTextAttribute(_hConsole,tAttrHigh);
	WriteFile(_hConsole,(LPCVOID)(text+40),(DWORD)(len-40),&dummy,NULL);
}


//////////////////////////////////////////////////////////////////////////////
/*static*/ const char log::ansi2oem[256] = {
'\x00','\x01','\x02','\x03','\x04','\x05','\x06','\x07','\x08',
'\x09','\x0A','\x0B','\x0C','\x0D','\x0E','\x0F','\x10','\x11',
'\x12','\x13','\x14','\x15','\x16','\x17','\x18','\x19','\x1A',
'\x1B','\x1C','\x1D','\x1E','\x1F','\x20','\x21','\x22','\x23',
'\x24','\x25','\x26','\x27','\x28','\x29','\x2A','\x2B','\x2C',
'\x2D','\x2E','\x2F','\x30','\x31','\x32','\x33','\x34','\x35',
'\x36','\x37','\x38','\x39','\x3A','\x3B','\x3C','\x3D','\x3E',
'\x3F','\x40','\x41','\x42','\x43','\x44','\x45','\x46','\x47',
'\x48','\x49','\x4A','\x4B','\x4C','\x4D','\x4E','\x4F','\x50',
'\x51','\x52','\x53','\x54','\x55','\x56','\x57','\x58','\x59',
'\x5A','\x5B','\x5C','\x5D','\x5E','\x5F','\x60','\x61','\x62',
'\x63','\x64','\x65','\x66','\x67','\x68','\x69','\x6A','\x6B',
'\x6C','\x6D','\x6E','\x6F','\x70','\x71','\x72','\x73','\x74',
'\x75','\x76','\x77','\x78','\x79','\x7A','\x7B','\x7C','\x7D',
'\x7E','\x7F','\x5F','\x5F','\x27','\x9F','\x22','\x2E','\xC5',
'\xCE','\x5E','\x25','\x53','\x3C','\x4F','\x5F','\x5A','\x5F',
'\x5F','\x27','\x27','\x22','\x22','\x07','\x2D','\x2D','\x7E',
'\x54','\x73','\x3E','\x6F','\x5F','\x7A','\x59','\xFF','\xAD',
'\xBD','\x9C','\xCF','\xBE','\xDD','\xF5','\xF9','\xB8','\xA6',
'\xAE','\xAA','\xF0','\xA9','\xEE','\xF8','\xF1','\xFD','\xFC',
'\xEF','\xE6','\xF4','\xFA','\xF7','\xFB','\xA7','\xAF','\xAC',
'\xAB','\xF3','\xA8','\xB7','\xB5','\xB6','\xC7','\x8E','\x8F',
'\x92','\x80','\xD4','\x90','\xD2','\xD3','\xDE','\xD6','\xD7',
'\xD8','\xD1','\xA5','\xE3','\xE0','\xE2','\xE5','\x99','\x9E',
'\x9D','\xEB','\xE9','\xEA','\x9A','\xED','\xE8','\xE1','\x85',
'\xA0','\x83','\xC6','\x84','\x86','\x91','\x87','\x8A','\x82',
'\x88','\x89','\x8D','\xA1','\x8C','\x8B','\xD0','\xA4','\x95',
'\xA2','\x93','\xE4','\x94','\xF6','\x9B','\x97','\xA3','\x96',
'\x81','\xEC','\xE7','\xCC'
};

//////////////////////////////////////////////////////////////////////////////
static void debugMakeTimestamp(std::string &ret,const char *file,int line)
{
	char locBuf[50];
	const char *p;
	SYSTEMTIME t;
	size_t cl=strlen(file);
	if (cl>24)	 { p=file+(cl-23); }
	else		 { p=file;		   }
	GetLocalTime(&t);

	for(int u=0;u<24;u++)
	{
		if(*p)	{ locBuf[u]=*p; p++; }
		else	{ locBuf[u]=' '; }
	}
	locBuf[23]='|';
	for(int u=0;u<5;u++)
	{
		locBuf[28-u]= line%10 + '0';
		line /= 10;
	}
	locBuf[29]='|';
	locBuf[30]=(t.wHour/10)+'0';
	locBuf[31]=(t.wHour%10)+'0';
	locBuf[32]=':';
	locBuf[33]=(t.wMinute/10)+'0';
	locBuf[34]=(t.wMinute%10)+'0';
	locBuf[35]=':';
	locBuf[36]=(t.wSecond/10)+'0';
	locBuf[37]=(t.wSecond%10)+'0';
	locBuf[38]='|';
	locBuf[39]=' ';
	locBuf[40]=0;

	ret.assign(locBuf,locBuf+40);
}

//////////////////////////////////////////////////////////////////////////////
log::~log() 
{
	try
	{
		std::string ret;
		const char *p = _format;
		const char *e = _format+strlen(_format)+1;
		size_t nbItems = _values.size();

		debugMakeTimestamp(ret,_file,_line);
		if(nbItems)
		{
			while(p!=e)
			{
				const char *b = p;
				while(p!=e && *p!='{') ++p;
				ret.append(b,p);
				if(p==e) break;

				// Here *p=='{'
				b=p;
				++p;
				if(p==e || *p=='{') 
				{
					ret.append(1,'{');++p;
					continue;
				}
				for(int i=0;i<2 && p!=e && isdigit(*p);++i,++p);
				if(p==e)	{ ret.append(b,p);break; }
				if(*p!='}')	{ ret.append(b,p);continue; }

				std::string sIndex(b+1,p);++p;
				size_t index = (size_t)atoi(sIndex.c_str());
				if(index>nbItems)
					ret.append(b,p);
				else
					ret.append( _values[index] );
			}
		}
		else ret.append(p,e);

		ret.append(1,'\n');
		OutputDebugStringA (ret.c_str());
		for(std::string::iterator i=ret.begin();i!=ret.end();i++) *i=ansi2oem[*i];
		logConsole.WriteLine(_tAttrLow,_tAttrHigh,ret.c_str(),ret.length());
	}
	catch(...){}
}

//////////////////////////////////////////////////////////////////////////////
debug::log& log::operator%(const logformatter &value)
{
	std::string toAdd;
	value.getValue(toAdd);
	_values.push_back( toAdd );
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
debug::log& log::operator%(const wchar_t *value) 
{ 
	USES_CONVERSION;
	_values.push_back(std::string( W2CA(value) ));
	return *this; 
}

//////////////////////////////////////////////////////////////////////////////
debug::log& log::operator%(const char *value)
{
	_values.push_back(std::string( value ));
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
debug::log& log::operator%(const std::wstring &value)
{
	USES_CONVERSION;
	_values.push_back(std::string( W2CA(value.c_str()) ));
	return *this; 
}

//////////////////////////////////////////////////////////////////////////////
debug::log& log::operator%(const std::string  &value)
{
	_values.push_back( std::string( value ) );
	return *this; 
}


//////////////////////////////////////////////////////////////////////////////
debug::log& log::operator%(const GUID &value)
{
	USES_CONVERSION;
	OLECHAR targGuid[90];
	StringFromGUID2(value,targGuid,90);
	_values.push_back(std::string( W2CA(targGuid) ));
	return *this; 
}

//////////////////////////////////////////////////////////////////////////////
debug::log& log::operator%(const _bstr_t &text)
{
	_values.push_back(std::string( text ));
	return *this; 
}

//////////////////////////////////////////////////////////////////////////////
debug::log& log::operator%(const _com_error &value)
{
	USES_CONVERSION;
	std::string text = "COM Exception{ HR=";

	{
		std::string toAdd;
		logHRESULT::FormatError( value.Error(), toAdd );
		text.append( toAdd );
	}
	
	text.append(" Source=[");text.append( W2CA(value.Source()) );
	text.append("] Decription=[");text.append( W2CA(value.Description()) );
	text.append("] }");

	_values.push_back(std::string( text ));

	return *this;
}


#endif