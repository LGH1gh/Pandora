#pragma once
#include "stdafx.h"

struct VertexData
{
	XMFLOAT4 position;
	VertexData(float x, float y, float z):
		position(x, y, z, 1.f)
	{}
};

