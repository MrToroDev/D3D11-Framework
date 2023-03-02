#pragma once

#include <d3d11.h>
#include "../Application.h"

class TestsState : public DX::States
{
public:
	TestsState(ApplicationDataRef data);

	void init() override;
	void update(float dt) override;
	void draw() override;
	void destroy() override;

private:
	ApplicationDataRef _data;

	// https://github.com/walbourn/directx-sdk-samples/blob/main/BasicCompute11/BasicCompute11.cpp

	struct BufType
	{
		float f, j;
	};
	BufType g_vBuf0[1024];
	BufType g_vBuf1[1024];

	DX::ComPtr<ID3D11ComputeShader> computeShader;
	DX::ComPtr < ID3D11Buffer> g_pBuf0 = nullptr;
	DX::ComPtr < ID3D11Buffer> g_pBuf1 = nullptr;
	DX::ComPtr < ID3D11Buffer> g_pBufResult = nullptr;

	DX::ComPtr < ID3D11ShaderResourceView> g_pBuf0SRV = nullptr;
	DX::ComPtr < ID3D11ShaderResourceView> g_pBuf1SRV = nullptr;
	DX::ComPtr < ID3D11UnorderedAccessView> g_pBufResultUAV = nullptr;

	HRESULT CreateStructuredBuffer(ID3D11Device* pDevice, UINT uElementSize, UINT uCount, void* pInitData, ID3D11Buffer** ppBufOut);
	HRESULT CreateBufferSRV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut);
	HRESULT CreateBufferUAV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUAVOut);
};