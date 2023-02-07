#include "Graphic.h"
#include "../COMException.h"
#include "../Logger.h"
#include <string>
#include <sstream>
#include <assert.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DX;

void DX::Graphic::CreateDepthStencilBuffer(int width, int height)
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

    DX_CHECK(this->dev->CreateTexture2D(&dsDesc, nullptr, depthStencilBuffer.GetAddressOf()));
    DX_CHECK(this->dev->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, depthStencilView.GetAddressOf()));

    D3D11_DEPTH_STENCIL_DESC DSDesc;
    ZeroMemory(&DSDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    DSDesc.DepthEnable = true;
    DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    DSDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    DX_CHECK(this->dev->CreateDepthStencilState(&DSDesc, depthStencilState.GetAddressOf()));

}

void Graphic::CreateSwapChain()
{
    ID3D11Texture2D* backBufferTexture = nullptr;
    DX_CHECK(swapchain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        (void**)&backBufferTexture));

    assert(backBufferTexture);

    DX_CHECK(dev->CreateRenderTargetView(
        backBufferTexture,
        nullptr,
        this->backbuffer.GetAddressOf()));
    backBufferTexture->GetDesc(&backBufferDesc);
    backBufferTexture->Release();
}

void DX::Graphic::DebugLayer()
{
    {
        UINT64 messages = debugInfo->GetNumStoredMessages();
        for (int i = 0; i < messages; i++) {
            SIZE_T mSize = 0;
            debugInfo->GetMessageW(i, nullptr, &mSize);

            D3D11_MESSAGE* message = (D3D11_MESSAGE*)malloc(mSize);
            DX_CHECK(debugInfo->GetMessageW(i, message, &mSize));

            auto severity = message->Severity;
            auto category = message->Category;
            std::string strMessage = message->pDescription;

            std::stringstream ss;
            ss << "[D3D11] [";
            switch (severity)
            {
            case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_INFO:
                ss << "INFO]";
                break;
            case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_MESSAGE:
                ss << "MESSAGE]";
                break;
            case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_WARNING:
                ss << "WARNING]";
                break;
            case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_ERROR:
                ss << "ERROR]";
                break;
            case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_CORRUPTION:
                ss << "CORRUPTION]";
                break;
            }

            switch (category)
            {
            case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED:
                ss << " APPLICATION: ";
                break;
            case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_CLEANUP:
                ss << " CLEANUP: ";
                break;
            case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_COMPILATION:
                ss << " COMPILATION: ";
                break;
            case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_EXECUTION:
                ss << " EXECUTION: ";
                break;
            case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_INITIALIZATION:
                ss << " INITIALIZATION: ";
                break;
            case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_MISCELLANEOUS:
                ss << " MISCELANEOUS: ";
                break;
            case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_RESOURCE_MANIPULATION:
                ss << " RESOURCE MANIPULATION: ";
                break;
            case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_SHADER:
                ss << " SHADER: ";
                break;
            case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_STATE_CREATION:
                ss << " STATE CREATION: ";
                break;
            case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_STATE_GETTING:
                ss << " STATE GETTING: ";
                break;
            case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_STATE_SETTING:
                ss << " STATE SETTING: ";
                break;
            }

            ss << strMessage << " {Error ID: " << message->ID << "}";

            DX::LogDebugMessage(ss.str());
            free(message);
        }

        debugInfo->ClearStoredMessages();
    }
    {
        UINT64 messages = dxgiDebugInfo->GetNumStoredMessages(DXGI_DEBUG_DXGI);
        for (int i = 0; i < messages; i++) {
            SIZE_T mSize = 0;
            dxgiDebugInfo->GetMessageW(DXGI_DEBUG_DXGI, i, nullptr, &mSize);

            DXGI_INFO_QUEUE_MESSAGE* message = (DXGI_INFO_QUEUE_MESSAGE*)malloc(mSize);
            DX_CHECK(dxgiDebugInfo->GetMessageW(DXGI_DEBUG_DXGI, i, message, &mSize));

            auto severity = message->Severity;
            auto category = message->Category;
            std::string strMessage = message->pDescription;

            std::stringstream ss;
            ss << "[DXGI] [";
            switch (severity)
            {
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:
                ss << "INFO]";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:
                ss << "MESSAGE]";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
                ss << "WARNING]";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
                ss << "ERROR]";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:
                ss << "CORRUPTION]";
                break;
            }

            switch (category)
            {
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_CLEANUP:
                ss << " CLEANUP: ";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_COMPILATION:
                ss << " COMPILATION: ";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_EXECUTION:
                ss << " EXECUTION: ";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_INITIALIZATION:
                ss << " INITIALIZATION: ";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_MISCELLANEOUS:
                ss << " MISCELANEOUS: ";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_RESOURCE_MANIPULATION:
                ss << " RESOURCE MANIPULATION: ";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_SHADER:
                ss << " SHADER: ";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_CREATION:
                ss << " STATE CREATION: ";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_GETTING:
                ss << " STATE GETTING: ";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_SETTING:
                ss << " STATE SETTING: ";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_UNKNOWN:
                ss << " UNKNOWN: ";
                break;
            }

            ss << strMessage;
            DX::LogDebugMessage(ss.str());
            free(message);
        }

        dxgiDebugInfo->ClearStoredMessages(DXGI_DEBUG_DXGI);
    }
}

Graphic::Graphic(Window* win, bool fullscreen)
{
    winRef = win;

    constexpr D3D_FEATURE_LEVEL deviceFeatureLevel[] = {
        D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0

    };
    UINT deviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
    deviceFlags |= D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    DX_CHECK(D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        deviceFlags,
        deviceFeatureLevel,
        1,
        D3D11_SDK_VERSION,
        &dev,
        nullptr,
        &devcon));

    {
        IDXGIDevice1* dxgiDevice;
        HRESULT hResult = dev->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
        assert(SUCCEEDED(hResult));

        IDXGIAdapter* dxgiAdapter;
        hResult = dxgiDevice->GetAdapter(&dxgiAdapter);
        assert(SUCCEEDED(hResult));
        dxgiDevice->Release();

        DXGI_ADAPTER_DESC adapterDesc;
        dxgiAdapter->GetDesc(&adapterDesc);

        hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&_dxgifactory);
        assert(SUCCEEDED(hResult));
        dxgiAdapter->Release();
    }

    DXGI_SWAP_CHAIN_DESC1 swapChainDescriptor = {};
    swapChainDescriptor.Width = winRef->GetWidth();
    swapChainDescriptor.Height = winRef->GetHeight();
    swapChainDescriptor.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDescriptor.SampleDesc.Count = 1;
    swapChainDescriptor.SampleDesc.Quality = 0;
    swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescriptor.BufferCount = 2;
    swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDescriptor.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDescriptor.Scaling = DXGI_SCALING_STRETCH;
    swapChainDescriptor.Flags = {};

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDescriptor = {};
    swapChainFullscreenDescriptor.Windowed = true;

    DX_CHECK(_dxgifactory->CreateSwapChainForHwnd(
        dev.Get(),
        winRef->GetHandle(),
        &swapChainDescriptor,
        &swapChainFullscreenDescriptor,
        nullptr,
        &swapchain));

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
    
    // Query information
    D3D11_QUERY_DESC qDesc;
    qDesc.Query = D3D11_QUERY::D3D11_QUERY_TIMESTAMP;
    qDesc.MiscFlags = 0;
    DX_CHECK(dev->CreateQuery(&qDesc, startQuery.GetAddressOf()));
    DX_CHECK(dev->CreateQuery(&qDesc, endQuery.GetAddressOf()));
    qDesc.Query = D3D11_QUERY::D3D11_QUERY_TIMESTAMP_DISJOINT;
    DX_CHECK(dev->CreateQuery(&qDesc, disjoinQuery.GetAddressOf()));

    this->CreateDepthStencilBuffer(winRef->GetWidth(), winRef->GetHeight());
    CreateSwapChain();
    swapchain->SetFullscreenState(fullscreen, nullptr);

#if defined(_DEBUG)
    DX_CHECK(dev.As(&debuglayer));
    assert(debuglayer);
    DX_CHECK(debuglayer.As(&debugInfo));
    assert(debugInfo);

    DX_CHECK(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiDebugInfo.GetAddressOf())));
    assert(dxgiDebugInfo);
#endif
}

Graphic::~Graphic()
{
    if (backbuffer) backbuffer.Reset();
    if (swapchain) swapchain.Reset();
    if (depthStencilBuffer) depthStencilBuffer.Reset();
    if (depthStencilState) depthStencilState.Reset();
    if (depthStencilView) depthStencilView.Reset();
    if (rasterizerState) rasterizerState.Reset();
    if (_dxgifactory) _dxgifactory.Reset();
    if (endQuery) endQuery.Reset();
    if (startQuery) startQuery.Reset();
    if (disjoinQuery) disjoinQuery.Reset();
    if (devcon) devcon.Reset();
    if (dev) dev.Reset();
}

float DX::Graphic::GetGPUTime()
{
    D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoinData;
    devcon->GetData(disjoinQuery.Get(), &disjoinData, sizeof(D3D11_QUERY_DATA_TIMESTAMP_DISJOINT), 0);
    if (disjoinData.Disjoint) {
        return 0;
    }

    UINT64 begin, end;
    devcon->GetData(startQuery.Get(), &begin, sizeof(UINT64), 0);
    devcon->GetData(endQuery.Get(), &end, sizeof(UINT64), 0);

    float ms = float(end - begin) / float(disjoinData.Frequency) * 1000.0f;
    return ms;
}

void Graphic::UpdateSwapChain()
{
    devcon->Flush();
    backbuffer.Reset();
    depthStencilBuffer.Reset();
    depthStencilState.Reset();
    depthStencilView.Reset();

    if (winRef->GetWidth() != backBufferDesc.Width || winRef->GetHeight() != backBufferDesc.Height) {
        DX_CHECK(swapchain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
    }

    this->CreateDepthStencilBuffer(winRef->GetWidth(), winRef->GetHeight());
    this->CreateSwapChain();
}

void Graphic::Clear(float color[4])
{
    devcon->Begin(disjoinQuery.Get());
    devcon->End(startQuery.Get());

    devcon->ClearRenderTargetView(backbuffer.Get(), color);
    devcon->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Graphic::SetRenderTarget()
{
    devcon->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
    devcon->OMSetRenderTargets( // Set Render Target
        1,
        backbuffer.GetAddressOf(),
        depthStencilView.Get());
    devcon->RSSetState(rasterizerState.Get());
}

void Graphic::UnBoundRenderTarget()
{
    devcon->OMSetRenderTargets(0, nullptr, nullptr);
    devcon->OMSetDepthStencilState(nullptr, 0);
}

void Graphic::Present(int vsync)
{
    swapchain->Present(vsync, 0);
    
    devcon->End(endQuery.Get());
    devcon->End(disjoinQuery.Get());
    while (devcon->GetData(disjoinQuery.Get(), nullptr, 0, 0) == S_FALSE) { Sleep(1); }

    DebugLayer();

}

void Graphic::SetViewport(int width, int height, int x, int y)
{
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = static_cast<float>(x);
    viewport.TopLeftY = static_cast<float>(y);
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    devcon->RSSetViewports(1, &viewport); // Set ViewPort

    D3D11_RECT scRect = {};
    scRect.left = 0;
    scRect.right = width;
    scRect.top = 0;
    scRect.bottom = height;

    devcon->RSSetScissorRects(1, &scRect); // Set Scissor state
}

void DX::Graphic::ClearStateFlush()
{
    devcon->Flush();
    devcon->ClearState();
}

bool DX::Graphic::isFullscreen() const
{
    int fullscreen;
    DX_CHECK(swapchain->GetFullscreenState(&fullscreen, nullptr));
    return fullscreen;
}

void DX::Graphic::SetFullScreen(bool fullscreen)
{
    DX_CHECK(swapchain->SetFullscreenState(fullscreen, nullptr));
}

ComPtr<ID3D11Device> Graphic::getDevice()
{
    return dev;
}

ComPtr<ID3D11DeviceContext> Graphic::getDeviceContext()
{
    return devcon;
}

ComPtr<IDXGIFactory2> DX::Graphic::getFactory()
{
    return this->_dxgifactory;
}

ComPtr<ID3D11RenderTargetView> Graphic::getRenderTarget()
{
    return this->backbuffer;
}