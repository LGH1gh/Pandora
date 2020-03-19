#pragma once

#include "Renderer.h"
#include "BaseApp.h"
#include "StepTimer.h"
#include "SimpleCamera.h"

struct ParticleVertex
{
    XMFLOAT4 color;
};

// Position and velocity data for the particles in the system.
// Two buffers full of Particle data are utilized in this sample.
// The compute thread alternates writing to each of them.
// The render thread renders using the buffer that is not currently
// in use by the compute shader.
struct Particle
{
    XMFLOAT4 position;
    XMFLOAT4 velocity;
};

struct ConstantBufferGS
{
    XMFLOAT4X4 worldViewProjection;
    XMFLOAT4X4 inverseView;

    // Constant buffers are 256-byte aligned in GPU memory. Padding is added
    // for convenience when computing the struct's size.
    float padding[32];
};

struct ConstantBufferCS
{
    UINT param[4];
    float paramf[4];
};

class NBodyGravity : public BaseApp
{
public:
	NBodyGravity(UINT width, UINT height, std::wstring title);
	
	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnRender();

private:
	void LoadData();
	void LoadParticles(_Out_writes_(numParticles) Particle* pParticles, const XMFLOAT3& center, const XMFLOAT4& velocity, float spread, UINT numParticles);
	float RandomPercent();
	void Simulate();
	static const float ParticleSpread;
	static const UINT ParticleCount = 10000;
	SimpleCamera m_camera;
	StepTimer m_timer;


	Kernel m_kernel;
	ComputeKernel m_computeKernel;
	RootSignature m_rootSignature;
	RootSignature m_computeRootSignature;

	Pipeline m_pipelineState;
	Pipeline m_computePipelineState;
	VertexSetup m_vertexSetup;

	ResourceHeap m_computeBuffer;
	ResourceHeap m_constantBufferHeapGS;
	ConstantBufferGS m_constantBufferGS;
	ResourceHeap m_constantBufferHeapCS;
	ConstantBufferCS m_constantBufferCS;

	void PopulateGraphicsCommand();
	void PopulateComputeCommand();
};