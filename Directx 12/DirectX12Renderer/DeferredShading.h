#pragma once
#include "BaseApp.h"
#include "StepTimer.h"
#include "SimpleCamera.h"
#include "Geometry.h"

struct DeferredShadingLighting
{
	XMFLOAT3 position;
};

struct DeferredShadingCamera
{
	XMFLOAT4X4 worldViewProjection;
	XMFLOAT4X4 inverseTranspose;
	XMFLOAT3 position;
};

class DeferredShading : public BaseApp
{
public:
	DeferredShading(UINT width, UINT height, std::wstring title);

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();

private:
	SimpleCamera m_camera;
	StepTimer m_timer;

	Kernel m_kernel;
	RootSignature m_rootSignature;
	Pipeline m_pipeline;
	Pipeline m_lightPipeline;
	VertexSetup m_quadVertexSetup;
	VertexSetup m_teapotVertexSetup;

	DeferredShadingLighting m_lightData[1];
	ResourceHeap m_constantBufferLight;
	DeferredShadingCamera m_cameraData[1];
	ResourceHeap m_constantBufferCamera;
	ResourceHeap m_rtvHeap;
	ResourceHeap m_dsvHeap;
	ResourceHeap m_srvRtvHeap;
	ResourceHeap m_srvDsvHeap;
	Format m_rtvFormats[3] = { FORMAT_R11G11B10_FLOAT , FORMAT_R8G8B8A8_SNORM , FORMAT_R8G8B8A8_UNORM };
	float m_clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
};