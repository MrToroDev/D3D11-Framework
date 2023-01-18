#pragma once
#include <DirectXMath.h>
#include "../System/Math.h"

namespace DX
{
    using Position = DirectX::XMFLOAT3;
    using Color = DirectX::XMFLOAT3;
    using Texcoord = DirectX::XMFLOAT2;
    using Normal = DirectX::XMFLOAT3;

    // Vertex, Texture, Normals
    struct VertexTexture
    {
        VertexTexture() {}
        VertexTexture(float x, float y, float z, float u, float v, float xN, float yN, float zN)
            : position(x, y, z), texcoord(u, v), normal(xN, yN, zN) {}
        VertexTexture(float x, float y, float z, float u, float v, float xN, float yN, float zN, float cX, float cY, float cZ)
            : position(x, y, z), texcoord(u, v), normal(xN, yN, zN), color(cX, cY, cZ) {}
        VertexTexture(DirectX::XMFLOAT3 v, DirectX::XMFLOAT2 uv, DirectX::XMFLOAT3 normal)
            : position(v), texcoord(uv), normal(normal) {}
        VertexTexture(DirectX::XMFLOAT3 v, DirectX::XMFLOAT2 uv, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT3 Color)
            : position(v), texcoord(uv), normal(normal), color(Color) {}
        VertexTexture(hlsl::float3 pos, hlsl::float2 uv, hlsl::float3 normal)
            : position(pos.x, pos.y, pos.z), texcoord(uv.x, uv.y), normal(normal.x, normal.y, normal.z) {}
        VertexTexture(hlsl::float3 pos, hlsl::float2 uv, hlsl::float3 normal, hlsl::float3 Color)
            : position(pos.x, pos.y, pos.z), texcoord(uv.x, uv.y), normal(normal.x, normal.y, normal.z), color(Color.x, Color.y, Color.z) {}

        Position position;
        Texcoord texcoord;
        Normal normal;
        Color color = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
    };

    struct Pixel
    {
        float r, g, b, a;
    };
}