cbuffer ConstantBuffer : register(b0)
{
    row_major float4x4 g_mWorldViewProj;
    row_major float4x4 g_mRotateWithY;
    row_major float4x4 g_mRotateWithZ;
}

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

PSInput VSMain(float4 position : POSITION, float2 texCoord : TEXCOORD)
{
    PSInput result;
    //result.position = mul(g_mWorldViewProj, position);
    //result.position = mul(position, g_mWorldViewProj);
    //result.position = mul(position, g_mRotateWithY);
    result.position = mul(mul(mul(position, g_mRotateWithY), g_mRotateWithZ), g_mWorldViewProj);
    result.texCoord = texCoord;

    return result;
}