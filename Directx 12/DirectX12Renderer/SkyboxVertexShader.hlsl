cbuffer cb0
{
    row_major float4x4 g_worldViewProjection;
    row_major float4x4 g_staticWorldViewProjection;
};

TextureCube	g_EnvironmentTexture : register(t0);
SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

struct VertexPos
{
    float4 PosL : POSITION;
};

struct VertexPosHL
{
    float4 PosH : SV_POSITION;
    float4 PosL : POSITION;
};

VertexPosHL VSMain(VertexPos vIn)
{
    VertexPosHL vOut;

    // ����z = wʹ��z/w = 1(��պб�����Զƽ��)
    float4 posH = mul(vIn.PosL, g_staticWorldViewProjection);
    vOut.PosH = posH.xyww;
    vOut.PosL = vIn.PosL;
    return vOut;
}