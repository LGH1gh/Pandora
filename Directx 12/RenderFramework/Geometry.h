#pragma once

interface Geometry
{
	virtual UINT GetStrideInBytes() = 0;
	virtual UINT GetSizeInBytes() = 0;
	virtual void* GetDataPtr() = 0;
	virtual D3D12_INPUT_LAYOUT_DESC GetInputLayout() = 0;
	virtual DWORD* GetIndexPtr() = 0;
	virtual UINT GetIndexSize() = 0;
};