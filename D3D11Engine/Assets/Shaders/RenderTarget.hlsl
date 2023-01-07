#include "Include/ScreenEffects.hlsli"

struct VSInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VSOutput
{
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 pos : SV_Position;
};

// Vertex Shader
VSOutput VS_RenderTarget(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.uv = input.uv;
    output.normal = input.normal;
    output.pos = float4(input.position, 1.0f);
    return output;
}

// Pixel Shader
cbuffer HDRData : register(b0)
{
    bool hdr;
    float exposure;
    float gamma;
    bool mosaic;
    float mosaicStrong;
    float clock;
    float3 padding;
};

Texture2D renderTarget : register(t0);
SamplerState renderSampler : register(s0);

float4 PS_RenderTarget(VSOutput input) : SV_Target
{   
    float3 result;
    float4 hdrColor;
    
    if (mosaic)
    {
        hdrColor = renderTarget.Sample(renderSampler, ScreenEffects::PixelateFrame(input.uv, mosaicStrong));
    }
    else
    {
        hdrColor = renderTarget.Sample(renderSampler, input.uv);
    }
    
    if (hdr)
    {
        result = float3(1.0f, 1.0f, 1.0f) - exp(-hdrColor.xyz * exposure);
        result = pow(result, float3(1.0 / gamma, 1.0 / gamma, 1.0 / gamma));
    }
    else
    {
        result = pow(hdrColor.xyz, float3(1.0 / gamma, 1.0 / gamma, 1.0 / gamma));
    }
    
    return float4(result, 1.0f);
}