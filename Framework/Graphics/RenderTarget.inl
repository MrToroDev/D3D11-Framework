#include "../COMException.h"
#include "RenderTarget.h"

template<DX::RenderType T>
void DX::RenderTarget<T>::CreateTexture(Microsoft::WRL::ComPtr<ID3D11Device> dev, int width, int height, DXGI_FORMAT textureFormat)
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

template<DX::RenderType T>
void DX::RenderTarget<T>::CreateDepthStencilBuffer(Microsoft::WRL::ComPtr<ID3D11Device> dev, int width, int height)
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

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.StencilEnable = false;
    depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    D3D11_DEPTH_STENCILOP_DESC sD = {
        D3D11_STENCIL_OP_KEEP,
        D3D11_STENCIL_OP_KEEP,
        D3D11_STENCIL_OP_KEEP,
        D3D11_COMPARISON_ALWAYS
    };
    depthStencilDesc.FrontFace = sD;
    depthStencilDesc.BackFace = sD;

    DX_CHECK(dev->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf()));
}

template<DX::RenderType T>
DX::RenderTarget<T>::RenderTarget(Microsoft::WRL::ComPtr<ID3D11Device> dev, Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon, int width, int height, std::wstring shader, DXGI_FORMAT tFormat)
{
    std::vector<VertexTexture> v = {
        VertexTexture(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
        VertexTexture( 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
        VertexTexture(-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
        VertexTexture( 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
    };
    std::vector<int> i = {
        0, 1, 2,
        2, 3, 1
    };
    textureFormat = tFormat;
    if (T == DX::RenderType::RenderTarget) {
        targetQuad = new Mesh(dev, shader, "PS_RenderTarget", "VS_RenderTarget", v, i);
    }
    else {
        targetQuad = new Mesh(dev, shader, "PS_RenderTarget_Depth", "VS_RenderTarget", v, i);
    }
    renderData.Initialize(dev.Get());
    textureType = T;
    if (T == DX::RenderType::RenderTarget) CreateTexture(dev, width, height, textureFormat);
    CreateDepthStencilBuffer(dev, width, height);

    D3D11_RASTERIZER_DESC rsDesc = {};
    rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
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

    dev.CopyTo(devRef.GetAddressOf());
    devcon.CopyTo(devconRef.GetAddressOf());
}

template<DX::RenderType T>
DX::RenderTarget<T>::~RenderTarget()
{
    if (T == DX::RenderType::RenderTarget)backbuffer.Reset();
    if (T == DX::RenderType::RenderTarget)backbufferSRV.Reset();
    if (T == DX::RenderType::RenderTarget)backbufferTexture.Reset();
    depthStencilBuffer.Reset();
    depthStencilState.Reset();
    depthStencilView.Reset();
    if (T == DX::RenderType::RenderTarget)textureST.Reset();
    delete targetQuad;
    devconRef.Reset();
    devRef.Reset();
}

template<DX::RenderType T>
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DX::RenderTarget<T>::GetTextureSRV()
{
    if (T == DX::RenderType::RenderTarget) {
        return backbufferSRV;
    }
    /*else {
        return depthStencilBuffer;
    }*/
}

template<DX::RenderType T>
void DX::RenderTarget<T>::SetConstantBufferData(bool hdr, float hdrExposure, float gamma, bool mosaic, float mosaicStrenght, float clock)
{
    renderData.data.hdr = hdr;
    renderData.data.exposure = hdrExposure;
    renderData.data.gamma = gamma;
    renderData.data.mosaic = mosaic;
    renderData.data.mosaicStrong = mosaicStrenght;
    renderData.data.clock = clock;
    renderData.ApplyChanges(devconRef.Get());
}

template<DX::RenderType T>
void DX::RenderTarget<T>::Clear(float color[4])
{
    if (T == DX::RenderType::RenderTarget) devconRef->ClearRenderTargetView(backbuffer.Get(), color);
    devconRef->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

template<DX::RenderType T>
void DX::RenderTarget<T>::SetRenderTarget(int width, int height)
{
    if (T == DX::RenderType::RenderTarget)backbuffer.Reset();
    if (T == DX::RenderType::RenderTarget)backbufferTexture.Reset();
    if (T == DX::RenderType::RenderTarget)backbufferSRV.Reset();
    if (T == DX::RenderType::RenderTarget)textureST.Reset();
    depthStencilBuffer.Reset();
    depthStencilState.Reset();
    depthStencilView.Reset();

    if (T == DX::RenderType::RenderTarget)CreateTexture(devRef, width, height, textureFormat);
    CreateDepthStencilBuffer(devRef, width, height);

    devconRef->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
    if (T == DX::RenderType::RenderTarget) {
        devconRef->OMSetRenderTargets( // Set Render Target
            1,
            backbuffer.GetAddressOf(),
            depthStencilView.Get());
        devconRef->RSSetState(rasterizerState.Get());
    }
}

template<DX::RenderType T>
inline void DX::RenderTarget<T>::SetBlendState(float blendFactor[4])
{
    devconRef->OMSetBlendState(blendState.Get(), blendFactor, 0xffffffff);
}

template<DX::RenderType T>
inline void DX::RenderTarget<T>::UnBoundBlendState()
{
    devconRef->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

template<DX::RenderType T>
inline void DX::RenderTarget<T>::UnBoundTarget()
{
    if (T == DX::RenderType::RenderTarget) devconRef->OMSetRenderTargets(0, nullptr, nullptr);
    devconRef->OMSetDepthStencilState(nullptr, 0);
}

template<DX::RenderType T>
void DX::RenderTarget<T>::Draw()
{
    targetQuad->prepareDraw(devconRef);
    devconRef->PSSetShaderResources(0, 1, backbufferSRV.GetAddressOf());
    devconRef->PSSetSamplers(0, 1, textureST.GetAddressOf());
    renderData.Bind(devconRef.Get(), 0, ConstantBuffer_BindType::PixelShader);
    targetQuad->Draw(devconRef, 6);
}
