#pragma once

#include "stdafx.h"
#include "ConstantBuffer.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class GraphicsConstantBuffer : public ConstantBuffer
{
public:
    struct Data
    {
        XMFLOAT4 worldViewProjection;
        XMFLOAT4 inverseView;

        // Constant buffers are 256-byte aligned in GPU memory. Padding is added
        // for convenience when computing the struct's size.
        float padding[32];
    };

    virtual void OnUpdate()
    {

    }

    virtual UINT GetSize()
    {
        return sizeof(Data);
    }

    virtual void* GetDataPtr()
    {
        return static_cast<void*>(&data);
    }

    Data data;
};