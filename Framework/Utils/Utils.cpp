#include "Utils.h"
#include <string.h>
#include <Windows.h>
#pragma warning(disable:4996)
const wchar_t* DX::toWchar(const char* a)
{
    const size_t cSize = strlen(a) + 1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs(wc, a, cSize);

    return wc;
}
