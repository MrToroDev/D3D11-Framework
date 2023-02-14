#pragma once

#include <d3d11.h>
#include <string>

namespace DX
{
	const wchar_t* toWchar(const char* a);

	std::string to_string(std::wstring input);
	std::wstring to_wstring(std::string input);

	template<UINT TNameLength>
	inline void SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_z_ const char(&name)[TNameLength])
	{
#if defined(_DEBUG) || defined(PROFILE)
		resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#endif
	}
}