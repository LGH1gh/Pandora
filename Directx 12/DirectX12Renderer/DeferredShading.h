#pragma once
#include "BaseApp.h"
#include "StepTimer.h"
#include "SimpleCamera.h"
#include "Geometry.h"

struct DeferredShadingConstantBuffer
{
	XMFLOAT4X4 worldViewProjection;
	XMFLOAT4X4 inverseTranspose;
	XMFLOAT3 cameraPosition;
	XMFLOAT3 lightPosition;
};

class DeferredShading : public BaseApp
{
public:
	DeferredShading(UINT width, UINT height, std::wstring title);

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();

	virtual void OnKeyUp(UINT8 wParam);
	virtual void OnKeyDown(UINT8 wParam);

private:
	SimpleCamera m_camera;
	StepTimer m_timer;

	Kernel m_kernel;
	RootSignature m_rootSignature;
	Pipeline m_pipeline;
	Pipeline m_lightPipeline;
	VertexSetup m_quadVertexSetup;
	VertexSetup m_teapotVertexSetup;


	DeferredShadingConstantBuffer m_constantData[1];
	ResourceHeap m_constantBuffer;
	ResourceHeap m_rtvHeap;
	ResourceHeap m_dsvHeap;
	ResourceHeap m_srvHeap;
	ResourceHeap m_srvRtvHeap;
	ResourceHeap m_srvDsvHeap;
	Format m_rtvFormats[3] = { FORMAT_R11G11B10_FLOAT , FORMAT_R8G8B8A8_SNORM , FORMAT_R8G8B8A8_UNORM };
	float m_clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
};