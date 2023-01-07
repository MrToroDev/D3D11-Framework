#include "Include/Lights.hlsli"
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
    float3 WorldPosition : WORLD_POSITION;
    float3 ViewPos : VIEW_POS;
};

// Vertex Shader

cbuffer CameraData : register(b0)
{
    row_major matrix worldPos;
    matrix projection;
    matrix view;
    vector position;
};

cbuffer LightData : register(b1)
{
    float3 viewPos;
    float3 viewDir;
    float3 ambient;
    float specular;
    float shininess;
    
    float4x4 dum;
    float dum1;
};

VSOutput VS(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.normal = mul(float4(input.normal, 0.0), worldPos).xyz;
    output.normal = normalize(output.normal);
    output.pos = mul(float4(input.position, 1.0), (worldPos));
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    output.WorldPosition = normalize(mul(float4(input.position, 1.0f), worldPos));
    output.ViewPos = (viewPos);
    output.uv = input.uv;
    return output;
}

// Pixel Shader

struct PixelOutput
{
    float4 PixelColor : SV_Target0;
};

cbuffer CubeEdit : register(b0)
{
    float3 color;
    bool useTexture;
};

Texture2D diffuse_texture : register(t0);
Texture2D specular_texture : register(t1);
Texture2D normal_texture : register(t2);
SamplerState samplerState1 : register(s0);

PixelOutput PS(VSOutput input)
{
    PixelOutput output;
    
    Lights::Light light;
    light.WorldPosition = input.WorldPosition;
    light.Normal = input.normal;
    light.Position = viewPos;
    light.ViewPos = input.ViewPos;
    light.Ambient = ambient;
    light.Specular = specular;
    light.shininess = shininess;
    
    if (useTexture)
    {
        output.PixelColor = diffuse_texture.Sample(samplerState1, input.uv);
    }
    else
    {
        output.PixelColor = float4(color, 1.0f) * float4(Lights::PointLight_raw(light, float3(1, 1, 1)), 1.0);
    }
    
    return output;
}