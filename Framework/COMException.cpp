#include "COMException.h"
#include "Logger.h"

namespace DX
{
	const char* com_exception::what() const
    {
        static char s_str[1024] = {};
        sprintf_s(s_str, "COM_ERROR: \nFailure with HRESULT of 0x%08X\nFile: %s\nLine: %d",
            static_cast<unsigned int>(result),
            file.c_str(),
            line);
        LogError(s_str);
        return s_str;
    }

    int com_exception::GetErrorCode()
    {
        return static_cast<unsigned int>(result);
    }

    void ThrowIfFailed(HRESULT hr, const char* file, int line)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr, file, line);
        }
    }
}