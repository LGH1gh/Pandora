#pragma once

interface Geometry
{
	virtual UINT GetStrideInBytes() = 0;
	virtual UINT GetSizeInBytes() = 0;
	virtual void* GetDataPtr() = 0;
};