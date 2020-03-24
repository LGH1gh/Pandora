#pragma once




TextureCube	g_EnvironmentTexture : register(t0);

SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : TEXCOORD0;
    float3 eye : TEXCOORD1;
};

float4 PSMain(PSInput input) : SV_TARGET
{
    float3 vLight = {0.5f, -0.5f, 0.0f};
    float AmbientIntensity = 1;
    float DiffuseIntensity = 1;
    float SpecularIntensity = 1;
    float4 AmbientColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 DiffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 SpecularColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float SpecularPow = 2;


    float3 light = normalize(vLight);
    float3 eye = normalize(input.eye);
    float3 normal = normalize(input.normal);

    float ndl = saturate(dot(light, normal));
    //光线的反射方向
    float3 r = normalize(reflect(-light, normal));
    float rdv = pow(saturate(dot(r, eye)), SpecularPow);

    //环境贴图 
    //这两行主要功能
    float3 reflectEye = reflect(eye, normal);
    float3 refractEye = refract(eye, normal, 0.99);
    float4 reflectColor = g_EnvironmentTexture.Sample(gsamLinearClamp, reflectEye);
    float4 refractColor = g_EnvironmentTexture.Sample(gsamLinearClamp, refractEye);


    ////光照方程
    float4 color = AmbientColor * AmbientIntensity + DiffuseColor * DiffuseIntensity * ndl + SpecularColor * SpecularIntensity * rdv;
    return (reflectColor * 0.7 + refractColor * 0.3) * color;
}