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

    result.positionfloat4(position, 1.0) + offset;
    result.colorcolor;
    result.uvuv;
    
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}
