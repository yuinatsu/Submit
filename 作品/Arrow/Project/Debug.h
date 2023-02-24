#pragma once
#include<string>
//#include<wstring>
class Debug
{
public:
	static void Out(const char* str);
	static void Out(const wchar_t* str);
	static void Out(const std::string& str);
	static void Out(const std::wstring& str);
	static void OutLine(const char* str);
	static void OutLine(const wchar_t* str);
	static void OutLine(const std::string& str);
	static void OutLine(const std::wstring& str);
	static void Out(int val);
	static void Out(float val);
};

extern int test;//テストのint(4バイト領域を食いつぶす)

