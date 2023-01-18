struct VSInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 color : COLOR;
};

struct PSInput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 color : COLOR;
};

cbuffer MVPData : register(b0)
{
    matrix projection;
    matrix view;
    matrix worldPos;
};

PSInput VSMain(VSInput input)
{
    PSInput output = (PSInput) 0;

    output.pos = mul(float4(input.position, 1.0), worldPos);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    output.normal = input.normal;
    output.uv = input.uv;
    output.color = input.color;
    
    return output;
}

float4 PSMain(PSInput input) : SV_Target
{
    return float4(input.color, 1);
}