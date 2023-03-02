#include "../COMException.h"
#include "../Utils/Utils.h"
#include "../Memory.h"
#include "RenderTarget.h"

void DX::RenderTarget::CreateTexture(ID3D11Device* dev, int width, int height, DXGI_FORMAT textureFormat)
{
    D3D11_TEXTURE2D_DESC dsDesc = {};
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    D3D11_SAMPLER_DESC samplerDesc = {};
    dsDesc.Width = width;
    dsDesc.Height = height;
    dsDesc.MipLevels = 1;
    dsDesc.ArraySize = 1;
    dsDesc.Format = textureFormat;
    dsDesc.SampleDesc.Count = 1;
    dsDesc.SampleDesc.Quality = 0;
    dsDesc.Usage = D3D11_USAGE_DEFAULT;
    dsDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    dsDesc.CPUAccessFlags = 0;
    dsDesc.MiscFlags = 0;
    DX_CHECK(dev->CreateTexture2D(&dsDesc, nullptr, backbufferTexture.GetAddressOf()));

    renderTargetViewDesc.Format = dsDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    DX_CHECK(dev->CreateRenderTargetView(
        backbufferTexture.Get(),
        &renderTargetViewDesc,
        this->backbuffer.GetAddressOf()));

    srvDesc.Format = dsDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    DX_CHECK(dev->CreateShaderResourceView(backbufferTexture.Get(), &srvDesc, backbufferSRV.GetAddressOf()));

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    DX_CHECK(dev->CreateSamplerState(&samplerDesc, textureST.GetAddressOf()));
}

void DX::RenderTarget::CreateDepthStencilBuffer(ID3D11Device* dev, int width, int height)
{
    D3D11_TEXTURE2D_DESC dsDesc = {};
    dsDesc.Width = width;
    dsDesc.Height = height;
    dsDesc.MipLevels = 1;
    dsDesc.ArraySize = 1;
    dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsDesc.SampleDesc.Count = 1;
    dsDesc.SampleDesc.Quality = 0;
    dsDesc.Usage = D3D11_USAGE_DEFAULT;
    dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    dsDesc.CPUAccessFlags = 0;
    dsDesc.MiscFlags = 0;

    DX_CHECK(dev->CreateTexture2D(&dsDesc, nullptr, depthStencilBuffer.GetAddressOf()));
    DX_CHECK(dev->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, depthStencilView.GetAddressOf()));
}

DX::RenderTarget::RenderTarget(ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height, std::wstring shader, DXGI_FORMAT tFormat)
{
    std::vector<VertexTexture> v = {
        VertexTexture(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
        VertexTexture(1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
        VertexTexture(-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
        VertexTexture(1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
    };
    std::vector<int> i = {
        0, 1, 2,
        2, 3, 1
    };
    textureFormat = tFormat;
    
    targetQuad = new Mesh(dev, shader, "PS_RenderTarget", "VS_RenderTarget", v, i);
    renderData = new ConstantBuffer<HDRData>(dev);
    CreateTexture(dev, width, height, textureFormat);
    CreateDepthStencilBuffer(dev, width, height);

    D3D11_RASTERIZER_DESC rsDesc = {};
    rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    rsDesc.FrontCounterClockwise = false;
    rsDesc.DepthBias = false;
    rsDesc.DepthBiasClamp = 0;
    rsDesc.SlopeScaledDepthBias = 0;
    rsDesc.DepthClipEnable = true;
    rsDesc.ScissorEnable = true;
    rsDesc.MultisampleEnable = false;
    rsDesc.AntialiasedLineEnable = false;
    DX_CHECK(dev->CreateRasterizerState(&rsDesc, rasterizerState.GetAddressOf()));

    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));

    D3D11_RENDER_TARGET_BLEND_DESC rtbd;
    ZeroMemory(&rtbd, sizeof(rtbd));

    rtbd.BlendEnable = true;
    rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rtbd.BlendOp = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.RenderTarget[0] = rtbd;

    DX_CHECK(dev->CreateBlendState(&blendDesc, blendState.GetAddressOf()));
}

DX::RenderTarget::~RenderTarget()
{
    Memory::Destroy(backbuffer);
    Memory::Destroy(backbufferSRV);
    Memory::Destroy(backbufferTexture);
    Memory::Destroy(depthStencilBuffer);
    Memory::Destroy(depthStencilView);
    Memory::Destroy(textureST);
    delete targetQuad;
    Memory::Destroy(rasterizerState);
    Memory::Destroy(blendState);
    Memory::Destroy(renderData);
}

void DX::RenderTarget::SetObjectName(const char name[256])
{
    char n[256];
    sprintf_s(n, "%s_Backbuffer", name);
    SetDebugObjectName(backbuffer.Get(), n);
    sprintf_s(n, "%s_BackbufferSRV", name);
    SetDebugObjectName(backbufferSRV.Get(), n);
    sprintf_s(n, "%s_BackbufferTexture", name);
    SetDebugObjectName(backbufferTexture.Get(), n);
    sprintf_s(n, "%s_DepthStencilBuffer", name);
    SetDebugObjectName(depthStencilBuffer.Get(), n);
    sprintf_s(n, "%s_DepthStencilView", name);
    SetDebugObjectName(depthStencilView.Get(), n);
    sprintf_s(n, "%s_Texture", name);
    SetDebugObjectName(textureST.Get(), n);
    sprintf_s(n, "%s_RasterizerState", name);
    SetDebugObjectName(rasterizerState.Get(), n);
    sprintf_s(n, "%s_BlendState", name);
    SetDebugObjectName(blendState.Get(), n);
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DX::RenderTarget::GetTextureSRV()
{
    return backbufferSRV;
}

void DX::RenderTarget::SetConstantBufferData(ID3D11DeviceContext* devcon, bool hdr, float hdrExposure, float gamma, bool mosaic, float mosaicStrenght, float clock)
{
    renderData->data.hdr = hdr;
    renderData->data.exposure = hdrExposure;
    renderData->data.gamma = gamma;
    renderData->data.mosaic = mosaic;
    renderData->data.mosaicStrong = mosaicStrenght;
    renderData->data.clock = clock;
    renderData->ApplyChanges(devcon);
}

void DX::RenderTarget::Clear(ID3D11DeviceContext* devcon, float color[4])
{
    devcon->ClearRenderTargetView(backbuffer.Get(), color);
    devcon->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX::RenderTarget::SetRenderTarget(ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height)
{
    backbuffer.Reset();
    backbufferTexture.Reset();
    backbufferSRV.Reset();
    textureST.Reset();
    depthStencilBuffer.Reset();
    depthStencilView.Reset();

    CreateTexture(dev, width, height, textureFormat);
    CreateDepthStencilBuffer(dev, width, height);

    devcon->OMSetRenderTargets( // Set Render Target
            1,
            backbuffer.GetAddressOf(),
            depthStencilView.Get());
    devcon->RSSetState(rasterizerState.Get());
    
}

void DX::RenderTarget::SetBlendState(ID3D11DeviceContext* devcon, float blendFactor[4])
{
    devcon->OMSetBlendState(blendState.Get(), blendFactor, 0xffffffff);
}

void DX::RenderTarget::UnBoundBlendState(ID3D11DeviceContext* devcon)
{
    devcon->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

void DX::RenderTarget::UnBoundTarget(ID3D11DeviceContext* devcon)
{
    devcon->OMSetRenderTargets(0, nullptr, nullptr);
    devcon->OMSetDepthStencilState(nullptr, 0);
}

void DX::RenderTarget::Draw(ID3D11DeviceContext* devcon)
{
    devcon->PSSetShaderResources(0, 1, backbufferSRV.GetAddressOf());
    devcon->PSSetSamplers(0, 1, textureST.GetAddressOf());
    renderData->Bind(devcon, 0, ConstantBuffer_BindType::PixelShader);
    targetQuad->Draw(devcon, 6);
}
