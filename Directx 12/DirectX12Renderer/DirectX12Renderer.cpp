#pragma once
#include "Renderer.h"
#include "DirectX12RenderHelper.h"

struct SDescriptorHeap
{
	ComPtr<ID3D12DescriptorHeap> m_descHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpuDescStart;
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpuDescStart;
	UINT8* m_pData;
	size_t m_descSize;

	void Init(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = numDescriptors;
		desc.Type = type;
		desc.Flags = flags;
		ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_descHeap)));

		m_cpuDescStart = m_descHeap->GetCPUDescriptorHandleForHeapStart();
		m_gpuDescStart = m_descHeap->GetGPUDescriptorHandleForHeapStart();
		m_descSize = device->GetDescriptorHandleIncrementSize(type);
		m_pData = nullptr;

	}

	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT offset) const
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = m_cpuDescStart;
		handle.ptr += offset * m_descSize;
		return handle;
	}

	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(UINT64 offset) const
	{
		D3D12_GPU_DESCRIPTOR_HANDLE handle = m_gpuDescStart;
		handle.ptr += offset * m_descSize;
		return handle;
	}
};

struct SResource
{
	ComPtr<ID3D12Resource> m_resource;
};

struct SKernel
{
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<IDXGISwapChain3> m_swapChain;

	DescriptorHeap m_rtvHeap;
	Resource m_renderTargets[FrameCount];

	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValues[FrameCount];

	UINT m_width, m_height;
	SKernel()
	{
		m_rtvHeap = new SDescriptorHeap;
		for (UINT n = 0; n < FrameCount; ++n)
		{
			m_renderTargets[n] = new SResource;
		}
	}
};

struct SRootSignature
{
	ComPtr<ID3D12RootSignature> m_rootSignature;
	UINT cbvIndex;
	UINT srvIndex;
	UINT uavIndex;
};

struct SPipeline
{
	ComPtr<ID3D12PipelineState> m_pipeline;
	D3D12_PRIMITIVE_TOPOLOGY m_primitiveTopology;
};

struct SVertexSetup
{
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
};


void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = adapter.Detach();
}

void WaitForGPU(Kernel kernel)
{
	// Schedule a Signal command in the queue.
	ThrowIfFailed(kernel->m_commandQueue->Signal(kernel->m_fence.Get(), kernel->m_fenceValues[kernel->m_frameIndex]));

	// Wait until the fence has been processed.
	ThrowIfFailed(kernel->m_fence->SetEventOnCompletion(kernel->m_fenceValues[kernel->m_frameIndex], kernel->m_fenceEvent));
	WaitForSingleObjectEx(kernel->m_fenceEvent, INFINITE, FALSE);

	// Increment the fence value for the current frame.
	kernel->m_fenceValues[kernel->m_frameIndex]++;
}

void MoveToNextFrame(Kernel kernel)
{
	// Schedule a Signal command in the queue.
	const UINT64 currentFenceValue = kernel->m_fenceValues[kernel->m_frameIndex];
	ThrowIfFailed(kernel->m_commandQueue->Signal(kernel->m_fence.Get(), currentFenceValue));

	// Update the frame index.
	kernel->m_frameIndex = kernel->m_swapChain->GetCurrentBackBufferIndex();

	// If the next frame is not ready to be rendered yet, wait until it is ready.
	if (kernel->m_fence->GetCompletedValue() < kernel->m_fenceValues[kernel->m_frameIndex])
	{
		ThrowIfFailed(kernel->m_fence->SetEventOnCompletion(kernel->m_fenceValues[kernel->m_frameIndex], kernel->m_fenceEvent));
		WaitForSingleObjectEx(kernel->m_fenceEvent, INFINITE, FALSE);
	}

	// Set the fence value for the next frame.
	kernel->m_fenceValues[kernel->m_frameIndex] = currentFenceValue + 1;
}

D3D12_STATIC_SAMPLER_DESC Translate(StaticSampleDesc* staticSampleDesc)
{
	D3D12_STATIC_SAMPLER_DESC result;
	result.Filter = (D3D12_FILTER)staticSampleDesc->Filter,
	result.AddressU = (D3D12_TEXTURE_ADDRESS_MODE)staticSampleDesc->AddressU,
	result.AddressV = (D3D12_TEXTURE_ADDRESS_MODE)staticSampleDesc->AddressV,
	result.AddressW = (D3D12_TEXTURE_ADDRESS_MODE)staticSampleDesc->AddressW,
	result.MipLODBias = staticSampleDesc->MipLODBias,
	result.MaxAnisotropy = staticSampleDesc->MaxAnisotropy,
	result.ComparisonFunc = (D3D12_COMPARISON_FUNC)staticSampleDesc->ComparisonFunc,
	result.BorderColor = (D3D12_STATIC_BORDER_COLOR)staticSampleDesc->BorderColor,
	result.MinLOD = staticSampleDesc->MinLOD,
	result.MaxLOD = staticSampleDesc->MaxLOD,
	result.ShaderRegister = staticSampleDesc->ShaderRegister,
	result.RegisterSpace = staticSampleDesc->RegisterSpace,
	result.ShaderVisibility = (D3D12_SHADER_VISIBILITY)staticSampleDesc->ShaderVisibility;

	return result;
}
D3D12_RESOURCE_DESC Translate(ResourceDesc resourceDesc)
{
	D3D12_RESOURCE_DESC result = {};
	result.Width = resourceDesc.Width;
	result.Height = resourceDesc.Height;
	result.Dimension = (D3D12_RESOURCE_DIMENSION)resourceDesc.Dimension;
	result.Alignment = resourceDesc.Alignment;
	result.DepthOrArraySize = resourceDesc.DepthOrArraySize;
	result.MipLevels = resourceDesc.MipLevels;
	result.Format = (DXGI_FORMAT)resourceDesc.Format;
	result.SampleDesc.Count = resourceDesc.SampleDesc.Count;
	result.SampleDesc.Quality = resourceDesc.SampleDesc.Quality;
	result.Layout = (D3D12_TEXTURE_LAYOUT)resourceDesc.Layout;
	result.Flags = (D3D12_RESOURCE_FLAGS)resourceDesc.Flags;
	return result;
}
D3D12_SUBRESOURCE_DATA Translate(SubresourceData subresourceData)
{
	D3D12_SUBRESOURCE_DATA result;
	result.pData = subresourceData.pData;
	result.RowPitch = subresourceData.RowPitch;
	result.SlicePitch = subresourceData.SlicePitch;
	return result;
}

Kernel CreateKernel(UINT width, UINT height, bool useWarpDevice, HWND hwnd)
{
	Kernel kernel = new SKernel();
	UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ID3D12Debug* debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif

	ComPtr<IDXGIFactory4> factory;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));
	if (useWarpDevice)
	{
		ComPtr<IDXGIAdapter> warpAdapter;
		ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			warpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&kernel->m_device)
		));
	}
	else
	{
		ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(factory.Get(), &hardwareAdapter);

		ThrowIfFailed(D3D12CreateDevice(
			hardwareAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&kernel->m_device)
		));
	}

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	ThrowIfFailed(kernel->m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&kernel->m_commandQueue)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(factory->CreateSwapChainForHwnd(
		kernel->m_commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
		hwnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	));

	// This sample does not support fullscreen transitions.
	ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));
	ThrowIfFailed(swapChain.As(&kernel->m_swapChain));
	kernel->m_frameIndex = kernel->m_swapChain->GetCurrentBackBufferIndex();


	kernel->m_rtvHeap->Init(kernel->m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FrameCount, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	for (UINT n = 0; n < FrameCount; ++n)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = kernel->m_rtvHeap->GetCPUHandle(n);
		ThrowIfFailed(kernel->m_swapChain->GetBuffer(n, IID_PPV_ARGS(&kernel->m_renderTargets[n]->m_resource)));
		kernel->m_device->CreateRenderTargetView(kernel->m_renderTargets[n]->m_resource.Get(), nullptr, rtvHandle);
	}

	for (UINT i = 0; i < FrameCount; ++i)
	{
		ThrowIfFailed(kernel->m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&kernel->m_commandAllocators[i])));
	}
	ThrowIfFailed(kernel->m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, kernel->m_commandAllocators[kernel->m_frameIndex].Get(), nullptr, IID_PPV_ARGS(&kernel->m_commandList)));
	kernel->m_width = width; 
	kernel->m_height = height;
	return kernel;
}

RootSignature CreateRootSignature(Kernel kernel, UINT cbvCount, UINT srvCount, UINT uavCount, StaticSampleDesc* staticSampleDesc)
{
	RootSignature rootSignature = new SRootSignature();
	XD3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	XD3D12_DESCRIPTOR_RANGE1 tableRanges[3];
	XD3D12_ROOT_PARAMETER1 rootParameters[3];
	UINT index = 0;
	if (cbvCount + srvCount + uavCount == 0)
	{
		rootSignatureDesc.Init_1_1(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	}
	if (cbvCount)
	{
		tableRanges[index].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, cbvCount, 0, 0);
		rootSignature->cbvIndex = index++;
	}
	if (srvCount)
	{
		tableRanges[index].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, srvCount, 0, 0);
		rootSignature->srvIndex = index++;
	}
	if (uavCount)
	{
		tableRanges[index].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, uavCount, 0, 0);
		rootSignature->uavIndex = index++;
	}
	for (UINT i = 0; i < index; ++i)
	{
		rootParameters[i].InitAsDescriptorTable(1, &tableRanges[i], D3D12_SHADER_VISIBILITY_ALL);
	}
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	if (staticSampleDesc == nullptr)
	{
		rootSignatureDesc.Init_1_1(index, rootParameters, 0, nullptr, rootSignatureFlags);
	}
	else
	{
		D3D12_STATIC_SAMPLER_DESC samplerDesc;
		samplerDesc = Translate(staticSampleDesc);
		rootSignatureDesc.Init_1_1(index, rootParameters, 1, &samplerDesc, rootSignatureFlags);
	}

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
	ThrowIfFailed(kernel->m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature->m_rootSignature)));


	return rootSignature;
}

Pipeline CreateGraphicsPipeline(Kernel kernel, GraphicsPipelineStateDesc& graphicsPipelineStateDesc)
{

	Pipeline pipeline = new SPipeline();
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = graphicsPipelineStateDesc.RootSignature->m_rootSignature.Get();

	D3D12_INPUT_ELEMENT_DESC inputElementsDesc[16];
	UINT offset = 0;
	for (UINT i = 0; i < graphicsPipelineStateDesc.InputLayout.NumElements; ++i)
	{

		inputElementsDesc[i].SemanticName = graphicsPipelineStateDesc.InputLayout.pInputElementDescs[i].SemanticName;
		inputElementsDesc[i].SemanticIndex = 0;
		inputElementsDesc[i].Format = (DXGI_FORMAT)graphicsPipelineStateDesc.InputLayout.pInputElementDescs[i].Format;
		inputElementsDesc[i].InputSlot = 0;
		inputElementsDesc[i].AlignedByteOffset = offset;
		inputElementsDesc[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		inputElementsDesc[i].InstanceDataStepRate = 0;

		offset += FormatToBits[graphicsPipelineStateDesc.InputLayout.pInputElementDescs[i].Format] / 8;
	}
	psoDesc.InputLayout = { inputElementsDesc, graphicsPipelineStateDesc.InputLayout.NumElements };

	ComPtr<ID3DBlob> vertexShader;
	ComPtr<ID3DBlob> hullShader;
	ComPtr<ID3DBlob> domainShader;
	ComPtr<ID3DBlob> geometryShader;
	ComPtr<ID3DBlob> pixelShader;
	{
		ThrowIfFailed(D3DCompileFromFile(
			graphicsPipelineStateDesc.VS.filePath, nullptr, nullptr, 
			graphicsPipelineStateDesc.VS.entryPoint.c_str(), "vs_5_0", 
			graphicsPipelineStateDesc.VS.flags, 0, &vertexShader, nullptr));
		psoDesc.VS = XD3D12_SHADER_BYTECODE(vertexShader.Get());
	}
	if (graphicsPipelineStateDesc.HS.active)
	{
		ThrowIfFailed(D3DCompileFromFile(
			graphicsPipelineStateDesc.HS.filePath, nullptr, nullptr,
			graphicsPipelineStateDesc.HS.entryPoint.c_str(), "hs_5_0",
			graphicsPipelineStateDesc.HS.flags, 0, &hullShader, nullptr));
		psoDesc.HS = XD3D12_SHADER_BYTECODE(hullShader.Get());
	}
	if (graphicsPipelineStateDesc.DS.active)
	{
		ThrowIfFailed(D3DCompileFromFile(
			graphicsPipelineStateDesc.DS.filePath, nullptr, nullptr,
			graphicsPipelineStateDesc.DS.entryPoint.c_str(), "ds_5_0",
			graphicsPipelineStateDesc.DS.flags, 0, &domainShader, nullptr));
		psoDesc.DS = XD3D12_SHADER_BYTECODE(domainShader.Get());
	}
	if (graphicsPipelineStateDesc.GS.active)
	{
		ThrowIfFailed(D3DCompileFromFile(
			graphicsPipelineStateDesc.GS.filePath, nullptr, nullptr,
			graphicsPipelineStateDesc.GS.entryPoint.c_str(), "gs_5_0",
			graphicsPipelineStateDesc.GS.flags, 0, &geometryShader, nullptr));
		psoDesc.GS = XD3D12_SHADER_BYTECODE(geometryShader.Get());
	}
	if (graphicsPipelineStateDesc.PS.active)
	{
		ThrowIfFailed(D3DCompileFromFile(
			graphicsPipelineStateDesc.PS.filePath, nullptr, nullptr,
			graphicsPipelineStateDesc.PS.entryPoint.c_str(), "ps_5_0",
			graphicsPipelineStateDesc.PS.flags, 0, &pixelShader, nullptr));
		psoDesc.PS = XD3D12_SHADER_BYTECODE(pixelShader.Get());
	}

	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.RasterizerState.CullMode = (D3D12_CULL_MODE)(graphicsPipelineStateDesc.CullMode);
	psoDesc.RasterizerState.DepthClipEnable = true;
	psoDesc.RasterizerState.MultisampleEnable = true;
	psoDesc.BlendState = XD3D12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = XD3D12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;

	psoDesc.PrimitiveTopologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)((graphicsPipelineStateDesc.PrimitiveTopologyType + 3) >> 1);
	pipeline->m_primitiveTopology = (D3D12_PRIMITIVE_TOPOLOGY)(graphicsPipelineStateDesc.PrimitiveTopologyType + 1);
	ThrowIfFailed(kernel->m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipeline->m_pipeline)));

	return pipeline;
}

VertexSetup CreateVertexSetup(Kernel kernel, const void* pVertexData, UINT vertexSize, UINT vertexBufferStride, const void* pIndexData, UINT indexSize, UINT indexBufferStride)
{
	VertexSetup vertexSetup = new SVertexSetup();
	
	ID3D12Resource* vertexBuffer;
	ID3D12Resource* vertexBufferUpload;
	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Buffer(vertexSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer)
	));
	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Buffer(vertexSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBufferUpload)
	));
	D3D12_SUBRESOURCE_DATA vertexData = { pVertexData, vertexSize, vertexSize };
	UpdateSubresources<1>(kernel->m_commandList.Get(), vertexBuffer, vertexBufferUpload, 0, 0, 1, &vertexData);
	kernel->m_commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(vertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	vertexSetup->m_vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexSetup->m_vertexBufferView.StrideInBytes = vertexBufferStride;
	vertexSetup->m_vertexBufferView.SizeInBytes = vertexSize;

	if (pIndexData == nullptr) {
		return vertexSetup;
	}

	ID3D12Resource* indexBuffer;
	ID3D12Resource* indexBufferUpload;
	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Buffer(indexSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&indexBuffer)
	));
	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Buffer(indexSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBufferUpload)
	));
	D3D12_SUBRESOURCE_DATA indexData = { pIndexData, indexSize, indexSize };
	UpdateSubresources<1>(kernel->m_commandList.Get(), indexBuffer, indexBufferUpload, 0, 0, 1, &indexData);
	kernel->m_commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(indexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	vertexSetup->m_indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	vertexSetup->m_indexBufferView.SizeInBytes = indexSize;
	vertexSetup->m_indexBufferView.Format = indexBufferStride == sizeof(UINT16) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

	return vertexSetup;
}

DescriptorHeap CreateDepthStencil(Kernel kernel)
{
	DescriptorHeap dsvHeap = new SDescriptorHeap();
	dsvHeap->Init(kernel->m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	ID3D12Resource* depthStencilBuffer;
	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, kernel->m_width, kernel->m_height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(&depthStencilBuffer)
	));

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	kernel->m_device->CreateDepthStencilView(depthStencilBuffer, &depthStencilDesc, dsvHeap->GetCPUHandle(0));

	return dsvHeap;
}

DescriptorHeap CreateConstantBuffer(Kernel kernel, void* bufferData, UINT bufferSize)
{
	DescriptorHeap cbvHeap = new SDescriptorHeap();
	ID3D12Resource* constantBuffer;
	cbvHeap->Init(kernel->m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	
	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Buffer((bufferSize + 255) & ~255),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constantBuffer)
	));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constantBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (bufferSize + 255) & ~255;
	kernel->m_device->CreateConstantBufferView(&cbvDesc, cbvHeap->GetCPUHandle(0));
	ZeroMemory(bufferData, bufferSize);

	XD3D12_RANGE readRange(0, 0);
	ThrowIfFailed(constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&cbvHeap->m_pData)));
	memcpy(cbvHeap->m_pData, bufferData, bufferSize);

	return cbvHeap;
}

DescriptorHeap CreateTexture(Kernel kernel)
{
	DescriptorHeap texture = new SDescriptorHeap();
	texture->Init(kernel->m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	ID3D12Resource* textureBuffer;
	ID3D12Resource* textureBufferUpload;
	D3D12_RESOURCE_DESC textureDesc;
	D3D12_SUBRESOURCE_DATA textureData;

	{
		ResourceDesc tempTextureDesc;
		SubresourceData tempTextureData;
		LoadImageDataFromFile(tempTextureDesc, tempTextureData, L"D:\\Pandora\\Directx 12\\x64\\Debug\\braynzar.jpg");
		textureDesc = Translate(tempTextureDesc);
		textureData = Translate(tempTextureData);
	}
	
	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&textureBuffer)
	));
	UINT64 textureBufferUploadSize;
	kernel->m_device->GetCopyableFootprints(&textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &textureBufferUploadSize);
	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Buffer(textureBufferUploadSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureBufferUpload)
	));
	UpdateSubresources<1>(kernel->m_commandList.Get(), textureBuffer, textureBufferUpload, 0, 0, 1, &textureData);
	kernel->m_commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(textureBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	kernel->m_device->CreateShaderResourceView(textureBuffer, &srvDesc, texture->GetCPUHandle(0));
	
	return texture;
}

void UpdateConstantBuffer(DescriptorHeap cbvHeap, void* bufferData, UINT bufferSize)
{
	memcpy(cbvHeap->m_pData, bufferData, bufferSize);
}

void EndOnInit(Kernel kernel)
{
	ThrowIfFailed(kernel->m_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { kernel->m_commandList.Get() };
	kernel->m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	ThrowIfFailed(kernel->m_device->CreateFence(kernel->m_fenceValues[kernel->m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&kernel->m_fence)));
	kernel->m_fenceValues[kernel->m_frameIndex]++;
	kernel->m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (kernel->m_fenceEvent == nullptr)
	{
		ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}
	WaitForGPU(kernel);
}

void EndOnRender(Kernel kernel)
{
	ID3D12CommandList* ppCommandLists[] = { kernel->m_commandList.Get() };
	kernel->m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	ThrowIfFailed(kernel->m_swapChain->Present(1, 0));
	MoveToNextFrame(kernel);
}

void EndOnDestory(Kernel kernel)
{
	WaitForGPU(kernel);
	CloseHandle(kernel->m_fenceEvent);
}

void Reset(Kernel kernel, Pipeline pipeline)
{
	ThrowIfFailed(kernel->m_commandAllocators[kernel->m_frameIndex]->Reset());
	ThrowIfFailed(kernel->m_commandList->Reset(kernel->m_commandAllocators[kernel->m_frameIndex].Get(), pipeline->m_pipeline.Get()));
}

void BeginRender(Kernel kernel, DescriptorHeap dsvHeap, const float* clearColor)
{

	D3D12_VIEWPORT vp = { 0.0f, 0.0f, static_cast<float>(kernel->m_width), static_cast<float>(kernel->m_height), 0.0f, 1.0f };
	D3D12_RECT sr = { 0, 0, static_cast<LONG>(kernel->m_width), static_cast<LONG>(kernel->m_height) };
	kernel->m_commandList->RSSetViewports(1, &vp);
	kernel->m_commandList->RSSetScissorRects(1, &sr);
	kernel->m_commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(kernel->m_renderTargets[kernel->m_frameIndex]->m_resource.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(kernel->m_rtvHeap->GetCPUHandle(kernel->m_frameIndex));
	if (dsvHeap == nullptr)
	{
		kernel->m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
		kernel->m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	}
	else
	{
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle(dsvHeap->GetCPUHandle(0));
		kernel->m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		kernel->m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		kernel->m_commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
}

void SetGraphicsRootSignature(Kernel kernel, RootSignature rootSignature)
{
	kernel->m_commandList->SetGraphicsRootSignature(rootSignature->m_rootSignature.Get());
}

void SetPipeline(Kernel kernel, Pipeline pipeline)
{
	kernel->m_commandList->SetPipelineState(pipeline->m_pipeline.Get());
	kernel->m_commandList->IASetPrimitiveTopology(pipeline->m_primitiveTopology);
}

void SetVertexSetup(Kernel kernel, VertexSetup vertexSetup)
{
	if (vertexSetup->m_vertexBufferView.BufferLocation)
	{
		kernel->m_commandList->IASetVertexBuffers(0, 1, &vertexSetup->m_vertexBufferView);
	}
	if (vertexSetup->m_indexBufferView.BufferLocation)
	{
		kernel->m_commandList->IASetIndexBuffer(&vertexSetup->m_indexBufferView);
	}
	
}

void SetConstantBuffer(Kernel kernel, DescriptorHeap heap)
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { heap->m_descHeap.Get() };
	kernel->m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	kernel->m_commandList->SetGraphicsRootDescriptorTable(0, heap->GetGPUHandle(0));
}

void SetShaderResource(Kernel kernel, DescriptorHeap heap)
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { heap->m_descHeap.Get() };
	kernel->m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	kernel->m_commandList->SetGraphicsRootDescriptorTable(1, heap->GetGPUHandle(0));
}

void Draw(Kernel kernel, UINT StartVertexLocation, UINT VertexCountPerInstance)
{
	kernel->m_commandList->DrawInstanced(VertexCountPerInstance, 1, StartVertexLocation, 0);
}

void DrawIndexed(Kernel kernel, UINT StartIndexLocation, UINT IndexCountPerInstance)
{
	kernel->m_commandList->DrawIndexedInstanced(IndexCountPerInstance, 1, StartIndexLocation, 0, 0);
}

void DrawIndexInstanced(Kernel kernel, UINT StartIndexLocation, UINT IndexCountPerInstance, UINT StartInstanceLocation, UINT InstanceCount)
{
	kernel->m_commandList->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, 0, StartInstanceLocation);
}

void EndRender(Kernel kernel)
{
	kernel->m_commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(kernel->m_renderTargets[kernel->m_frameIndex]->m_resource.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	ThrowIfFailed(kernel->m_commandList->Close());
}