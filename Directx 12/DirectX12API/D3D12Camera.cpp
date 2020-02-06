#include "stdafx.h"
#include "D3D12Camera.h"

#define InterlockedGetValue(object) InterlockedCompareExchange(object, 0, 0)

const float D3D12Camera::ParticleSpread = 400.f;

D3D12Camera::D3D12Camera(UINT width, UINT height, std::wstring name):
	DXBase(width, height, name),
	m_frameIndex(0),
	m_viewport(0.f, 0.f, static_cast<float>(width), static_cast<float>(height)),
	m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)),
	m_rtvDescriptorSize(0),
	m_srvUavDescriptorSize(0),
	m_pConstantBufferGSData(nullptr),
	m_renderContextFenceValue(0),
	m_terminating(0),
	m_srvIndex{},
	m_frameFenceValues{}
{
	for (int n = 0; n < ThreadCount; n++)
	{
		m_renderContextFenceValues[n] = 0;
		m_threadFenceValues[n] = 0;
	}

	if (m_widthInstances * (m_heightInstances - 1) >= ThreadCount)
	{
		m_heightInstances--;
	}
}

void D3D12Camera::OnInit()
{
	m_camera.Init({ 0.f, 0.f, 1500.f });
	m_camera.SetMoveSpeed(250.f);

	LoadPipeline();
	LoadAssets();
	CreateAsyncContexts();
}

void D3D12Camera::LoadPipeline()
{
	UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif

	ComPtr<IDXGIFactory4> factory;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

	if (m_useWarpDevice)
	{
		ComPtr<IDXGIAdapter> warpAdapter;
		ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			warpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_device)
		));
	}
	else
	{
		ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(factory.Get(), &hardwareAdapter);

		ThrowIfFailed(D3D12CreateDevice(
			hardwareAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_device)
		));
	}

	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDese = {};
	queueDese.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDese.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(m_device->CreateCommandQueue(&queueDese, IID_PPV_ARGS(&m_commandQueue)));
	NAME_D3D12_OBJECT(m_commandQueue);

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.Width = m_width;
	swapChainDesc.Height = m_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(factory->CreateSwapChainForHwnd(
		m_commandQueue.Get(),
		Win32Application::GetHwnd(),
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	));

	ThrowIfFailed(factory->MakeWindowAssociation(Win32Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

	ThrowIfFailed(swapChain.As(&m_swapChain));
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
	m_swapChainEvent = m_swapChain->GetFrameLatencyWaitableObject();

	// Create descriptor heaps.
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = FrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

		// Describe and create a shader resource view (SRV) and unordered access view (UAV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC srvUavHeapDesc = {};
		srvUavHeapDesc.NumDescriptors = DescriptorCount;
		srvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(m_device->CreateDescriptorHeap(&srvUavHeapDesc, IID_PPV_ARGS(&m_srvUavHeap)));
		NAME_D3D12_OBJECT(m_srvUavHeap);

		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_srvUavDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	}

	// Create frame resources.
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

		for (UINT n = 0; n < FrameCount; n++)
		{
			ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
			m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorSize);

			NAME_D3D12_OBJECT_INDEXED(m_renderTargets, n);

			ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n])));
		}
	}
}

void D3D12Camera::LoadAssets()
{
	// Create the root signatures.
	{
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

		if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
		{
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}

		// Graphics root signature.
		{
			CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
			CD3DX12_ROOT_PARAMETER1 rootParameters[GraphicsRootParametersCount];

			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
			rootParameters[GraphicsRootCVB].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_ALL);
			rootParameters[GraphicsRootSRVTable].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);

			CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;
			ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
			ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
			NAME_D3D12_OBJECT(m_rootSignature);
		}

		// Compute root signature.
		{
			CD3DX12_DESCRIPTOR_RANGE1 ranges[2];
			CD3DX12_ROOT_PARAMETER1 rootParameters[ComputeRootParametersCount];

			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);
			rootParameters[ComputeRootCBV].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC);
			rootParameters[ComputeRootSRVTable].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_ALL);
			rootParameters[ComputeRootUAVTable].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_ALL);

			CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC computeRootSignatureDesc;
			computeRootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr);

			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;
			ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&computeRootSignatureDesc, featureData.HighestVersion, &signature, &error));
			ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_computeRootSignature)));
			NAME_D3D12_OBJECT(m_computeRootSignature);
		}
	}

	// Create the pipeline states, which includes compiling and loading shaders.
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> geometryShader;
		ComPtr<ID3DBlob> pixelShader;
		ComPtr<ID3DBlob> computeShader;

#if defined(_DEBUG)
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif

		ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"ParticleDraw.hlsl").c_str(), nullptr, nullptr, "VSParticleDraw", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"ParticleDraw.hlsl").c_str(), nullptr, nullptr, "GSParticleDraw", "gs_5_0", compileFlags, 0, &geometryShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"ParticleDraw.hlsl").c_str(), nullptr, nullptr, "PSParticleDraw", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"NBodyGravityCS.hlsl").c_str(), nullptr, nullptr, "CSMain", "cs_5_0", compileFlags, 0, &computeShader, nullptr));

		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		};

		// Describe the blend and depth states.
		CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

		CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc(D3D12_DEFAULT);
		depthStencilDesc.DepthEnable = FALSE;
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
		psoDesc.GS = CD3DX12_SHADER_BYTECODE(geometryShader.Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = blendDesc;
		psoDesc.DepthStencilState = depthStencilDesc;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		psoDesc.SampleDesc.Count = 1;

		ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
		NAME_D3D12_OBJECT(m_pipelineState);

		// Describe and create the compute pipeline state object (PSO).
		D3D12_COMPUTE_PIPELINE_STATE_DESC computePsoDesc = {};
		computePsoDesc.pRootSignature = m_computeRootSignature.Get();
		computePsoDesc.CS = CD3DX12_SHADER_BYTECODE(computeShader.Get());

		ThrowIfFailed(m_device->CreateComputePipelineState(&computePsoDesc, IID_PPV_ARGS(&m_computeState)));
		NAME_D3D12_OBJECT(m_computeState);
	}

	// Create the command list.
	ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[m_frameIndex].Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)));
	NAME_D3D12_OBJECT(m_commandList);

	CreateVertexBuffer();
	CreateParticleBuffers();

	ComPtr<ID3D12Resource> constantBufferCSUpload;

	// Create the compute shader's constant buffer.
	{
		const UINT bufferSize = sizeof(ConstantBufferCS);

		ThrowIfFailed(m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_constantBufferCS)
		));

		ThrowIfFailed(m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constantBufferCSUpload)
		));

		NAME_D3D12_OBJECT(m_constantBufferCS);

		ConstantBufferCS constantBufferCS = {};
		constantBufferCS.param[0] = ParticleCount;
		constantBufferCS.param[1] = int(ceil(ParticleCount / 128.f));
		constantBufferCS.paramf[0] = 0.1f;
		constantBufferCS.paramf[1] = 1.0f;

		D3D12_SUBRESOURCE_DATA computeCBData = {};
		computeCBData.pData = reinterpret_cast<UINT8*>(&constantBufferCS);
		computeCBData.RowPitch = bufferSize;
		computeCBData.SlicePitch = computeCBData.RowPitch;

		UpdateSubresources<1>(m_commandList.Get(), m_constantBufferCS.Get(), constantBufferCSUpload.Get(), 0, 0, 1, &computeCBData);
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_constantBufferCS.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
	}

	// Create the geometry shader's constant buffer.
	{
		const UINT constantBufferGSSize = sizeof(ConstantBufferGS) * FrameCount;

		ThrowIfFailed(m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(constantBufferGSSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_constantBufferGS)
		));
		NAME_D3D12_OBJECT(m_constantBufferGS);

		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_constantBufferGS->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBufferGSData)));
		ZeroMemory(m_pConstantBufferGSData, constantBufferGSSize);
	}

	// Close the command list and execute it to begin the initial GPU setup.
	ThrowIfFailed(m_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Create the synchronization objects and wait until assets have been uploaded to the GPU.
	{
		ThrowIfFailed(m_device->CreateFence(m_renderContextFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_renderContextFence)));
		m_renderContextFenceValue++;

		m_renderContextFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_renderContextFenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}
		WaitForRenderContext();
	}
}

void D3D12Camera::CreateVertexBuffer()
{
	std::vector<ParticleVertex> vertices;
	vertices.resize(ParticleCount);
	for (UINT i = 0; i < ParticleCount; i++)
	{
		vertices[i].color = XMFLOAT4(1.f, 1.f, 0.2f, 1.f);
	}
	const UINT bufferSize = ParticleCount * sizeof(ParticleVertex);

	ThrowIfFailed(m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer)
	));

	ThrowIfFailed(m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertexBufferUpload)
	));

	NAME_D3D12_OBJECT(m_vertexBuffer);

	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = reinterpret_cast<INT8*>(&vertices[0]);
	vertexData.RowPitch = bufferSize;
	vertexData.SlicePitch = vertexData.RowPitch;

	UpdateSubresources<1>(m_commandList.Get(), m_vertexBuffer.Get(), m_vertexBufferUpload.Get(), 0, 0, 1, &vertexData);
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = static_cast<UINT>(bufferSize);
	m_vertexBufferView.StrideInBytes = sizeof(ParticleVertex);
}


// Random percent value, frome -1 to 1.
float D3D12Camera::RandomPercent()
{
	float ret = static_cast<float>((rand() % 10000) - 5000);
	return ret / 5000.f;
}

void D3D12Camera::LoadParticles(_Out_writes_(numParticles) Particle* pParticles, const XMFLOAT3& center, const XMFLOAT4& velocity, float spread, UINT numParticles)
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
		pParticles[i].position.w = 10000.f * 10000.f;

		pParticles[i].velocity = velocity;
	}
}

void D3D12Camera::CreateParticleBuffers()
{
	std::vector<Particle> data;
	data.resize(ParticleCount);
	const UINT dataSize = ParticleCount * sizeof(Particle);

	float centerSpread = ParticleSpread * 0.50f;
	LoadParticles(&data[0], XMFLOAT3(centerSpread, 0, 0), XMFLOAT4(0, 0, -20, 1 / 100000000.0f), ParticleSpread, ParticleCount / 2);
	LoadParticles(&data[ParticleCount / 2], XMFLOAT3(-centerSpread, 0, 0), XMFLOAT4(0, 0, 20, 1 / 100000000.0f), ParticleSpread, ParticleCount / 2);

	D3D12_HEAP_PROPERTIES defaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(dataSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	D3D12_RESOURCE_DESC uploadBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(dataSize);

	for (UINT index = 0; index < ThreadCount; index++)
	{
		ThrowIfFailed(m_device->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_particleBuffer0[index])));

		ThrowIfFailed(m_device->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_particleBuffer1[index])));

		ThrowIfFailed(m_device->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&uploadBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_particleBuffer0Upload[index])));

		ThrowIfFailed(m_device->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&uploadBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_particleBuffer1Upload[index])));

		NAME_D3D12_OBJECT_INDEXED(m_particleBuffer0, index);
		NAME_D3D12_OBJECT_INDEXED(m_particleBuffer1, index);
	
		D3D12_SUBRESOURCE_DATA particleData = {};
		particleData.pData = reinterpret_cast<UINT8*>(&data[0]);
		particleData.RowPitch = dataSize;
		particleData.SlicePitch = particleData.RowPitch;
		
		UpdateSubresources<1>(m_commandList.Get(), m_particleBuffer0[index].Get(), m_particleBuffer0Upload[index].Get(), 0, 0, 1, &particleData);
		UpdateSubresources<1>(m_commandList.Get(), m_particleBuffer1[index].Get(), m_particleBuffer1Upload[index].Get(), 0, 0, 1, &particleData);
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_particleBuffer0[index].Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_particleBuffer1[index].Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = ParticleCount;
		srvDesc.Buffer.StructureByteStride = sizeof(Particle);
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

		CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle0(m_srvUavHeap->GetCPUDescriptorHandleForHeapStart(), SrvParticlePosVelo0 + index, m_srvUavDescriptorSize);
		CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle1(m_srvUavHeap->GetCPUDescriptorHandleForHeapStart(), SrvParticlePosVelo1 + index, m_srvUavDescriptorSize);
		m_device->CreateShaderResourceView(m_particleBuffer0[index].Get(), &srvDesc, srvHandle0);
		m_device->CreateShaderResourceView(m_particleBuffer1[index].Get(), &srvDesc, srvHandle1);

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = ParticleCount;
		uavDesc.Buffer.StructureByteStride = sizeof(Particle);
		uavDesc.Buffer.CounterOffsetInBytes = 0;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

		CD3DX12_CPU_DESCRIPTOR_HANDLE uavHandle0(m_srvUavHeap->GetCPUDescriptorHandleForHeapStart(), UavParticlePosVelo0 + index, m_srvUavDescriptorSize);
		CD3DX12_CPU_DESCRIPTOR_HANDLE uavHandle1(m_srvUavHeap->GetCPUDescriptorHandleForHeapStart(), UavParticlePosVelo1 + index, m_srvUavDescriptorSize);
		m_device->CreateUnorderedAccessView(m_particleBuffer0[index].Get(), nullptr, &uavDesc, uavHandle0);
		m_device->CreateUnorderedAccessView(m_particleBuffer1[index].Get(), nullptr, &uavDesc, uavHandle1);
	}
}

void D3D12Camera::CreateAsyncContexts()
{
	for (UINT threadIndex = 0; threadIndex < ThreadCount; ++threadIndex)
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = { D3D12_COMMAND_LIST_TYPE_COMPUTE, 0, D3D12_COMMAND_QUEUE_FLAG_NONE };
		ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_computeCommandQueue[threadIndex])));
		ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&m_computeAllocator[threadIndex])));
		ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, m_computeAllocator[threadIndex].Get(), nullptr, IID_PPV_ARGS(&m_computeCommandList[threadIndex])));
		ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_SHARED, IID_PPV_ARGS(&m_threadFences[threadIndex])));

		m_threadFenceEvents[threadIndex] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_threadFenceEvents[threadIndex] == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}

		m_threadData[threadIndex].pContext = this;
		m_threadData[threadIndex].threadIndex = threadIndex;

		m_threadHandles[threadIndex] = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(ThreadProc),
			reinterpret_cast<void*>(&m_threadData[threadIndex]),
			CREATE_SUSPENDED,
			nullptr
		);

		ResumeThread(m_threadHandles[threadIndex]);

	}
}

void D3D12Camera::OnUpdate()
{
	WaitForSingleObjectEx(m_swapChainEvent, 100, FALSE);

	m_timer.Tick(NULL);
	m_camera.Update(static_cast<float>(m_timer.GetElapsedSeconds()));

	ConstantBufferGS constantBufferGS = {};
	XMStoreFloat4x4(&constantBufferGS.worldViewProjection, XMMatrixMultiply(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(0.8f, m_aspectRatio, 1.f, 5000.f)));
	XMStoreFloat4x4(&constantBufferGS.inverseView, XMMatrixInverse(nullptr, m_camera.GetViewMatrix()));

	UINT8* destination = m_pConstantBufferGSData + sizeof(ConstantBufferGS) * m_frameIndex;
	memcpy(destination, &constantBufferGS, sizeof(ConstantBufferGS));
}

void D3D12Camera::OnRender()
{
	for (UINT n = 0; n < ThreadCount; ++n)
	{
		InterlockedExchange(&m_renderContextFenceValues[n], m_renderContextFenceValue);
	}

	for (UINT n = 0; n < ThreadCount; ++n)
	{
		UINT64 threadFenceValue = InterlockedGetValue(&m_threadFenceValues[n]);
		if (m_threadFences[n]->GetCompletedValue() < threadFenceValue)
		{
			ThrowIfFailed(m_commandQueue->Wait(m_threadFences[n].Get(), threadFenceValue));
		}
	}

	PIXBeginEvent(m_commandQueue.Get(), 0, L"Render");

	PopulateCommandList();

	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	PIXEndEvent(m_commandQueue.Get());

	ThrowIfFailed(m_swapChain->Present(1, 0));

	MoveToNextFrame();
}

void D3D12Camera::PopulateCommandList()
{
	ThrowIfFailed(m_commandAllocators[m_frameIndex]->Reset());

	ThrowIfFailed(m_commandList->Reset(m_commandAllocators[m_frameIndex].Get(), m_pipelineState.Get()));

}