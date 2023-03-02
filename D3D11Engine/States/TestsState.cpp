#include "TestsState.h"
#include <Graphics/GraphicLoader.h>

TestsState::TestsState(ApplicationDataRef data)
	: _data(data)
{
}

void TestsState::init()
{
	DX::ComPtr<ID3DBlob> blob;
	if (!DX::CompileShaderFromFile(L"Assets/Shaders/Compute/TestCompute.hlsl", "CSMain", "cs_5_0", blob)) {
		return;
	}

	_data->D3Dgraphic->getDevice()->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, computeShader.GetAddressOf());
	
	blob.Reset();

    for (int i = 0; i < 1024; ++i)
    {
        g_vBuf0[i].j = i;
        g_vBuf0[i].f = (float)i;

        g_vBuf1[i].j = i;
        g_vBuf1[i].f = (float)i;
    }

    CreateStructuredBuffer(_data->D3Dgraphic->getDevice().Get(), sizeof(BufType), 1024, &g_vBuf0[0], g_pBuf0.GetAddressOf());
    CreateStructuredBuffer(_data->D3Dgraphic->getDevice().Get(), sizeof(BufType), 1024, &g_vBuf1[0], g_pBuf1.GetAddressOf());
    CreateStructuredBuffer(_data->D3Dgraphic->getDevice().Get(), sizeof(BufType), 1024, nullptr, g_pBufResult.GetAddressOf());

    CreateBufferSRV(_data->D3Dgraphic->getDevice().Get(), g_pBuf0.Get(), g_pBuf0SRV.GetAddressOf());
    CreateBufferSRV(_data->D3Dgraphic->getDevice().Get(), g_pBuf1.Get(), g_pBuf1SRV.GetAddressOf());
    CreateBufferUAV(_data->D3Dgraphic->getDevice().Get(), g_pBufResult.Get(), g_pBufResultUAV.GetAddressOf());

    _data->D3Dgraphic->getDeviceContext()->CSSetShader(computeShader.Get(), nullptr, 0);
    ID3D11ShaderResourceView* aRViews[2] = { g_pBuf0SRV.Get(), g_pBuf1SRV.Get() };
    _data->D3Dgraphic->getDeviceContext()->CSSetShaderResources(0, 2, aRViews);
    _data->D3Dgraphic->getDeviceContext()->CSSetUnorderedAccessViews(0, 1, g_pBufResultUAV.GetAddressOf(), 0);

    _data->D3Dgraphic->getDeviceContext()->Dispatch(1, 1, 1);

    ID3D11Buffer* outputBuffer;
    D3D11_BUFFER_DESC outputDesc;
    outputDesc.ByteWidth = sizeof(BufType) * 1024;
    outputDesc.StructureByteStride = sizeof(BufType);
    outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    outputDesc.Usage = D3D11_USAGE_STAGING;
    outputDesc.BindFlags = 0;
    outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    _data->D3Dgraphic->getDevice()->CreateBuffer(&outputDesc, nullptr, &outputBuffer);

    _data->D3Dgraphic->getDeviceContext()->CopyResource(outputBuffer, g_pBufResult.Get());

    // Update particle system data with output from Compute Shader
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = _data->D3Dgraphic->getDeviceContext()->Map(outputBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);

    if (SUCCEEDED(hr))
    {
        BufType* dataView = reinterpret_cast<BufType*>(mappedResource.pData);

        // Update particle positions and velocities
        std::string dataString = "Some Data: ";
        dataString.append(std::to_string(dataView[2].f));
        DX::LogInfo(dataString);

        _data->D3Dgraphic->getDeviceContext()->Unmap(outputBuffer, 0);
    }

    outputBuffer->Release();
    outputBuffer = 0;
}

void TestsState::update(float dt)
{
    
}

void TestsState::draw()
{
    _data->debugConsole.Draw();
}

void TestsState::destroy()
{
	computeShader.Reset();
    g_pBuf0.Reset();
    g_pBuf1.Reset();
    g_pBufResult.Reset();
    g_pBufResultUAV.Reset();
    g_pBuf0SRV.Reset();
    g_pBuf1SRV.Reset();
}

HRESULT TestsState::CreateStructuredBuffer(ID3D11Device* pDevice, UINT uElementSize, UINT uCount, void* pInitData, ID3D11Buffer** ppBufOut)
{
    *ppBufOut = nullptr;

    D3D11_BUFFER_DESC desc = {};
    desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    desc.ByteWidth = uElementSize * uCount;
    desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    desc.StructureByteStride = uElementSize;

    if (pInitData)
    {
        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = pInitData;
        return pDevice->CreateBuffer(&desc, &InitData, ppBufOut);
    }
    else
        return pDevice->CreateBuffer(&desc, nullptr, ppBufOut);
}

HRESULT TestsState::CreateBufferSRV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut)
{
    D3D11_BUFFER_DESC descBuf = {};
    pBuffer->GetDesc(&descBuf);

    D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
    desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    desc.BufferEx.FirstElement = 0;

    if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
    {
        // This is a Raw Buffer

        desc.Format = DXGI_FORMAT_R32_TYPELESS;
        desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
        desc.BufferEx.NumElements = descBuf.ByteWidth / 4;
    }
    else
        if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
        {
            // This is a Structured Buffer

            desc.Format = DXGI_FORMAT_UNKNOWN;
            desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
        }
        else
        {
            return E_INVALIDARG;
        }

    return pDevice->CreateShaderResourceView(pBuffer, &desc, ppSRVOut);
}

HRESULT TestsState::CreateBufferUAV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUAVOut)
{
    D3D11_BUFFER_DESC descBuf = {};
    pBuffer->GetDesc(&descBuf);

    D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
    desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    desc.Buffer.FirstElement = 0;

    if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
    {
        // This is a Raw Buffer

        desc.Format = DXGI_FORMAT_R32_TYPELESS; // Format must be DXGI_FORMAT_R32_TYPELESS, when creating Raw Unordered Access View
        desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
        desc.Buffer.NumElements = descBuf.ByteWidth / 4;
    }
    else
        if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
        {
            // This is a Structured Buffer

            desc.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
            desc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
        }
        else
        {
            return E_INVALIDARG;
        }

    return pDevice->CreateUnorderedAccessView(pBuffer, &desc, ppUAVOut);
}