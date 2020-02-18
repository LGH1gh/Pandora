#pragma once

#include "ConstantBuffer.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class SceneConstantBuffer : public ConstantBuffer
{
public:
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


    struct Data
    {
        XMFLOAT4 offset;
    } data;
};