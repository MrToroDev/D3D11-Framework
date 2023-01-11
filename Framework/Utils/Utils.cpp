#include "Utils.h"
#include <string.h>
#include <Windows.h>
#include <locale>
#include <codecvt>

#pragma warning(disable:4996)
const wchar_t* DX::toWchar(const char* a)
{
    const size_t cSize = strlen(a) + 1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs(wc, a, cSize);

    return wc;
}

std::string DX::to_string(std::wstring input)
{
    using convert_t = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_t, wchar_t> strconverter;
    return strconverter.to_bytes(input);
}

std::wstring DX::to_wstring(std::string input)
{
    using convert_t = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_t, wchar_t> strconverter;
    return strconverter.from_bytes(input);
}
