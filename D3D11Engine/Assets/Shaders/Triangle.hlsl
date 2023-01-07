struct VSInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct PSInput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

PSInput VSMain(VSInput input)
{
    PSInput output = (PSInput) 0;

    output.pos = float4(input.position, 1.0f);
    output.normal = input.normal;
    output.uv = input.uv;
    
    return output;
}

float4 PSMain(PSInput input) : SV_Target
{
    return float4(0, 1, 0, 1);
}