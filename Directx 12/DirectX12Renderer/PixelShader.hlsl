Texture2D t1 : register(t0);
SamplerState s1 : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};


float4 PSMain(PSInput input) : SV_TARGET
{
    return t1.Sample(s1, input.texCoord);
}

//float4 PSMain(PSInput input) : SV_TARGET
//{
//    return float4(input.texCoord, 1.0f, 1.0f);
//}
