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

cbuffer ConstantBuffer : register(b0)
{
    row_major float4x4 g_mWorldViewProj;
    row_major float4x4 g_mRotateWithY;
}

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR)
{
    PSInput result;
    result.position = mul(mul(position, g_mRotateWithY), g_mWorldViewProj);
    result.color = color;

    return result;
}

float4 PSMain1(PSInput input) : SV_TARGET
{
    return input.color;
}

float4 PSMain2(PSInput input) : SV_TARGET
{
    return float4(1.0, 0.0, 0.0, 1.0);
}
