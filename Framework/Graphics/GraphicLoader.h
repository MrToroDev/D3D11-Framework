#pragma once

#include <wrl.h>
#include <string_view>
#include <string>
#include <d3d11_2.h>
#include <vector>
#include "Vertex.h"

namespace DX
{
    bool CompileShaderFromFile(
        const std::wstring_view fileName,
        const std::string_view entryPoint,
        const std::string_view profile,
        Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob);

    bool CompileShaderFromStream(
        const std::vector<unsigned char> fileData,
        const std::string_view entryPoint,
        const std::string_view profile,
        Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob);

    std::vector<unsigned char> ReadFile(std::string file);

    struct MeshData
    {
        std::vector<VertexTexture> vertices;
        std::vector<int> indices;
    };

    MeshData LoadMeshFile(std::string file);
}