#pragma once

cbuffer cb0
{
    row_major float4x4 g_mWorldViewProj;
    float4 g_mOffset;
};

struct PSInput
{
    float4 position : SV_POSITION;
};

PSInput VSMain(float4 position : POSITION)
{
    PSInput result;
    result.position = mul(position + g_mOffset, g_mWorldViewProj);

    return result;
}
