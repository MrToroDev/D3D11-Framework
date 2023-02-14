#pragma once

#include <wrl.h>
#include <d3d11.h>

namespace DX {
	class SamplerState
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler = nullptr;

	public:
		SamplerState(Microsoft::WRL::ComPtr<ID3D11Device> dev,
					 D3D11_TEXTURE_ADDRESS_MODE u,
					 D3D11_TEXTURE_ADDRESS_MODE v,
					 D3D11_TEXTURE_ADDRESS_MODE w,
					 D3D11_FILTER filter);
		~SamplerState();

		void SetObjectName(const char name[256]);

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon, int count);
	};
}