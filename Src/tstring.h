#pragma once

#include <windows.h>
#include <sstream>
#include <string>
#include <assert.h>
#include <tchar.h>
#include <iomanip>
#include <vector>

namespace AC
{
	#ifdef _UNICODE
		typedef std::wstring tstring;
		typedef std::wostringstream tostringstream;
		#define AnsiTotstring(s) AC::AnsiToWideString(s, lstrlenA(s))
		#define WideTotstring(s) AC::tstring(s)
		#define tstringTostring(s) AC::WideToAnsiString(s.c_str(), s.length())
	#else
		typedef std::string tstring;
		typedef std::ostringstream tostringstream;
		#define AnsiTotstring(s) AC::tstring(s)
		#define WideTotstring(s) AC::WideToAnsiString(s, lstrlenW(s))
		#define tstringTostring(s) std::string(s)
	#endif

	inline std::wstring AnsiToWideString(const char* pStr, int len)
	{
		assert(pStr != NULL); 
		assert(len >= 0); 

		int nChars = MultiByteToWideChar(CP_ACP, 0, pStr, len, NULL, 0); 
		if (len == -1)
			nChars--; 
		if (nChars == 0)
			return L"";

		std::wstring buf ;
		buf.resize(nChars) ; 
		MultiByteToWideChar(CP_ACP, 0, pStr, len, 
			const_cast<wchar_t*>(buf.c_str()), nChars ) ; 

		return buf ;
	}

	inline std::string WideToAnsiString(const wchar_t* pStr , int len)
	{
		assert(pStr != NULL); 
		assert(len >= 0); 

		int nChars = WideCharToMultiByte(CP_ACP, 0, pStr, 
			len, NULL, 0, NULL, NULL) ; 
		if (len == -1)
			nChars--; 
		if (nChars == 0)
			return "";

		std::string buf ;
		buf.resize(nChars);
		WideCharToMultiByte(CP_ACP, 0, pStr, len, 
			  const_cast<char*>(buf.c_str()), nChars, NULL, NULL); 

		return buf ; 
	}

	inline tstring StrError(DWORD ErrorCode)
	{
		LPVOID FailReason;

		if (FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &FailReason, 0, NULL) == 0)
		{
			return _T("");
		}

		tstring FailReasonStr = (LPCTSTR)FailReason;
		LocalFree(FailReason);
		return FailReasonStr;
	}
}
