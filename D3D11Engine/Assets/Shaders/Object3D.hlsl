#include "Include/Common.hlsli"

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 pos : SV_Position;
};

// Vertex Shader

cbuffer MVPData : register(b0)
{
    matrix projection;
    matrix view;
    matrix worldPos;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.pos = mul(float4(input.position, 1.0), worldPos);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    output.uv = input.uv;
    return output;
}

// Pixel Shader

struct PixelOutput
{
    float4 PixelColor : SV_Target0;
};

PixelOutput PSMain(VSOutput input)
{
    PixelOutput output;
    
    output.PixelColor = float4(0, 1, 0, 1);
    
    return output;
}