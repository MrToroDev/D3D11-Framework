#include "COMException.h"
#include "Logger.h"
#include "Utils\Utils.h"
#include <comdef.h>

namespace DX
{
	const char* com_exception::what() const
    {
        _com_error errHr(result);
        std::string errMsg = DX::to_string(std::wstring(errHr.ErrorMessage()));
        static char s_str[1024] = {};
        sprintf_s(s_str, "COM_ERROR: \nFailure with HRESULT of 0x%08X\nFile: %s\nLine: %d\nMessage: %s\n",
            static_cast<unsigned int>(result),
            file.c_str(),
            line,
            errMsg.c_str());
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