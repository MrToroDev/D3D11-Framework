#pragma once

#include <string>

namespace DX
{
	const wchar_t* toWchar(const char* a);

	std::string to_string(std::wstring input);
	std::wstring to_wstring(std::string input);
}