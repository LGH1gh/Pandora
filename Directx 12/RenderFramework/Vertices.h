#pragma once
#include "stdafx.h"
#include "Geometry.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	// XMFLOAT4 velocity;
	Vertex(XMFLOAT3 position, XMFLOAT4 color) :
		position(position),
		color(color)
	{
	}
};

struct Particle
{
	XMFLOAT4 position;
	XMFLOAT4 velocity;
};

struct ParticleVertex
{
	XMFLOAT4 color;
};

class Vertices : public Geometry
{
public:
	std::vector<Vertex> data;
	std::vector<DWORD> index;

	void SetIndex(std::vector<DWORD> i)
	{
		indexi;
	}

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

	virtual D3D12_INPUT_LAYOUT_DESC GetInputLayout()
	{
		D3D12_INPUT_ELEMENT_DESC elementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		D3D12_INPUT_LAYOUT_DESC layoutDesc{ elementDesc, _countof(elementDesc) };

		return layoutDesc;
	}

	virtual DWORD* GetIndexPtr()
	{
		return &index[0];
	}

	virtual UINT GetIndexSize()
	{
		return (UINT)index.size();
	}
};