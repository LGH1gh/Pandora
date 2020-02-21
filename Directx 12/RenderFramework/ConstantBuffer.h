#pragma once

interface ConstantBuffer {
	virtual void OnUpdate()0;
	virtual UINT GetSize()0;
	virtual void* GetDataPtr()0;
};