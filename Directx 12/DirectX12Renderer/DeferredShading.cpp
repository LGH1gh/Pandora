#include "DeferredShading.h"

DeferredShading::DeferredShading(UINT width, UINT height, std::wstring title) :
	BaseApp(width, height, title)
{}

void DeferredShading::OnInit()
{
	m_camera.Init({ 0.0f, 0.0f, 100.0f });
	m_camera.SetMoveSpeed(20.0f);
	m_lightData->position = XMFLOAT3(0, 7, -15);

	m_kernel = CreateKernel(m_width, m_height, false, m_hwnd);
	m_rootSignature = CreateRootSignature(m_kernel, 2, 4, 0);
	
	UINT compileFlags = COMPILE_DEBUG | COMPILE_SKIP_OPTIMIZATION;
	InputElementDesc inputElementDesc[] = {
		{ "POSITION", FORMAT_R32G32B32A32_FLOAT },
		{ "NORMAL", FORMAT_R32G32B32_FLOAT }
	};
	GraphicsPipelineStateDesc psoDesc;
	psoDesc.RootSignature = m_rootSignature;
	psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
	psoDesc.VS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\BasicVS.hlsl", "VSMain", compileFlags);
	psoDesc.PS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\BasicPS.hlsl", "PSMain", compileFlags);
	psoDesc.RTVFormats[0] = m_rtvFormats[0];
	psoDesc.RTVFormats[1] = m_rtvFormats[1];
	psoDesc.RTVFormats[2] = m_rtvFormats[2];
	psoDesc.NumRenderTargets = 3;
	psoDesc.DSVFormat = FORMAT_D24_UNORM_S8_UINT;
	m_pipeline = CreateGraphicsPipeline(m_kernel, psoDesc);

	InputElementDesc quadInputElementDesc[] = {
		{ "POSITION", FORMAT_R32G32B32A32_FLOAT },
		{ "TEXCOORD", FORMAT_R32G32_FLOAT }
	};
	GraphicsPipelineStateDesc lightPsoDesc;
	lightPsoDesc.RootSignature = m_rootSignature;
	lightPsoDesc.InputLayout = { quadInputElementDesc, _countof(quadInputElementDesc) };
	lightPsoDesc.VS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\ScreenQuadVS.hlsl", "VSMain", compileFlags);
	lightPsoDesc.PS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\LightPassPS.hlsl", "PSMain", compileFlags);
	lightPsoDesc.DepthStencilState.DepthEnable = false;
	lightPsoDesc.RasterizerState.DepthClipEnable = false;
	m_lightPipeline = CreateGraphicsPipeline(m_kernel, lightPsoDesc);

	
	m_quadVertexSetup = CreateVertexSetup(
		m_kernel,
		quadVertex, sizeof(quadVertex), sizeof(VertexPosTex)
	);

	m_teapotVertexSetup = CreateVertexSetup(
		m_kernel,
		teapotVertex, sizeof(teapotVertex), sizeof(VertexPosNormal),
		teapotIndex, sizeof(teapotIndex), sizeof(DWORD)
	);

	m_rtvHeap = CreateRenderTargets(m_kernel, 3, std::vector<Format>(m_rtvFormats, m_rtvFormats + 3), m_clearColor);
	m_srvRtvHeap = CreateShaderResourcesWithResource(m_kernel, m_rtvHeap, 3, std::vector<Format>(m_rtvFormats, m_rtvFormats + 3));
	m_dsvHeap = CreateDepthStencilViewHeap(m_kernel, FORMAT_D24_UNORM_S8_UINT);
	m_srvDsvHeap = CreateShaderResourcesWithResource(m_kernel, m_dsvHeap, 1, std::vector<Format>({ FORMAT_R24_UNORM_X8_TYPELESS }));
	m_constantBufferCamera = CreateConstantBuffer(m_kernel, m_cameraData, sizeof(DeferredShadingCamera));
	m_constantBufferLight = CreateConstantBuffer(m_kernel, m_lightData, sizeof(DeferredShadingLighting));

	EndOnInit(m_kernel);
}

void DeferredShading::OnUpdate()
{
	m_timer.Tick(NULL);
	m_camera.Update(static_cast<float>(m_timer.GetElapsedSeconds()));
	XMStoreFloat4x4(&m_cameraData[0].worldViewProjection, XMMatrixMultiply(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(0.8f, static_cast<float>(m_width) / static_cast<float>(m_height), 1.0f, 5000.0f)));
	XMStoreFloat4x4(&m_cameraData[0].inverseTranspose, XMMatrixInverse(nullptr, m_camera.GetViewMatrix()));
	m_cameraData[0].position = m_camera.GetEyePosition();

	m_lightData[0].position = XMFLOAT3(0, 7, -15);

	UpdateConstantBuffers(m_constantBufferCamera, m_cameraData, sizeof(DeferredShadingCamera), 1);
	UpdateConstantBuffers(m_constantBufferLight, m_lightData, sizeof(DeferredShadingLighting), 1);
}

void DeferredShading::OnRender()
{
	Reset(m_kernel, m_pipeline);
	BeginPopulateGraphicsCommand(m_kernel, nullptr, m_clearColor, true);
	{
		SetGraphicsRootSignature(m_kernel, m_rootSignature);
		SetPipeline(m_kernel, m_pipeline);
		for (UINT i = 0; i < 3; ++i)
			ClearRenderTargetView(m_kernel, m_rtvHeap, i, m_clearColor);
		ClearDepthStencilView(m_kernel, m_dsvHeap);
		SetDescriptorHeaps(m_kernel, std::vector<ResourceHeap>({ m_constantBufferCamera }));
		SetDescriptorHeaps(m_kernel, std::vector<ResourceHeap>({ m_constantBufferLight }));
		SetDescriptorHeaps(m_kernel, std::vector<ResourceHeap>({ m_srvRtvHeap }));
		SetDescriptorHeaps(m_kernel, std::vector<ResourceHeap>({ m_srvDsvHeap }));
		SetConstantBuffer(m_kernel, m_constantBufferCamera, 0);
		SetConstantBuffer(m_kernel, m_constantBufferLight, 0);
		SetTexture(m_kernel, m_srvRtvHeap);
		SetTexture(m_kernel, m_srvDsvHeap);

		SetVertexSetup(m_kernel, m_teapotVertexSetup);
		DrawIndexed(m_kernel, 0, sizeof(teapotIndex) / sizeof(DWORD));
		SetRenderTargets(m_kernel, nullptr, nullptr, 1);

		SetRenderTargetsVisible(m_kernel, m_rtvHeap, 3);
		SetDepthStencilVisible(m_kernel, m_dsvHeap, 1);
		SetPipeline(m_kernel, m_lightPipeline);
		SetVertexSetup(m_kernel, m_quadVertexSetup);
		Draw(m_kernel, 0, 4);
	}
	EndPopulateGraphicsCommand(m_kernel);
	ExecuteCommand(m_kernel);

	EndOnRender(m_kernel);
}

void DeferredShading::OnDestroy()
{
	EndOnDestroy(m_kernel);
}