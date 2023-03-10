#include "Mesh.h"
#include "GraphicLoader.h"
#include "../Utils/Utils.h"
#include "../Memory.h"
#include "../COMException.h"
#include <filesystem>
#include <string>

void DX::Mesh::initShaders(ID3D11Device* dev, std::wstring p, std::wstring v, std::string vmain, std::string pmain)
{
    if (p.substr(p.find_last_of('.') + 1) == L"hlsl" && v.substr(p.find_last_of('.') + 1) == L"hlsl") {
        if (!CompileShaderFromFile(p.c_str(), pmain.c_str(), "ps_5_0", pixelShaderBlob)) {
            return;
        }

        if (!CompileShaderFromFile(v.c_str(), vmain.c_str(), "vs_5_0", vertexShaderBlob)) {
            return;
        }
    }
    else if (p.substr(p.find_last_of('.') + 1) == L"cso" && v.substr(p.find_last_of('.') + 1) == L"cso") {
        auto psData = DX::ReadFile(DX::to_string(p));

        if (!CompileShaderFromStream(psData, pmain.c_str(), "ps_5_0", pixelShaderBlob)) {
            return;
        }

        auto vsData = DX::ReadFile(DX::to_string(v));

        if (!CompileShaderFromStream(vsData, vmain.c_str(), "vs_5_0", vertexShaderBlob)) {
            return;
        }
    }
    else {
        MessageBoxA(NULL, "NO SHADER PASSED IN PARAMETER!", "Shading Compile Error", MB_ICONWARNING | MB_OK);
        return;
    }

    DX_CHECK(dev->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr,
        &pixelShader));

    DX_CHECK(dev->CreateVertexShader(
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        nullptr,
        &vertexShader));
}

void DX::Mesh::initBuffer(ID3D11Device* dev, std::vector<VertexTexture> vertex, std::vector<int> indices)
{
    D3D11_BUFFER_DESC bufferInfo = {};
    bufferInfo.ByteWidth = static_cast<UINT>(sizeof(VertexTexture) * vertex.size());
    bufferInfo.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
    bufferInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA resourceData = {};
    resourceData.pSysMem = vertex.data();
    DX_CHECK(dev->CreateBuffer(
        &bufferInfo,
        &resourceData,
        Vbuffer.GetAddressOf()));

    bufferInfo.ByteWidth = static_cast<UINT>(sizeof(int) * indices.size());
    bufferInfo.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    bufferInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
    resourceData.pSysMem = indices.data();
    DX_CHECK(dev->CreateBuffer(
        &bufferInfo,
        &resourceData,
        Ibuffer.GetAddressOf()
    ));
}

void DX::Mesh::initBuffer(ID3D11Device* dev, float* v, std::vector<int> indices)
{
    D3D11_BUFFER_DESC bufferInfo = {};
    bufferInfo.ByteWidth = 4 * sizeof(v);
    bufferInfo.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
    bufferInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA resourceData = {};
    resourceData.pSysMem = v;
    DX_CHECK(dev->CreateBuffer(
        &bufferInfo,
        &resourceData,
        Vbuffer.GetAddressOf()));

    bufferInfo.ByteWidth = static_cast<UINT>(sizeof(int) * indices.size());
    bufferInfo.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    bufferInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
    resourceData.pSysMem = indices.data();
    DX_CHECK(dev->CreateBuffer(
        &bufferInfo,
        &resourceData,
        Ibuffer.GetAddressOf()
    ));
}

void DX::Mesh::initInputLayout(ID3D11Device* dev)
{
    constexpr D3D11_INPUT_ELEMENT_DESC vertexInputLayoutInfo[] =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            offsetof(DX::VertexTexture, position),
            D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "TEXCOORD",
            0,
            DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,
            0,
            offsetof(DX::VertexTexture, texcoord),
            D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "NORMAL",
            0,
            DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            offsetof(DX::VertexTexture, normal),
            D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "COLOR",
            0,
            DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            offsetof(DX::VertexTexture, color),
            D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
    };

    DX_CHECK(dev->CreateInputLayout(
        vertexInputLayoutInfo,
        _countof(vertexInputLayoutInfo),
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        &inputLayout));

}

void DX::Mesh::initInputLayout_V(ID3D11Device* dev)
{
    constexpr D3D11_INPUT_ELEMENT_DESC vertexInputLayoutInfo[] =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            0,
            D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "NORMAL",
            0,
            DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "TEXCOORD",
            0,
            DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "COLOR",
            0,
            DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };

    DX_CHECK(dev->CreateInputLayout(
        vertexInputLayoutInfo,
        _countof(vertexInputLayoutInfo),
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        &inputLayout));
}

DX::Mesh::Mesh(ID3D11Device* dev, std::wstring ShaderFile, std::string psmain, std::string vsmain, std::vector<VertexTexture> v, std::vector<int> indices)
{
    this->initShaders(dev, ShaderFile, ShaderFile, vsmain, psmain);
    this->initInputLayout(dev);
    this->initBuffer(dev, v, indices);
}

DX::Mesh::Mesh(ID3D11Device* dev, std::wstring ShaderFile, std::string psmain, std::string vsmain, float* v, std::vector<int> indices)
{
    this->initShaders(dev, ShaderFile, ShaderFile, vsmain, psmain);
    this->initInputLayout_V(dev);
    this->initBuffer(dev, v, indices);
    isVertexRawFloat = true;
}

void DX::Mesh::SetObjectName(const char name[256])
{
    char n[256];
    sprintf_s(n, "%s_PixelShader", name);
    SetDebugObjectName(pixelShader.Get(), n);
    sprintf_s(n, "%s_VertexShader", name);
    SetDebugObjectName(vertexShader.Get(), n);
    sprintf_s(n, "%s_InputLayout", name);
    SetDebugObjectName(inputLayout.Get(), n);
    sprintf_s(n, "%s_VertexBuffer", name);
    SetDebugObjectName(Vbuffer.Get(), n);
    sprintf_s(n, "%s_IndexBuffer", name);
    SetDebugObjectName(Ibuffer.Get(), n);
}

DX::Mesh::~Mesh()
{
    Memory::Destroy(this->pixelShader);
    Memory::Destroy(this->pixelShaderBlob);
    Memory::Destroy(this->vertexShader);
    Memory::Destroy(this->vertexShaderBlob);
    Memory::Destroy(this->inputLayout);
    Memory::Destroy(this->Vbuffer);
    Memory::Destroy(this->Ibuffer);
}

void DX::Mesh::Draw(ID3D11DeviceContext* devcon, int vertCount, PrimitiveMode mode)
{
    UINT vertexOffset = 0;
    UINT vertexStride;
    if (!isVertexRawFloat) vertexStride = sizeof(DX::VertexTexture);
    else vertexStride = 11 * 4; // stride = (vertex_size + uv_size + normal_size + color_size) * float_byte_size;

    switch (mode)
    {
    case PrimitiveMode::Triangle:
        devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        break;
    case PrimitiveMode::Line:
        devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        break;
    case PrimitiveMode::Point:
        devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
        break;
    }

    devcon->IASetInputLayout(inputLayout.Get());
    devcon->IASetVertexBuffers(
        0,
        1,
        Vbuffer.GetAddressOf(),
        &vertexStride,
        &vertexOffset);
    devcon->IASetIndexBuffer(
        Ibuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, vertexOffset
    );
    devcon->VSSetShader(
        vertexShader.Get(),
        nullptr,
        0);
    devcon->PSSetShader(
        pixelShader.Get(),
        nullptr,
        0);
    devcon->DrawIndexed(vertCount, 0, 0);
}
