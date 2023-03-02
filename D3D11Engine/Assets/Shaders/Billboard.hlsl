struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct PSInput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};

cbuffer CameraData : register(b0)
{
    matrix projection;
    matrix view;
    matrix worldPos;
}

PSInput VS(VSInput input)
{
    PSInput output;
    output.uv = input.texcoord;
    output.pos = mul(float4(input.position, 1.0), worldPos);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 PS(PSInput input) : SV_Target
{
    //return texture0.Sample(sampler0, input.uv);
    return float4(1, 0, 0, 1);
}