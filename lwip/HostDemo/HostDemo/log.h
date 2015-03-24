/**************************************************************************
 *
 * IP Host Demo
 * ....................................
 * LOG - Header
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
//=============================================================================
#pragma once

namespace debug {

//////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
class log;

class logformatter
{
	friend class log;

	virtual void getValue(std::string &toAppend) const = 0;
public:
	virtual ~logformatter() {}
};

class logHRESULT : public logformatter
{
	HRESULT _hr;

	void getValue(std::string &toAppend) const;
public:
	logHRESULT(HRESULT value) : _hr(value) {}

	static void FormatError(HRESULT hr,std::string &text);
};

class logWinError : public logformatter
{
	DWORD winErr;

	void getValue(std::string &toAppend) const;
public:
	logWinError(DWORD value) : winErr(value) {}
};

class log
{
	static const char ansi2oem[256];

	std::vector<std::string> _values;
	WORD _tAttrLow,_tAttrHigh;
	const char *_file,*_format;
	int _line;

	template<typename T>
	log& AppendData(T value)
	{
		std::stringstream s;
		s << value;
		_values.push_back( s.str() );
		return *this;
	}
public:
	log(WORD tAttrLow,WORD tAttrHigh,const char *file,int line,const char *text) 
		: _format(text),_file(file),_line(line),
		_tAttrLow(tAttrLow),_tAttrHigh(tAttrHigh) {}
	~log();


	log& operator%(int				value) { return AppendData(value); }
	log& operator%(char				value) { return AppendData(value); }
	log& operator%(long				value) { return AppendData(value); }
	log& operator%(short			value) { return AppendData(value); }
	log& operator%(__int64			value) { return AppendData(value); }
	log& operator%(unsigned			value) { return AppendData(value); }
	log& operator%(unsigned char	value) { return AppendData(value); }
	log& operator%(unsigned long	value) { return AppendData(value); }
	log& operator%(unsigned short	value) { return AppendData(value); }
	log& operator%(unsigned __int64	value) { return AppendData(value); }
	log& operator%(float			value) { return AppendData(value); }
	log& operator%(double			value) { return AppendData(value); }


	log& operator%(const wchar_t *value);
	log& operator%(const char *value);
	log& operator%(const std::wstring &value);
	log& operator%(const std::string  &value);
	log& operator%(const GUID &value);
	log& operator%(const _bstr_t &);
	log& operator%(const _com_error &value);

	log& operator%(const logformatter &value);
	log& operator%(const logWinError &value) { return operator%((const logformatter &)value); }
	log& operator%(const logHRESULT  &value) { return operator%((const logformatter &)value); }
};

#define DbgColor_Extern	FOREGROUND_GREEN, \
						FOREGROUND_GREEN|FOREGROUND_INTENSITY
#define DbgColor_Msg 	FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN, \
						FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY
#define DbgColor_Info	FOREGROUND_INTENSITY, \
						FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN
#define DbgColor_Notice FOREGROUND_RED|FOREGROUND_BLUE, \
						FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY
#define DbgColor_Warn  	FOREGROUND_RED|FOREGROUND_INTENSITY, \
						FOREGROUND_RED|FOREGROUND_GREEN
#define DbgColor_Error	FOREGROUND_RED|FOREGROUND_INTENSITY, \
						FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY

#define DBGINF(x)		debug::log(DbgColor_Info,   __FILE__,__LINE__,x)
#define DBGMSG(x) 		debug::log(DbgColor_Msg,    __FILE__,__LINE__,x)
#define DBGNOT(x)		debug::log(DbgColor_Notice, __FILE__,__LINE__,x)
#define DBGWRN(x)		debug::log(DbgColor_Warn,   __FILE__,__LINE__,x)
#define DBGERR(x)		debug::log(DbgColor_Error,  __FILE__,__LINE__,x)

#define DBGLASTERROR()	debug::logWinError(GetLastError())
#define DBGHRESULT(hr)	debug::logHRESULT (hr)
#define DBGWINERR(err)	debug::logWinError(err)

#else
// In release mode, the log calls are discarded

class logformatter {};
class logHRESULT : public logformatter
{
public:
	logHRESULT(HRESULT value) {}
};
class logWinError : public logformatter
{
public:
	logWinError(DWORD value) {}
};


class log
{
public:
	log(){}
	~log(){}

	inline log& operator%(const logformatter &value){return *this;}
	inline log& operator%(ULONG value)			{return *this;}
	inline log& operator%(int value)			{return *this;}
	inline log& operator%(double value)			{return *this;}
	inline log& operator%(long value)			{return *this;}
#ifndef _WIN64
	inline log& operator%(size_t value)			{return *this;}
#endif
	inline log& operator%(__int64 value)		{return *this;}
	inline log& operator%(unsigned __int64 value) {return *this;}
	inline log& operator%(const wchar_t *value)	{return *this;}
	inline log& operator%(const char *value)	{return *this;}
	inline log& operator%(const std::wstring &value) {return *this;}
	inline log& operator%(const std::string  &value) {return *this;}
	inline log& operator%(const GUID &value)	{return *this;}
	inline log& operator%(const _bstr_t &) {return *this;}
};

#define DBGINF(x)		debug::log()
#define DBGMSG(x) 		debug::log()
#define DBGNOT(x)		debug::log()
#define DBGWRN(x)		debug::log()
#define DBGERR(x)		debug::log()

#define DBGLASTERROR()	debug::logWinError(0)
#define DBGHRESULT(hr)	debug::logHRESULT(hr)
#define DBGWINERR(err)	debug::logWinError(err)

#endif

//=============================================================================
} //< namespace debug