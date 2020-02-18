#pragma once

#include "stdafx.h"
#include "ConstantBuffer.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class SceneConstantBuffer : public ConstantBuffer
{
public:
    struct Data
    {
        XMFLOAT4 offset;
    };

    SceneConstantBuffer()
    {
        data.offset = XMFLOAT4(0.0, 0.0, 0.0, 0.0);
    }

	virtual void OnUpdate()
	{
        const float translationSpeed = 0.005f;
        const float offsetBounds = 1.00f;
        data.offset.x += translationSpeed;

        if (data.offset.x > offsetBounds)
        {
            data.offset.x = -offsetBounds;
        }
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