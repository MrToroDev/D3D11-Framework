#pragma once
#include <Windows.h>
#include <stdio.h>
#include <exception>
#include <string>

#define DX_CHECK(hr) DX::ThrowIfFailed(hr, __FILE__, __LINE__)

namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr, std::string _file, int _line) : result(hr), file(_file), line(_line){}

        const char* what() const override;
        int GetErrorCode();

    private:
        HRESULT result;
        std::string file;
        int line;
    };

    // Helper utility converts D3D API failures into exceptions.
    void ThrowIfFailed(HRESULT hr, const char* file, int line);
}