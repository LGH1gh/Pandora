#include "MassModel.h"
#include "GeometryPrimitive.h"

MassModel::MassModel(UINT width, UINT height, std::wstring title) :
	BaseApp(width, height, title)
{
}

void MassModel::OnInit()
{
	m_camera.Init({ 0.0f, 0.0f, 1500.0f });
	m_camera.SetMoveSpeed(250.0f);

	m_kernel = CreateKernel(m_width, m_height, false, m_hwnd);
	m_rootSignature = CreateRootSignature(m_kernel, 1, 0, 0, &StaticSampleDesc::Init(Default));

	UINT compileFlags = COMPILE_DEBUG | COMPILE_SKIP_OPTIMIZATION;
	InputElementDesc inputElementDesc[] =
	{
		{ "POSITION", FORMAT_R32G32B32A32_FLOAT },
	};
	GraphicsPipelineStateDesc psoDesc;
	psoDesc.RootSignature = m_rootSignature;
	psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
	psoDesc.VS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\MassModelVertexShader.hlsl", "VSMain", compileFlags);
	psoDesc.PS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\MassModelPixelShader.hlsl", "PSMain", compileFlags);
	psoDesc.CullMode = CULL_MODE_NONE;
	m_pipeline = CreateGraphicsPipeline(m_kernel, psoDesc);

	m_vertexSetup = CreateVertexSetup(
		m_kernel,
		teapotVertex, sizeof(teapotVertex), sizeof(VertexData),
		teapotIndex, sizeof(teapotIndex), sizeof(DWORD)
	);

	for (UINT x = 0; x < TeapotRowCount; ++x)
	{
		for (UINT y = 0; y < TeapotColumnCount; ++y)
		{
			for (UINT z = 0; z < TeapotHeightCount; ++z)
			{
				m_constantBuffer[x * TeapotColumnCount * TeapotHeightCount + y * TeapotHeightCount + z].offset = XMFLOAT4(
					40.f * (int)(x - TeapotRowCount / 2), 40.f * (int)(y - TeapotColumnCount / 2), 40.f * (int)(z - TeapotHeightCount / 2), 0.0f
				);
				XMStoreFloat4x4(&m_constantBuffer[x * TeapotColumnCount * TeapotHeightCount + y * TeapotHeightCount + z].worldViewProjection, XMMatrixMultiply(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(0.8f, static_cast<float>(m_width) / static_cast<float>(m_height), 1.0f, 5000.0f)));

			}
		}
	}

	m_constantBufferView = CreateConstantBuffers(m_kernel, m_constantBuffer, sizeof(MassModelConstantBuffer), TeapotColumnCount * TeapotHeightCount * TeapotRowCount);
	m_depthStencilView = CreateDepthStencilViewHeap(m_kernel);
	EndOnInit(m_kernel);
}

void MassModel::OnUpdate()
{
	m_timer.Tick(NULL);
	m_camera.Update(static_cast<float>(m_timer.GetElapsedSeconds()));
	XMMATRIX view = m_camera.GetViewMatrix();
	XMMATRIX world = m_camera.GetProjectionMatrix(0.8f, 1.0f * GetWidth() / GetHeight(), 1.0f, 5000.0f);
	for (UINT i = 0; i < TeapotColumnCount * TeapotHeightCount * TeapotRowCount; ++i)
	{
		XMStoreFloat4x4(&m_constantBuffer[i].worldViewProjection, XMMatrixMultiply(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(0.8f, static_cast<float>(m_width) / static_cast<float>(m_height), 1.0f, 5000.0f)));
	}
	UpdateConstantBuffers(m_constantBufferView, m_constantBuffer, sizeof(MassModelConstantBuffer), TeapotColumnCount * TeapotHeightCount * TeapotRowCount);
}

void MassModel::OnRender()
{

	PopulateCommand();
	ExecuteCommand(m_kernel);

	EndOnRender(m_kernel);
}

void MassModel::PopulateCommand()
{
	Reset(m_kernel, m_pipeline);
	BeginPopulateGraphicsCommand(m_kernel, m_depthStencilView);
	{
		SetPipeline(m_kernel, m_pipeline);
		SetGraphicsRootSignature(m_kernel, m_rootSignature);
		SetVertexSetup(m_kernel, m_vertexSetup);
		SetDescriptorHeaps(m_kernel, std::vector<ResourceHeap>({ m_constantBufferView }));
		for (UINT i = 0; i < TeapotColumnCount * TeapotHeightCount * TeapotRowCount; ++i)
		{
			SetConstantBuffer(m_kernel, m_constantBufferView, i);
			DrawIndexed(m_kernel, 0, sizeof(teapotIndex) / sizeof(DWORD));
		}
	}
	EndPopulateGraphicsCommand(m_kernel);
}

void MassModel::OnDestroy()
{
	EndOnDestroy(m_kernel);
}

void MassModel::OnKeyDown(UINT8 key)
{
	m_camera.OnKeyDown(key);
}

void MassModel::OnKeyUp(UINT8 key)
{
	m_camera.OnKeyUp(key);
}