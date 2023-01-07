#pragma once
#include <DirectXMath.h>

namespace DX
{
    using Position = DirectX::XMFLOAT3;
    using Color = DirectX::XMFLOAT3;
    using Texcoord = DirectX::XMFLOAT2;
    using Normal = DirectX::XMFLOAT3;

    struct VertexColor
    {
        Position position;
        Color color;
    };
    
    struct VertexColorNormal
    {
        Position position;
        Color color;
        Normal normal;
    };

    // Vertex, Texture, Normals
    struct VertexTexture
    {
        VertexTexture() {}
        VertexTexture(float x, float y, float z, float u, float v, float xN, float yN, float zN)
            : position(x, y, z), texcoord(u, v), normal(xN, yN, zN) {}
        Position position;
        Texcoord texcoord;
        Normal normal;
    };

    struct Pixel
    {
        float r, g, b, a;
    };
}