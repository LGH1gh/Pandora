#pragma once

#include "stdafx.h"
#include "ConstantBuffer.h"

class ComputeConstantBuffer : public ConstantBuffer
{
public:
    struct Data
    {
        UINT param[4];
        float paramf[4];
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