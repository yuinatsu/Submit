#include "Debug.h"
#include <dxlib.h>
#include <sstream>


int test = 4;


void Debug::Out(const char* str)
{
	OutputDebugStringA(str);
}


void Debug::Out(const wchar_t* str)
{
	OutputDebugStringW(str);
}


void Debug::Out(const std::string& str)
{
	Out(str.c_str());
}


void Debug::Out(const std::wstring& str)
{
	Out(str.c_str());
}


void Debug::OutLine(const char* str)
{
	Out(str);
	Out("\n");
}


void Debug::OutLine(const wchar_t* str)
{
	Out(str);
	Out(L"\n");
}


void Debug::OutLine(const std::string& str)
{
	OutLine(str.c_str());
}


void Debug::OutLine(const std::wstring& str)
{
	OutLine(str.c_str());
}


void Debug::Out(int val)
{
	std::ostringstream oss;
	oss << val;
	Out(oss.str());
}


void Debug::Out(float val)
{
	std::ostringstream oss;
	oss << val;
	Out(oss.str());
}
