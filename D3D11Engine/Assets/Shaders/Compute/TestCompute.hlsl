struct BufType
{
    float f, j;
};

StructuredBuffer<BufType> Buffer0 : register(t0);
StructuredBuffer<BufType> Buffer1 : register(t1);
RWStructuredBuffer<BufType> BufferOut : register(u0);

[numthreads(1024, 1, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
    BufferOut[DTid.x].j = Buffer0[DTid.x].j + Buffer1[DTid.x].j;
    BufferOut[DTid.x].f = Buffer0[DTid.x].f + Buffer1[DTid.x].f;
}