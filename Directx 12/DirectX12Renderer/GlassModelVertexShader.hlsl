#pragma once

cbuffer cb0
{
    row_major float4x4 g_worldViewProjection;
    row_major float4x4 g_staticWorldViewProjection;
    row_major float4x4 g_worldInverseTranspose;
    float3 g_eyePosition;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : TEXCOORD0;
    float3 eye : TEXCOORD1;
};

PSInput VSMain(float4 position : POSITION, float3 normal : NORMAL)
{
    PSInput result;
    result.position = mul(position, g_worldViewProjection);
    result.normal = mul(normal, g_worldViewProjection);
    result.eye = position - g_eyePosition;
    return result;
}
