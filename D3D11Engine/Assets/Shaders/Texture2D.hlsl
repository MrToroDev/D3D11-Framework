struct VSOutput
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1.0);
    output.normal = input.normal;
    output.texcoord = input.texcoord;
    return output;
}

Texture2D texture0 : register(t0);
SamplerState samplerState : register(s0);

cbuffer Test : register(b0)
{
    float TextureT;
    float3 padding;
}

float4 PSMain(VSOutput input) : SV_Target
{
    input.texcoord += TextureT;
    return texture0.Sample(samplerState, input.texcoord);
}