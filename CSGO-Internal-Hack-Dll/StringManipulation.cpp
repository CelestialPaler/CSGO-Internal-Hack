#include "StringManipulation.h"

std::wstring Util::StringManipulation::StringToWstring(const std::string& _szString)
{
	std::wstring szWideString(_szString.begin(), _szString.end());
	return szWideString;
}

std::string Util::StringManipulation::WstringToString(const std::wstring& _szWstring)
{
	std::string szString(_szWstring.begin(), _szWstring.end());
	return szString;
}

std::vector<std::string> Util::StringManipulation::SplitString(const std::string& _str, const std::string& _delimiter)
{
	std::vector<std::string> Return;
	auto start = 0U;
	auto end = _str.find(_delimiter);
	while (end != std::string::npos)
	{
		Return.push_back(_str.substr(start, end - start));
		start = end + _delimiter.length();
		end = _str.find(_delimiter, start);
	}

	Return.push_back(_str.substr(start, end));
	return Return;
}
