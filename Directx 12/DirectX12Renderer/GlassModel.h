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

struct GlassModelConstantBuffer
{
	XMFLOAT4X4 WorldViewProjection;
	XMFLOAT4X4 StaticWorldViewProjection;
	XMFLOAT4X4 WorldInverseTranspose;
	XMFLOAT3 ViewPosition;
};

class GlassModel : public BaseApp
{
public:
	GlassModel(UINT width, UINT height, std::wstring title);

	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnRender();

	virtual void OnKeyUp(UINT8 wParam);
	virtual void OnKeyDown(UINT8 wParam);

private:
	void PopulateCommand();

	Kernel m_kernel;
	Pipeline m_teapotPipeline;
	Pipeline m_skyBoxPipeline;
	RootSignature m_rootSignature;
	VertexSetup m_teapotVertexSetup;
	VertexSetup m_skyboxVertexSetup;
	StepTimer m_timer;
	SimpleCamera m_camera;

	ResourceHeap m_skybox;
	ResourceHeap m_teapot;
	ResourceHeap m_depthStencil;
	ResourceHeap m_constantBuffer;
	GlassModelConstantBuffer m_constantBufferData[1];

};