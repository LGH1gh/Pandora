cbuffer SceneConstantBuffer : register(b0)
{
    float4 offset;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float4 uv : TEXCOORD;
};

PSInput VSMain(float3 position : POSITION, float4 color : COLOR, float4 uv : TEXCOORD)
{
    PSInput result;

    result.position = float4(position, 1.0) + offset;
    result.color = color;
    result.uv = uv;
    
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}
