namespace ScreenEffects
{
    float2 PixelateFrame(float2 UV, float noise)
    {
        float2 p = UV;
        p.x -= fmod(p.x, 1.0 / noise);
        p.y -= fmod(p.y, 1.0 / noise);

        return p;
    }

    float3 StaticTVNoise(float2 texcoords, float _clock)
    {

        float2 uv = texcoords;
    
        float t = _clock + 123.; // tweak the start moment
        float ta = t * .654321;
        float tb = t * (ta * .123456);
    
        float c = frac(sin(uv.x * ta + uv.y * tb) * 5678.);
        float3 col = float3(c, c, c);

        return col;
    }
    
    float4 TVScreenEffect(float2 texcoords, Texture2D t, SamplerState sample)
    {
        float2 uv = texcoords.xy;
        uv = uv * 2.0 - 1.0;
        float2 offset = abs(uv.yx) / float2(3, 3);
        uv = uv + uv * offset * offset;
        uv = uv * 0.5 + 0.5;
        float4 baseColor = t.Sample(sample, uv);
        if (uv.x < 0.0 || uv.y < 0.0 || uv.x > 1.0 || uv.y > 1.0)
        {
            return float4(0.0, 0.0, 0.0, 1.0);
        }
        else
        {
            return baseColor;
        }
    }
    
    float4 BlurEffect(Texture2D t, SamplerState st, float2 uv, float noise)
    {
        float offset = 1.0 / noise;
        float2 offsets[9] =
        {
            float2(-offset, offset), // top-left
            float2(0.0, offset), // top-center
            float2(offset, offset), // top-right
            float2(-offset, 0.0), // center-left
            float2(0.0, 0.0), // center-center
            float2(offset, 0.0), // center-right
            float2(-offset, -offset), // bottom-left
            float2(0.0, -offset), // bottom-center
            float2(offset, -offset) // bottom-right    
        };
        
        float kernel[9] =
        {
            1.0 / 16, 2.0 / 16, 1.0 / 16,
            2.0 / 16, 4.0 / 16, 2.0 / 16,
            1.0 / 16, 2.0 / 16, 1.0 / 16
        };

        float3 samples[9];
        for (int i = 0; i < 9; i++)
        {
            samples[i] = t.Sample(st, uv + offsets[i]).rgb;
        }
        float3 col = float3(0.0f, 0.0f, 0.0f);
        for (int j = 0; j < 9; j++)
        {
            col += samples[j] * kernel[j];
        }
        
        return float4(col, 1.0f);
    }
}