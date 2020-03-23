#pragma once

#include "Renderer.h"
#include "BaseApp.h"
#include "StepTimer.h"
#include "SimpleCamera.h"
#include "Geometry.h"

extern const VertexPosNormal teapotVertex[];
extern const UINT teapotVertexCount;
extern const DWORD teapotIndex[];
extern const UINT teapotIndexCount;
extern const VertexPos skyboxVertex[];
extern const UINT skyboxVertexCount;
extern const DWORD skyboxIndex[];
extern const UINT skyboxIndexCount;

struct MassModelConstantBuffer
{
	XMFLOAT4X4 worldViewProjection;
	XMFLOAT4 offset;
};

struct MassModelPosition
{
	XMFLOAT3 position;
};


class MassModel : public BaseApp
{
public:
	MassModel(UINT width, UINT height, std::wstring title);

	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnRender();

	virtual void OnKeyUp(UINT8 wParam);
	virtual void OnKeyDown(UINT8 wParam);

private:

	void PopulateCommand();
	static const UINT TeapotRowCount = 15;
	static const UINT TeapotColumnCount = 15;
	static const UINT TeapotHeightCount = 10;

	Kernel m_kernel;
	Pipeline m_pipeline;
	RootSignature m_rootSignature;
	VertexSetup m_vertexSetup;
	StepTimer m_timer;
	SimpleCamera m_camera;

	MassModelConstantBuffer m_constantBuffer[TeapotColumnCount * TeapotHeightCount * TeapotRowCount];
	ResourceHeap m_constantBufferView;
	ResourceHeap m_depthStencilView;
};