#include "DeferredShading.h"

DeferredShading::DeferredShading(UINT width, UINT height, std::wstring title) :
	BaseApp(width, height, title)
{}

void DeferredShading::OnInit()
{
	m_camera.Init({ 0.0f, 0.0f, 100.0f });
	m_camera.SetMoveSpeed(2.0f);
	m_constantData->lightPosition = XMFLOAT3(0, 7, -15);

	m_kernel = CreateKernel(m_width, m_height, false, m_hwnd);
	m_rootSignature = CreateRootSignature(m_kernel, 1, 4, 0);
	
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
		//quadIndex, sizeof(quadIndex), sizeof(DWORD)
	);

	m_teapotVertexSetup = CreateVertexSetup(
		m_kernel,
		teapotVertex, sizeof(teapotVertex), sizeof(VertexPosNormal),
		teapotIndex, sizeof(teapotIndex), sizeof(DWORD)
	);

	m_srvHeap = InitShaderResourceView(m_kernel, 4);
	m_rtvHeap = CreateRenderTargets(m_kernel, 3, std::vector<Format>(m_rtvFormats, m_rtvFormats + 3), m_clearColor);
	m_dsvHeap = CreateDepthStencilViewHeap(m_kernel, FORMAT_D24_UNORM_S8_UINT);
	CreateShaderResourceView(m_srvHeap, m_kernel, m_rtvHeap, 3, std::vector<Format>(m_rtvFormats, m_rtvFormats + 3), 0);
	CreateShaderResourceView(m_srvHeap, m_kernel, m_dsvHeap, 1, std::vector<Format>({ FORMAT_R24_UNORM_X8_TYPELESS }), 3);
	m_constantBuffer = CreateConstantBuffer(m_kernel, m_constantData, sizeof(DeferredShadingConstantBuffer));

	EndOnInit(m_kernel);
}

inline XMMATRIX GMathFM(XMFLOAT4X4& mat)
{
	return XMLoadFloat4x4(&mat);
}

inline XMFLOAT4X4 GMathMF(XMMATRIX& mat)
{

	XMFLOAT4X4 val;
	XMStoreFloat4x4(&val, mat);
	return val;

}

void DeferredShading::OnUpdate()
{
	m_timer.Tick(NULL);
	m_camera.Update(static_cast<float>(m_timer.GetElapsedSeconds()));
	XMMATRIX view = m_camera.GetViewMatrix();
	XMMATRIX projection = m_camera.GetProjectionMatrix(1.4f, static_cast<float>(m_width) / static_cast<float>(m_height), 0.01f, 10000.0f);
	XMFLOAT4X4 matScreen = XMFLOAT4X4(2.0f / m_width, 0, 0, 0, 0, -2.0f / m_height, 0, 0, 0, 0, 1.f, 0, -1.f, 1.f, 0, 1.f);

	XMMATRIX inverseTranspose = XMMatrixTranspose(
		GMathFM(matScreen) *
		XMMatrixInverse(&XMMatrixDeterminant(projection), projection) *
		XMMatrixInverse(&XMMatrixDeterminant(view), view)
	);

	XMStoreFloat4x4(&m_constantData[0].worldViewProjection, XMMatrixMultiply(view, projection));
	m_constantData[0].cameraPosition = m_camera.GetEyePosition();
	XMStoreFloat4x4(&m_constantData[0].inverseTranspose, inverseTranspose);
	m_constantData[0].lightPosition = XMFLOAT3(0, 7, -15);


	XMMATRIX result = GMathFM(m_constantData[0].worldViewProjection) * GMathFM(m_constantData[0].inverseTranspose);
	UpdateConstantBuffers(m_constantBuffer, m_constantData, sizeof(DeferredShadingConstantBuffer), 1);
}

void DeferredShading::OnRender()
{
	Reset(m_kernel, m_pipeline);
	BeginPopulateGraphicsCommand(m_kernel, nullptr, m_clearColor, true);
	{
		SetGraphicsRootSignature(m_kernel, m_rootSignature);
		SetPipeline(m_kernel, m_pipeline);

		SetDescriptorHeaps(m_kernel, std::vector<ResourceHeap>({ m_constantBuffer }));
		SetConstantBuffer(m_kernel, m_constantBuffer, 0);

		for (UINT i = 0; i < 3; ++i)
			ClearRenderTargetView(m_kernel, m_rtvHeap, i, m_clearColor);
		ClearDepthStencilView(m_kernel, m_dsvHeap);
		SetRenderTargets(m_kernel, m_rtvHeap, m_dsvHeap, 3);
		SetDescriptorHeaps(m_kernel, std::vector<ResourceHeap>({ m_srvHeap }));
		SetTexture(m_kernel, m_srvHeap);

		SetVertexSetup(m_kernel, m_teapotVertexSetup);
		DrawIndexed(m_kernel, 0, sizeof(teapotIndex) / sizeof(DWORD));

		SetRenderTargets(m_kernel, nullptr, nullptr, 1);
		SetRenderTargetsVisible(m_kernel, m_rtvHeap, 3);
		SetDepthStencilVisible(m_kernel, m_dsvHeap, 1);
		SetPipeline(m_kernel, m_lightPipeline);
		SetPrimitiveTopology(m_kernel, PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
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

void DeferredShading::OnKeyDown(UINT8 key)
{
	m_camera.OnKeyDown(key);
}

void DeferredShading::OnKeyUp(UINT8 key)
{
	m_camera.OnKeyUp(key);
}