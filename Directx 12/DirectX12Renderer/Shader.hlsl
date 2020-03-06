//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

//cbuffer constantbuffer : register(b0)
//{
//    row_major float4x4 g_mWorldViewProj;
//    row_major float4x4 g_mRotateWithY;
//}
//
//Texture2D t1 : register(t0);
//SamplerState s1 : register(s0);

cbuffer ConstantBuffer : register(b0)
{
     float4 offset;
}

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR)
{
    PSInput result;
    result.position = position;
    //result.position = mul(position, g_mWorldViewProj);
    //result.position = mul(mul(position, g_mRotateWithY), g_mWorldViewProj);
    result.color = color;

    return result;
}

//float4 PSMain(PSInput input) : SV_TARGET
//{
//    return t1.Sample(s1, input.texCoord);
//}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color + offset;
}
