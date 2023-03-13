struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float3 color : COLOR0;
};

struct VSOutput
{
    float3 uv : TEXCOORD;
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
    VSOutput output = (VSOutput)0;
    output.pos = mul(float4(input.position, 1.0), worldPos);
    //output.pos = mul(float4(input.position, 1.0), view);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    output.pos.z = output.pos.w;
    output.uv = input.position;
    return output;
}

// Pixel Shader

struct PixelOutput
{
    float4 PixelColor : SV_Target0;
};

SamplerState st : register(s0);
TextureCube tex : register(t0);

PixelOutput PSMain(VSOutput input)
{
    PixelOutput output;

    //output.PixelColor = float4(1.0, 0.0, 0.0, 1.0);
    output.PixelColor = tex.Sample(st, input.uv);

    return output;
}