#include <Windows.h>
#include <cassert>
#include "Utility.h"

std::wstring Utility::StringToWidthString(const std::string& str)
{
	//?Å˜???????Åh??Åg???
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.data(), -1, nullptr, 0);

	// string??wchar_tÅh?
	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.data(), -1, &wstr[0], num1);
	assert(num1 == num2);

	return wstr.substr(0, wstr.length() - 1);
}

std::string Utility::WidthStringToString(const std::wstring& wstr)
{
	auto num1 = WideCharToMultiByte(CP_ACP, 0U, wstr.data(), -1, nullptr, 0, nullptr, nullptr);
	std::string str;
	str.resize(num1);
	auto num2 = WideCharToMultiByte(CP_ACP, 0U, wstr.data(), -1, &str[0], num1, nullptr, nullptr);
	return str.substr(0, str.length() - 1);
}

std::string Utility::ReadString(std::ifstream& file)
{
	int num{ 0 };
	std::string str;
	file.read(reinterpret_cast<char*>(&num), sizeof(num));
	str.resize(num);
	file.read(str.data(), sizeof(str[0]) * num);
	return str;
}
