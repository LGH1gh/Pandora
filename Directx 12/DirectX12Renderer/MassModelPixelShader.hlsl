#pragma once

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : TEXCOORD0;
};

float4 PSMain(PSInput input) : SV_TARGET
{
    float3 vLight = {0.5f, -0.5f, 0.0f};
    float AmbientIntensity = 1;
    float DiffuseIntensity = 1;
    float4 AmbientColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 DiffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float SpecularPow = 2;

    float3 light = normalize(vLight);
    float3 normal = normalize(input.normal);

    float ndl = saturate(dot(light, normal));
    //光线的反射方向
    float3 r = normalize(reflect(-light, normal));
    float4 color = AmbientColor * AmbientIntensity + DiffuseColor * DiffuseIntensity * ndl;

    return normalize(color);
}