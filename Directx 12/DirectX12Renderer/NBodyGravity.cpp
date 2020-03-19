#include "NBodyGravity.h"

const float NBodyGravity::ParticleSpread = 400.0f;

NBodyGravity::NBodyGravity(UINT width, UINT height, std::wstring title) :
	BaseApp(width, height, title)
{
}

void NBodyGravity::OnInit()
{
	m_camera.Init({ 0.0f, 0.0f, 1500.0f });
	m_camera.SetMoveSpeed(250.0f);

	m_kernel = CreateKernel(m_width, m_height, false, m_hwnd);
	m_computeKernel = CreateComputeKernel(m_kernel);
	m_rootSignature = CreateRootSignature(m_kernel, 1, 1, 0);
	m_computeRootSignature = CreateRootSignature(m_kernel, 1, 1, 1);

	UINT compileFlags = COMPILE_DEBUG | COMPILE_SKIP_OPTIMIZATION;
	InputElementDesc inputElementDesc[] =
	{
		{ "COLOR", FORMAT_R32G32B32A32_FLOAT },
	};
	GraphicsPipelineStateDesc psoDesc;
	psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
	psoDesc.RootSignature = m_rootSignature;
	psoDesc.VS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\ParticleDraw.hlsl", "VSParticleDraw", compileFlags);
	psoDesc.GS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\ParticleDraw.hlsl", "GSParticleDraw", compileFlags);
	psoDesc.PS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\ParticleDraw.hlsl", "PSParticleDraw", compileFlags);
	psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = BLEND_ONE;
	psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = BLEND_ZERO;
	psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = BLEND_ZERO;
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.DepthWriteMask = DEPTH_WRITE_MASK_ZERO;
	psoDesc.PrimitiveTopologyType = PRIMITIVE_TOPOLOGY_TYPE_POINT;
	m_pipelineState = CreateGraphicsPipeline(m_kernel, psoDesc);

	ComputePipelineStateDesc computePsoDesc;
	computePsoDesc.RootSignature = m_computeRootSignature;
	computePsoDesc.CS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\NBodyGravityCS.hlsl", "CSMain", compileFlags);
	m_computePipelineState = CreateComputePipeline(m_kernel, computePsoDesc);
	
	LoadData();

	m_constantBufferCS.param[0] = ParticleCount;
	m_constantBufferCS.param[1] = int(ceil(ParticleCount / 128.0f));
	m_constantBufferCS.paramf[0] = 0.1f;
	m_constantBufferCS.paramf[1] = 1.0f;
	m_constantBufferHeapCS = CreateConstantBuffer(m_kernel, &m_constantBufferCS, sizeof(ConstantBufferCS));
	m_constantBufferHeapGS = CreateConstantBuffer(m_kernel, &m_constantBufferGS, sizeof(ConstantBufferGS));

	EndOnInit(m_kernel);
}

void NBodyGravity::LoadData()
{
	std::vector<ParticleVertex> vertices;
	vertices.resize(ParticleCount);
	for (UINT i = 0; i < ParticleCount; i++)
	{
		vertices[i].color = XMFLOAT4(1.0f, 1.0f, 0.2f, 1.0f);
	}
	const UINT bufferSize = ParticleCount * sizeof(ParticleVertex);

	m_vertexSetup = CreateVertexSetup(
		m_kernel,
		&vertices[0], bufferSize, sizeof(ParticleVertex)
	);

	std::vector<Particle> data;
	data.resize(ParticleCount);
	const UINT dataSize = ParticleCount * sizeof(Particle);
	float centerSpread = ParticleSpread * 0.50f;
	LoadParticles(&data[0], XMFLOAT3(centerSpread, 0, 0), XMFLOAT4(0, 0, -20, 1 / 100000000.0f), ParticleSpread, ParticleCount / 2);
	LoadParticles(&data[ParticleCount / 2], XMFLOAT3(-centerSpread, 0, 0), XMFLOAT4(0, 0, 20, 1 / 100000000.0f), ParticleSpread, ParticleCount / 2);
	
	m_computeBuffer = CreateComputeBuffer(m_kernel, &data[0], ParticleCount, sizeof(Particle));
}

float NBodyGravity::RandomPercent()
{
	float ret = static_cast<float>((rand() % 10000) - 5000);
	return ret / 5000.0f;
}

void NBodyGravity::LoadParticles(_Out_writes_(numParticles) Particle* pParticles, const XMFLOAT3& center, const XMFLOAT4& velocity, float spread, UINT numParticles)
{
	srand(0);
	for (UINT i = 0; i < numParticles; i++)
	{
		XMFLOAT3 delta(spread, spread, spread);

		while (XMVectorGetX(XMVector3LengthSq(XMLoadFloat3(&delta))) > spread* spread)
		{
			delta.x = RandomPercent() * spread;
			delta.y = RandomPercent() * spread;
			delta.z = RandomPercent() * spread;
		}

		pParticles[i].position.x = center.x + delta.x;
		pParticles[i].position.y = center.y + delta.y;
		pParticles[i].position.z = center.z + delta.z;
		pParticles[i].position.w = 10000.0f * 10000.0f;

		pParticles[i].velocity = velocity;
	}
}

void NBodyGravity::OnUpdate()
{
	m_timer.Tick(NULL);
	m_camera.Update(static_cast<float>(m_timer.GetElapsedSeconds()));
	XMMATRIX view = m_camera.GetViewMatrix();
	XMMATRIX world = m_camera.GetProjectionMatrix(0.8f, 1.0f * GetWidth() / GetHeight(), 1.0f, 5000.0f);
	XMStoreFloat4x4(&m_constantBufferGS.worldViewProjection, XMMatrixMultiply(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(0.8f, static_cast<float>(m_width) / static_cast<float>(m_height), 1.0f, 5000.0f)));
	XMStoreFloat4x4(&m_constantBufferGS.inverseView, XMMatrixInverse(nullptr, m_camera.GetViewMatrix()));

	UpdateConstantBuffer(m_constantBufferHeapGS, &m_constantBufferGS, sizeof(m_constantBufferGS));
}

void NBodyGravity::OnRender()
{
	PopulateComputeCommand();
	PopulateGraphicsCommand();
	EndOnRender(m_kernel);
}

void NBodyGravity::PopulateGraphicsCommand()
{
	Reset(m_kernel, m_pipelineState);
	const float clearColor[] = { 0.0f, 0.0f, 0.1f, 0.0f };
	BeginPopulateGraphicsCommand(m_kernel, nullptr, clearColor);
	{
		SetPipeline(m_kernel, m_pipelineState);
		SetGraphicsRootSignature(m_kernel, m_rootSignature);
		SetConstantBuffer(m_kernel, m_constantBufferHeapGS);
		SetShaderResource(m_kernel, m_computeBuffer);
		SetVertexSetup(m_kernel, m_vertexSetup);
		Draw(m_kernel, 0, ParticleCount);
	}
	EndPopulateGraphicsCommand(m_kernel);
	ExecuteCommand(m_kernel);
}

void NBodyGravity::PopulateComputeCommand()
{
	Simulate();
	Reset(m_computeKernel, m_computePipelineState);
}

void NBodyGravity::Simulate()
{
	BeginPopulateComputeCommand(m_computeKernel, m_computeBuffer);
	{
		SetComputePipeline(m_computeKernel, m_computePipelineState);
		SetComputeRootSignature(m_computeKernel, m_computeRootSignature);
		SetConstantBuffer(m_computeKernel, m_constantBufferHeapCS);
		SetComputeBuffer(m_computeKernel, m_computeBuffer);
	}
	const UINT dispatch[] = { ParticleCount / 128, 1, 1 };
	EndPopulateComputeCommand(m_computeKernel, m_computeBuffer, dispatch);
	ExecuteCommand(m_computeKernel);
}

void NBodyGravity::OnDestroy()
{
	EndOnDestroy(m_kernel);
}