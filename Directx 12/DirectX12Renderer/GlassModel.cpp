#include "GlassModel.h"

GlassModel::GlassModel(UINT width, UINT height, std::wstring title) :
	BaseApp(width, height, title)
{
}

void GlassModel::OnInit()
{
	m_camera.Init({ 0.0f, 0.0f, 100.0f });
	m_camera.SetMoveSpeed(200.0f);

	m_kernel = CreateKernel(m_width, m_height, false, m_hwnd);
	m_rootSignature = CreateRootSignature(m_kernel, 1, 1, 0);

	UINT compileFlags = COMPILE_DEBUG | COMPILE_SKIP_OPTIMIZATION;
	InputElementDesc inputElementDesc[] =
	{
		{ "POSITION", FORMAT_R32G32B32A32_FLOAT },
		{ "NORMAL", FORMAT_R32G32B32_FLOAT }
	};
	GraphicsPipelineStateDesc psoDesc;
	psoDesc.RootSignature = m_rootSignature;
	psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
	psoDesc.VS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\GlassModelVertexShader.hlsl", "VSMain", compileFlags);
	psoDesc.PS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\GlassModelPixelShader.hlsl", "PSMain", compileFlags);
	psoDesc.CullMode = CULL_MODE_NONE;
	m_teapotPipeline = CreateGraphicsPipeline(m_kernel, psoDesc);

	compileFlags |= COMPILE_PACK_MATRIX_ROW_MAJOR;
	InputElementDesc skyboxInputElementDesc[] =
	{
		{ "POSITION", FORMAT_R32G32B32A32_FLOAT }
	};
	GraphicsPipelineStateDesc skyboxPsoDesc;
	skyboxPsoDesc.RootSignature = m_rootSignature;
	skyboxPsoDesc.InputLayout = { skyboxInputElementDesc, _countof(skyboxInputElementDesc) };
	skyboxPsoDesc.VS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\SkyboxVertexShader.hlsl", "VSMain", compileFlags);
	skyboxPsoDesc.PS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\SkyboxPixelShader.hlsl", "PSMain", compileFlags);
	skyboxPsoDesc.DepthStencilState.DepthEnable = false;
	skyboxPsoDesc.DepthStencilState.StencilEnable = false;
	skyboxPsoDesc.DepthStencilState.DepthFunc = COMPARISON_FUNC_LESS_EQUAL;
	skyboxPsoDesc.CullMode = CULL_MODE_NONE;
	m_skyBoxPipeline = CreateGraphicsPipeline(m_kernel, skyboxPsoDesc);

	m_teapotVertexSetup = CreateVertexSetup(
		m_kernel,
		teapotVertex, sizeof(teapotVertex), sizeof(VertexPosNormal),
		teapotIndex, sizeof(teapotIndex), sizeof(DWORD)
	);


	m_skyboxVertexSetup = CreateVertexSetup(
		m_kernel,
		skyboxVertex, sizeof(skyboxVertex), sizeof(VertexPos),
		skyboxIndex, sizeof(skyboxIndex), sizeof(DWORD)
	);

	m_depthStencil = CreateDepthStencilViewHeap(m_kernel);
	m_constantBuffer = CreateConstantBuffer(m_kernel, m_constantBufferData, sizeof(GlassModelConstantBuffer));
	m_skybox = CreateSkybox(m_kernel, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Skybox\\desert-cube.dds");

	EndOnInit(m_kernel);
}

XMFLOAT4 Calculate(XMFLOAT4X4 matrix, XMFLOAT4 vector)
{
	XMFLOAT4 result;
	result.x = matrix._11 * vector.x + matrix._12 * vector.y + matrix._13 * vector.z + matrix._14 * vector.w;
	result.y = matrix._21 * vector.x + matrix._22 * vector.y + matrix._23 * vector.z + matrix._24 * vector.w;
	result.z = matrix._31 * vector.x + matrix._32 * vector.y + matrix._33 * vector.z + matrix._34 * vector.w;
	result.w = matrix._41 * vector.x + matrix._42 * vector.y + matrix._43 * vector.z + matrix._44 * vector.w;
	return result;
}

void GlassModel::OnUpdate()
{
	m_timer.Tick(NULL);
	m_camera.Update(static_cast<float>(m_timer.GetElapsedSeconds()));
	XMStoreFloat4x4(&m_constantBufferData[0].WorldViewProjection, XMMatrixMultiply(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(0.8f, static_cast<float>(m_width) / static_cast<float>(m_height), 1.0f, 5000.0f)));
	XMStoreFloat4x4(&m_constantBufferData[0].StaticWorldViewProjection, XMMatrixMultiply(m_camera.GetStaticViewMatrix(), m_camera.GetProjectionMatrix(0.8f, static_cast<float>(m_width) / static_cast<float>(m_height), 1.0f, 5000.0f)));
	XMStoreFloat4x4(&m_constantBufferData[0].WorldInverseTranspose, XMMatrixInverse(nullptr, m_camera.GetViewMatrix()));
	m_constantBufferData[0].ViewPosition = m_camera.GetEyePosition();
	UpdateConstantBuffers(m_constantBuffer, m_constantBufferData, sizeof(GlassModelConstantBuffer), 1);
}

void GlassModel::OnRender()
{
	PopulateCommand();
	ExecuteCommand(m_kernel);

	EndOnRender(m_kernel);
}

void GlassModel::PopulateCommand()
{
	Reset(m_kernel, m_teapotPipeline);
	BeginPopulateGraphicsCommand(m_kernel, m_depthStencil);
	{
		SetDescriptorHeaps(m_kernel, std::vector<ResourceHeap>({ m_constantBuffer }));
		SetDescriptorHeaps(m_kernel, std::vector<ResourceHeap>({ m_skybox }));
		SetGraphicsRootSignature(m_kernel, m_rootSignature);


		SetPipeline(m_kernel, m_skyBoxPipeline);
		SetTexture(m_kernel, m_skybox);
		SetConstantBuffer(m_kernel, m_constantBuffer);
		SetVertexSetup(m_kernel, m_skyboxVertexSetup);
		DrawIndexed(m_kernel, 0, sizeof(skyboxIndex) / sizeof(DWORD));

		SetPipeline(m_kernel, m_teapotPipeline);
		SetVertexSetup(m_kernel, m_teapotVertexSetup);
		DrawIndexed(m_kernel, 0, sizeof(teapotIndex) / sizeof(DWORD));
	}
	EndPopulateGraphicsCommand(m_kernel);
}

void GlassModel::OnDestroy()
{
	EndOnDestroy(m_kernel);
}

void GlassModel::OnKeyDown(UINT8 key)
{
	m_camera.OnKeyDown(key);
}

void GlassModel::OnKeyUp(UINT8 key)
{
	m_camera.OnKeyUp(key);
}