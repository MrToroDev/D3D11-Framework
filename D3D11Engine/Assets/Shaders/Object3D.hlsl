#include "Include/Common.hlsli"

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float3 color : COLOR0;
};

struct VSOutput
{
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 pos : SV_Position;
    float3 color : COLOR;
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
    output.color = input.color;
    return output;
}

// Pixel Shader

Texture2D t : register(t0);
SamplerState st : register(s0);

struct PixelOutput
{
    float4 PixelColor : SV_Target0;
};

PixelOutput PSMain(VSOutput input, uint pid : SV_PrimitiveID)
{
    PixelOutput output;
    
    //output.PixelColor = float4(1, 0.0, 0.0, 1.0);
    output.PixelColor = t.Sample(st, input.uv);
    
    return output;
}