#pragma once
#include "Geometry.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	Vertex(XMFLOAT3 position, XMFLOAT4 color) :
		position(position),
		color(color)
	{
	}
	Vertex() {}
};

class Vertices : public Geometry
{
public:
	std::vector<Vertex> data;

	void AddVertex(Vertex datum)
	{
		data.push_back(datum);
	}

	virtual UINT GetStrideInBytes()
	{
		return sizeof(Vertex);
	}

	virtual UINT GetSizeInBytes()
	{
		return sizeof(Vertex) * (UINT)data.size();
	}

	virtual void* GetDataPtr()
	{
		return &data[0];
	}
};