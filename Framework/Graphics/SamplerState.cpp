#include "SamplerState.h"
#include "../COMException.h"
#include "../Utils/Utils.h"
#include "../Memory.h"

DX::SamplerState::SamplerState(
	Microsoft::WRL::ComPtr<ID3D11Device> dev, 
	D3D11_TEXTURE_ADDRESS_MODE u, 
	D3D11_TEXTURE_ADDRESS_MODE v, 
	D3D11_TEXTURE_ADDRESS_MODE w,
	D3D11_FILTER filter)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = u;
	samplerDesc.AddressV = v;
	samplerDesc.AddressW = w;
	samplerDesc.Filter = filter;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DX_CHECK(dev->CreateSamplerState(&samplerDesc, this->sampler.GetAddressOf()));
}

DX::SamplerState::~SamplerState()
{
	Memory::Destroy(sampler);
}

void DX::SamplerState::SetObjectName(const char name[256])
{
	char n[256];
	sprintf_s(n, "%s_SamplerState", name);
	SetDebugObjectName(sampler.Get(), n);
}

void DX::SamplerState::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon, int count)
{
	devcon->PSSetSamplers(count, 1, sampler.GetAddressOf());
}

