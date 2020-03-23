#pragma once
#include "Renderer.h"
#include "DirectX12RenderHelper.h"

std::array<const XD3D12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers()
{
	const XD3D12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const XD3D12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const XD3D12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const XD3D12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const XD3D12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const XD3D12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy


	return  { pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}

struct SResourceHeap
{
	ComPtr<ID3D12DescriptorHeap> m_descHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpuDescStart;
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpuDescStart;
	UINT8* m_pData;
	size_t m_descSize;
	ComPtr<ID3D12Resource> m_resource;

	void Init(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags, size_t size = -1)
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

struct SKernel
{
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<IDXGISwapChain3> m_swapChain;

	ResourceHeap m_rtvHeap;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];

	UINT m_frameIndex = 0;
	ComPtr<ID3D12Fence> m_renderContextFence;
	UINT64 m_renderContextFenceValue = 0;
	HANDLE m_renderContextFenceEvent;
	UINT64 m_frameFenceValues[FrameCount] = {};

	UILayer m_uiLayer;

	UINT m_width, m_height;
	SKernel()
	{
		m_rtvHeap = new SResourceHeap();
	}

	void Destroy();
};

struct SComputeKernel
{
	ComPtr<ID3D12CommandAllocator> m_computeAllocator;
	ComPtr<ID3D12CommandQueue> m_computeCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> m_computeCommandList;
};

struct SUILayer
{
	std::vector<TextBlock> m_textBlock;

	ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;
	ComPtr<ID3D11On12Device> m_d3d11On12Device;
	ComPtr<IDWriteFactory> m_dwriteFactory;
	//ComPtr<ID2D1Factory3> m_d2dFactory;
	ComPtr<ID2D1Device2> m_d2dDevice;
	ComPtr<ID2D1DeviceContext2> m_d2dDeviceContext;
	ComPtr<ID3D11Resource> m_wrappedRenderTargets[FrameCount];
	ComPtr<ID2D1Bitmap1> m_d2dRenderTargets[FrameCount];

	ComPtr<ID2D1SolidColorBrush> m_textBrush;
	ComPtr<IDWriteTextFormat> m_textFormat;
	SUILayer(Kernel kernel, HWND hwnd)
	{
		UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		D2D1_FACTORY_OPTIONS d2dFactoryOptions = {};
#if defined(_DEBUG) || defined(DBG)
		// Enable the D2D debug layer.
		d2dFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

		// Enable the D3D11 debug layer.
		d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		ComPtr<ID3D11Device> d3d11Device;
		ID3D12CommandQueue* ppCommandQueues[] = { kernel->m_commandQueue.Get() };
		ThrowIfFailed(D3D11On12CreateDevice(
			kernel->m_device.Get(),
			d3d11DeviceFlags,
			nullptr,
			0,
			reinterpret_cast<IUnknown**>(ppCommandQueues),
			_countof(ppCommandQueues),
			0,
			&d3d11Device,
			&m_d3d11DeviceContext,
			nullptr
		));

		// Query the 11On12 device from the 11 device.
		ThrowIfFailed(d3d11Device.As(&m_d3d11On12Device));

#if defined(_DEBUG) || defined(DBG)
		// Filter a debug error coming from the 11on12 layer.
		ComPtr<ID3D12InfoQueue> infoQueue;
		if (SUCCEEDED(kernel->m_device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
		{
			// Suppress messages based on their severity level.
			D3D12_MESSAGE_SEVERITY severities[] =
			{
				D3D12_MESSAGE_SEVERITY_INFO,
			};

			// Suppress individual messages by their ID.
			D3D12_MESSAGE_ID denyIds[] =
			{
				// This occurs when there are uninitialized descriptors in a descriptor table, even when a
				// shader does not access the missing descriptors.
				D3D12_MESSAGE_ID_INVALID_DESCRIPTOR_HANDLE,
			};

			D3D12_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumSeverities = _countof(severities);
			filter.DenyList.pSeverityList = severities;
			filter.DenyList.NumIDs = _countof(denyIds);
			filter.DenyList.pIDList = denyIds;

			ThrowIfFailed(infoQueue->PushStorageFilter(&filter));
		}
#endif
		// Create D2D/DWrite components.
		{
			ComPtr<ID2D1Factory3> factory;
			ComPtr<IDXGIDevice> dxgiDevice;
			ThrowIfFailed(m_d3d11On12Device.As(&dxgiDevice));

			ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &d2dFactoryOptions, &factory));
			ThrowIfFailed(factory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice));
			ThrowIfFailed(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dDeviceContext));

			m_d2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
			ThrowIfFailed(m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_textBrush));

			ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_dwriteFactory));
		}

		float dpi = (float)GetDpiForWindow(hwnd);
		D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpi,
			dpi
		);

		for (UINT n = 0; n < FrameCount; ++n)
		{
			D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
			ThrowIfFailed(m_d3d11On12Device->CreateWrappedResource(
				kernel->m_renderTargets[n].Get(),
				&d3d11Flags,
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PRESENT,
				IID_PPV_ARGS(&m_wrappedRenderTargets[n])
			));

			ComPtr<IDXGISurface> surface;
			ThrowIfFailed(m_wrappedRenderTargets[n].As(&surface));
			ThrowIfFailed(m_d2dDeviceContext->CreateBitmapFromDxgiSurface(
				surface.Get(),
				&bitmapProperties,
				&m_d2dRenderTargets[n]
			));
		}

		ThrowIfFailed(m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_textBrush));
	}

	void Destroy();
};

struct TextBlock
{
	std::wstring content;
	D2D1_RECT_F layout;
	ComPtr<IDWriteTextFormat> format;
	TextBlock(Kernel kernel, FontDesc text)
	{
		ThrowIfFailed(kernel->m_uiLayer->m_dwriteFactory->CreateTextFormat(
			L"Verdana",
			NULL,
			(DWRITE_FONT_WEIGHT)text.FontWeight,
			(DWRITE_FONT_STYLE)text.FontStyle,
			(DWRITE_FONT_STRETCH)text.FontStretch,
			text.FontSize,
			L"en-us",
			&format
		));
		ThrowIfFailed(format->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)text.TextAlignment));
		ThrowIfFailed(format->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)text.ParagraphAlignment));
		ThrowIfFailed(format->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, text.LineSpacing, text.Baseline));

		content = text.Content;

		layout.bottom = text.Layout.bottom;
		layout.left = text.Layout.left;
		layout.right = text.Layout.right;
		layout.top = text.Layout.top;
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
	ThrowIfFailed(kernel->m_commandQueue->Signal(kernel->m_renderContextFence.Get(), kernel->m_renderContextFenceValue));

	// Wait until the fence has been processed.
	ThrowIfFailed(kernel->m_renderContextFence->SetEventOnCompletion(kernel->m_renderContextFenceValue, kernel->m_renderContextFenceEvent));
	WaitForSingleObjectEx(kernel->m_renderContextFenceEvent, INFINITE, FALSE);

	// Increment the fence value for the current frame.
	kernel->m_renderContextFenceValue++;
}

void MoveToNextFrame(Kernel kernel)
{
	// Schedule a Signal command in the queue.
	kernel->m_frameFenceValues[kernel->m_frameIndex] = kernel->m_renderContextFenceValue;
	ThrowIfFailed(kernel->m_commandQueue->Signal(kernel->m_renderContextFence.Get(), kernel->m_renderContextFenceValue));
	kernel->m_renderContextFenceValue++;

	kernel->m_frameIndex = kernel->m_swapChain->GetCurrentBackBufferIndex();

	if (kernel->m_renderContextFence->GetCompletedValue() < kernel->m_frameFenceValues[kernel->m_frameIndex])
	{
		ThrowIfFailed(kernel->m_renderContextFence->SetEventOnCompletion(kernel->m_frameFenceValues[kernel->m_frameIndex], kernel->m_renderContextFenceEvent));
		WaitForSingleObject(kernel->m_renderContextFenceEvent, INFINITE);
	}
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
D3D12_BLEND_DESC Translate(BlendDesc blendDesc)
{
	D3D12_BLEND_DESC result;
	result.AlphaToCoverageEnable = blendDesc.AlphaToCoverageEnable;
	result.IndependentBlendEnable = blendDesc.IndependentBlendEnable;
	for (UINT i = 0; i < 8; ++i)
	{
		result.RenderTarget[i].BlendEnable = blendDesc.RenderTarget[i].BlendEnable;
		result.RenderTarget[i].LogicOpEnable = blendDesc.RenderTarget[i].LogicOpEnable;
		result.RenderTarget[i].SrcBlend = (D3D12_BLEND)blendDesc.RenderTarget[i].SrcBlend;
		result.RenderTarget[i].DestBlend = (D3D12_BLEND)blendDesc.RenderTarget[i].DestBlend;
		result.RenderTarget[i].BlendOp = (D3D12_BLEND_OP)blendDesc.RenderTarget[i].BlendOp;
		result.RenderTarget[i].SrcBlendAlpha = (D3D12_BLEND)blendDesc.RenderTarget[i].SrcBlendAlpha;
		result.RenderTarget[i].DestBlendAlpha = (D3D12_BLEND)blendDesc.RenderTarget[i].DestBlendAlpha;
		result.RenderTarget[i].BlendOpAlpha = (D3D12_BLEND_OP)blendDesc.RenderTarget[i].BlendOpAlpha;
		result.RenderTarget[i].LogicOp = (D3D12_LOGIC_OP)blendDesc.RenderTarget[i].LogicOp;
		result.RenderTarget[i].RenderTargetWriteMask = blendDesc.RenderTarget[i].RenderTargetWriteMask;
	}
	return result;
}
D3D12_DEPTH_STENCILOP_DESC Translate(DepthStencilOpDesc depthStencilOpDesc)
{
	D3D12_DEPTH_STENCILOP_DESC result;
	result.StencilFailOp = (D3D12_STENCIL_OP)depthStencilOpDesc.StencilFailOp;
	result.StencilDepthFailOp = (D3D12_STENCIL_OP)depthStencilOpDesc.StencilDepthFailOp;
	result.StencilPassOp = (D3D12_STENCIL_OP)depthStencilOpDesc.StencilPassOp;
	result.StencilFunc = (D3D12_COMPARISON_FUNC)depthStencilOpDesc.StencilFunc;
	return result;
}
D3D12_DEPTH_STENCIL_DESC Translate(DepthStencilDesc depthStencilDesc)
{
	D3D12_DEPTH_STENCIL_DESC result;
	result.DepthEnable = depthStencilDesc.DepthEnable;
	result.DepthWriteMask = (D3D12_DEPTH_WRITE_MASK)depthStencilDesc.DepthWriteMask;
	result.DepthFunc = (D3D12_COMPARISON_FUNC)depthStencilDesc.DepthFunc;
	result.StencilEnable = depthStencilDesc.StencilEnable;
	result.StencilReadMask = depthStencilDesc.StencilReadMask;
	result.StencilWriteMask = depthStencilDesc.StencilWriteMask;
	result.FrontFace = Translate(depthStencilDesc.FrontFace);
	result.BackFace = Translate(depthStencilDesc.BackFace);
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
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;


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
		ThrowIfFailed(kernel->m_swapChain->GetBuffer(n, IID_PPV_ARGS(&kernel->m_renderTargets[n])));
		kernel->m_device->CreateRenderTargetView(kernel->m_renderTargets[n].Get(), nullptr, rtvHandle);
		ThrowIfFailed(kernel->m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&kernel->m_commandAllocators[n])));

	}

	ThrowIfFailed(kernel->m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, kernel->m_commandAllocators[kernel->m_frameIndex].Get(), nullptr, IID_PPV_ARGS(&kernel->m_commandList)));
	
	kernel->m_width = width; 
	kernel->m_height = height;
	kernel->m_uiLayer = new SUILayer(kernel, hwnd);

	return kernel;
}

ComputeKernel CreateComputeKernel(Kernel kernel)
{
	ComputeKernel computeKernel = new SComputeKernel();
	D3D12_COMMAND_QUEUE_DESC queueDesc = { D3D12_COMMAND_LIST_TYPE_COMPUTE, 0, D3D12_COMMAND_QUEUE_FLAG_NONE };
	ThrowIfFailed(kernel->m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&computeKernel->m_computeCommandQueue)));
	ThrowIfFailed(kernel->m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&computeKernel->m_computeAllocator)));
	ThrowIfFailed(kernel->m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, computeKernel->m_computeAllocator.Get(), nullptr, IID_PPV_ARGS(&computeKernel->m_computeCommandList)));
	
	return computeKernel;
}

RootSignature CreateRootSignature(Kernel kernel, UINT cbvCount, UINT srvCount, UINT uavCount)
{
	RootSignature rootSignature = new SRootSignature();
	
	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

	// This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

	if (FAILED(kernel->m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
	{
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}

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

	auto staticSamplers = GetStaticSamplers();
	rootSignatureDesc.Init_1_1(index, rootParameters, (UINT)staticSamplers.size(), staticSamplers.data(), rootSignatureFlags);


	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
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

	psoDesc.BlendState = Translate(graphicsPipelineStateDesc.BlendState);
	psoDesc.DepthStencilState = Translate(graphicsPipelineStateDesc.DepthStencilState);

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

Pipeline CreateComputePipeline(Kernel kernel, ComputePipelineStateDesc& computePipelineStateDesc)
{
	Pipeline pipeline = new SPipeline();
	D3D12_COMPUTE_PIPELINE_STATE_DESC computePsoDesc = {};
	computePsoDesc.pRootSignature = computePipelineStateDesc.RootSignature->m_rootSignature.Get();
	ComPtr<ID3DBlob> computeShader;
	{
		ThrowIfFailed(D3DCompileFromFile(
			computePipelineStateDesc.CS.filePath, nullptr, nullptr,
			computePipelineStateDesc.CS.entryPoint.c_str(), "cs_5_0",
			computePipelineStateDesc.CS.flags, 0, &computeShader, nullptr));
		computePsoDesc.CS = XD3D12_SHADER_BYTECODE(computeShader.Get());
	}
	ThrowIfFailed(kernel->m_device->CreateComputePipelineState(&computePsoDesc, IID_PPV_ARGS(&pipeline->m_pipeline)));
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

ResourceHeap CreateDepthStencilViewHeap(Kernel kernel)
{
	ResourceHeap dsvHeap = new SResourceHeap();
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

ResourceHeap CreateConstantBuffers(Kernel kernel, void* bufferData, UINT bufferSize, UINT bufferCount)
{
	ResourceHeap cbvHeap = new SResourceHeap;
	ID3D12Resource* constantBuffer;
	cbvHeap->Init(kernel->m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, bufferCount, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Buffer((UINT64)((bufferSize + 255) & ~255) * bufferCount),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constantBuffer)
	));
	XD3D12_RANGE readRange(0, 0);
	ThrowIfFailed(constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&cbvHeap->m_pData)));

	UINT64 cbvOffset = 0;
	for (UINT i = 0; i < bufferCount; ++i)
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = constantBuffer->GetGPUVirtualAddress() + cbvOffset;
		cbvDesc.SizeInBytes = ((bufferSize + 255) & ~255);
		cbvOffset += cbvDesc.SizeInBytes;
		kernel->m_device->CreateConstantBufferView(&cbvDesc, cbvHeap->GetCPUHandle(i));
	}
	
	cbvOffset = 0;
	for (UINT i = 0; i < bufferCount; ++i)
	{
		memcpy(cbvHeap->m_pData + (((bufferSize + 255) & ~255) * i), (char*)bufferData + (bufferSize * i), bufferSize);
	}
	cbvHeap->m_resource = constantBuffer;
	return cbvHeap;
}

ResourceHeap CreateConstantBuffer(Kernel kernel, void* bufferData, UINT bufferSize)
{
	ResourceHeap cbvHeap = new SResourceHeap();
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

	XD3D12_RANGE readRange(0, 0);
	ThrowIfFailed(constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&cbvHeap->m_pData)));
	memcpy(cbvHeap->m_pData, bufferData, bufferSize);
	cbvHeap->m_resource = constantBuffer;
	return cbvHeap;
}

ResourceHeap CreateTexture(Kernel kernel, LPCWSTR filename)
{
	ResourceHeap texture = new SResourceHeap();
	texture->Init(kernel->m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	ID3D12Resource* textureBuffer;
	ID3D12Resource* textureBufferUpload;
	D3D12_RESOURCE_DESC textureDesc;
	D3D12_SUBRESOURCE_DATA textureData;

	{
		ResourceDesc tempTextureDesc;
		SubresourceData tempTextureData;
		LoadImageDataFromFile(tempTextureDesc, tempTextureData, filename);
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

ResourceHeap CreateSkybox(Kernel kernel, LPCWSTR filename)
{
	ResourceHeap texture = new SResourceHeap();
	texture->Init(kernel->m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	ID3D12Resource* textureBuffer;
	ID3D12Resource* textureBufferUpload;
	std::vector<D3D12_SUBRESOURCE_DATA> textureDatas;
	std::unique_ptr<uint8_t[]> ddsData;
	ThrowIfFailed(LoadDDSTextureFromFile(
		kernel->m_device.Get(),
		filename,
		&textureBuffer,
		ddsData,
		textureDatas,
		SIZE_MAX
	));
	UINT textureBufferUploadSize = GetRequiredIntermediateSize(textureBuffer, 0, static_cast<UINT>(textureDatas.size()));
	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Buffer(textureBufferUploadSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureBufferUpload)
	));
	UpdateSubresources(kernel->m_commandList.Get(), textureBuffer, textureBufferUpload, 0, 0, static_cast<UINT>(textureDatas.size()), textureDatas.data());
	kernel->m_commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(textureBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureBuffer->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Texture2D.MipLevels = textureBuffer->GetDesc().MipLevels;
	kernel->m_device->CreateShaderResourceView(textureBuffer, &srvDesc, texture->GetCPUHandle(0));

	return texture;
}

void UpdateConstantBuffer(ResourceHeap cbvHeap, void* bufferData, UINT bufferSize)
{
	memcpy(cbvHeap->m_pData, bufferData, bufferSize);
}

void UpdateConstantBuffers(ResourceHeap cbvHeap, void* bufferData, UINT bufferSize, UINT bufferCount)
{
	for (UINT i = 0; i < bufferCount; ++i)
	{
		memcpy(cbvHeap->m_pData + (((bufferSize + 255) & ~255) * i), (char*)bufferData + (bufferSize * i), bufferSize);
	}
}


void EndOnInit(Kernel kernel)
{
	ThrowIfFailed(kernel->m_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { kernel->m_commandList.Get() };
	kernel->m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	{
		ThrowIfFailed(kernel->m_device->CreateFence(kernel->m_renderContextFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&kernel->m_renderContextFence)));
		kernel->m_renderContextFenceValue++;
		
		kernel->m_renderContextFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (kernel->m_renderContextFenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}
		WaitForGPU(kernel);
	}
}

void EndOnRender(Kernel kernel)
{
	ThrowIfFailed(kernel->m_swapChain->Present(1, 0));
	MoveToNextFrame(kernel);
}


void EndOnDestroy(Kernel kernel)
{
	WaitForGPU(kernel);
	kernel->Destroy();
	CloseHandle(kernel->m_renderContextFenceEvent);
}
void SKernel::Destroy()
{
	m_uiLayer->Destroy();
}
void SUILayer::Destroy()
{
	m_d2dDevice.Reset();
	m_d2dDeviceContext.Reset();
	m_textBrush.Reset();
	for (UINT n = 0; n < FrameCount; ++n)
	{
		m_wrappedRenderTargets[n].Reset();
		m_d2dRenderTargets[n].Reset();
	}
}

void Reset(Kernel kernel, Pipeline pipeline)
{
	ThrowIfFailed(kernel->m_commandAllocators[kernel->m_frameIndex]->Reset());
	ThrowIfFailed(kernel->m_commandList->Reset(kernel->m_commandAllocators[kernel->m_frameIndex].Get(), pipeline->m_pipeline.Get()));
}

void Reset(ComputeKernel computeKernel, Pipeline pipeline)
{
	ThrowIfFailed(computeKernel->m_computeAllocator->Reset());
	ThrowIfFailed(computeKernel->m_computeCommandList->Reset(computeKernel->m_computeAllocator.Get(), pipeline->m_pipeline.Get()));
}

void BeginPopulateGraphicsCommand(Kernel kernel, ResourceHeap dsvHeap, const float* clearColor)
{

	D3D12_VIEWPORT vp = { 0.0f, 0.0f, static_cast<float>(kernel->m_width), static_cast<float>(kernel->m_height), 0.0f, 1.0f };
	D3D12_RECT sr = { 0, 0, static_cast<LONG>(kernel->m_width), static_cast<LONG>(kernel->m_height) };
	kernel->m_commandList->RSSetViewports(1, &vp);
	kernel->m_commandList->RSSetScissorRects(1, &sr);
	kernel->m_commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(kernel->m_renderTargets[kernel->m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

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

void SetDescriptorHeaps(Kernel kernel, std::vector<ResourceHeap> heaps)
{
	std::vector<ID3D12DescriptorHeap*> ppHeaps;
	for (auto heap : heaps)
	{
		ppHeaps.push_back(heap->m_descHeap.Get());
	}
	kernel->m_commandList->SetDescriptorHeaps(ppHeaps.size(), ppHeaps.data());
}

void SetConstantBuffer(Kernel kernel, ResourceHeap heap)
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { heap->m_descHeap.Get() };
	kernel->m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	kernel->m_commandList->SetGraphicsRootDescriptorTable(0, heap->GetGPUHandle(0));
}

void SetConstantBuffer(Kernel kernel, ResourceHeap heap, UINT index)
{
	kernel->m_commandList->SetGraphicsRootDescriptorTable(0, heap->GetGPUHandle(index));
}

void SetShaderResource(Kernel kernel, ResourceHeap heap)
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { heap->m_descHeap.Get() };
	kernel->m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	kernel->m_commandList->SetGraphicsRootDescriptorTable(1, heap->GetGPUHandle(0));
}

void SetTexture(Kernel kernel, ResourceHeap heap)
{
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

void RenderText(Kernel kernel, std::vector<FontDesc> texts)
{
	for (auto text : texts)
	{
		kernel->m_uiLayer->m_textBlock.push_back(TextBlock(kernel, text));
	}

	D2D1_SIZE_F rtSize = kernel->m_uiLayer->m_d2dRenderTargets[kernel->m_frameIndex]->GetSize();

	kernel->m_uiLayer->m_d3d11On12Device->AcquireWrappedResources(kernel->m_uiLayer->m_wrappedRenderTargets[kernel->m_frameIndex].GetAddressOf(), 1);

	kernel->m_uiLayer->m_d2dDeviceContext->SetTarget(kernel->m_uiLayer->m_d2dRenderTargets[kernel->m_frameIndex].Get());
	kernel->m_uiLayer->m_d2dDeviceContext->BeginDraw();
	kernel->m_uiLayer->m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	for (auto text : kernel->m_uiLayer->m_textBlock)
	{
		kernel->m_uiLayer->m_d2dDeviceContext->DrawTextW(
			text.content.c_str(),
			(UINT32)text.content.size(),
			text.format.Get(),
			&text.layout,
			kernel->m_uiLayer->m_textBrush.Get()
		);
	}

	ThrowIfFailed(kernel->m_uiLayer->m_d2dDeviceContext->EndDraw());
	kernel->m_uiLayer->m_d3d11On12Device->ReleaseWrappedResources(kernel->m_uiLayer->m_wrappedRenderTargets[kernel->m_frameIndex].GetAddressOf(), 1);

	kernel->m_uiLayer->m_d3d11DeviceContext->Flush();
}

void EndPopulateGraphicsCommand(Kernel kernel)
{
	kernel->m_commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(kernel->m_renderTargets[kernel->m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	ThrowIfFailed(kernel->m_commandList->Close());
}

ResourceHeap CreateComputeBuffer(Kernel kernel, void* bufferData, UINT bufferCount, UINT bufferSize)
{
	ResourceHeap srvUavHeap = new SResourceHeap();
	srvUavHeap->Init(kernel->m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	ID3D12Resource* buffer;
	ID3D12Resource* bufferUpload;
	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Buffer(bufferSize * bufferCount, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&buffer)
	));

	ThrowIfFailed(kernel->m_device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Buffer(bufferSize * bufferCount),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&bufferUpload)
	));

	D3D12_SUBRESOURCE_DATA data = { reinterpret_cast<UINT8*>(bufferData), bufferSize* bufferCount, bufferSize* bufferCount };
	UpdateSubresources<1>(kernel->m_commandList.Get(), buffer, bufferUpload, 0, 0, 1, &data);
	kernel->m_commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = bufferCount;
	srvDesc.Buffer.StructureByteStride = bufferSize;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	kernel->m_device->CreateShaderResourceView(buffer, &srvDesc, srvUavHeap->GetCPUHandle(0));

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = bufferCount;
	uavDesc.Buffer.StructureByteStride = bufferSize;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	kernel->m_device->CreateUnorderedAccessView(buffer, nullptr, &uavDesc, srvUavHeap->GetCPUHandle(1));
	srvUavHeap->m_resource = buffer;
	return srvUavHeap;
}

void BeginPopulateComputeCommand(ComputeKernel computeKernel, ResourceHeap descriptorHeap)
{
	computeKernel->m_computeCommandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(descriptorHeap->m_resource.Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));
}

void SetComputePipeline(ComputeKernel computeKernel, Pipeline pipeline)
{
	computeKernel->m_computeCommandList->SetPipelineState(pipeline->m_pipeline.Get());
}

void SetComputeRootSignature(ComputeKernel computeKernel, RootSignature rootSignature)
{
	computeKernel->m_computeCommandList->SetComputeRootSignature(rootSignature->m_rootSignature.Get());
}

void SetComputeBuffer(ComputeKernel computeKernel, ResourceHeap descriptorHeap)
{
	ID3D12DescriptorHeap* ppHeaps[] = { descriptorHeap->m_descHeap.Get() };
	computeKernel->m_computeCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	computeKernel->m_computeCommandList->SetComputeRootDescriptorTable(1, descriptorHeap->GetGPUHandle(0));
	computeKernel->m_computeCommandList->SetComputeRootDescriptorTable(2, descriptorHeap->GetGPUHandle(1));
}

void SetConstantBuffer(ComputeKernel computeKernel, ResourceHeap descriptorHeap)
{
	ID3D12DescriptorHeap* ppHeaps[] = { descriptorHeap->m_descHeap.Get() };
	computeKernel->m_computeCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	computeKernel->m_computeCommandList->SetComputeRootDescriptorTable(0, descriptorHeap->GetGPUHandle(0));
}

void EndPopulateComputeCommand(ComputeKernel computeKernel, ResourceHeap descriptorHeap, const UINT dispatch[])
{
	computeKernel->m_computeCommandList->Dispatch(dispatch[0], dispatch[1], dispatch[2]);
	computeKernel->m_computeCommandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(descriptorHeap->m_resource.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
	ThrowIfFailed(computeKernel->m_computeCommandList->Close());

}

void ExecuteCommand(Kernel kernel)
{
	ID3D12CommandList* ppCommandLists[] = { kernel->m_commandList.Get() };
	kernel->m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}
void ExecuteCommand(ComputeKernel computeKernel)
{
	ID3D12CommandList* ppCommandLists[] = { computeKernel->m_computeCommandList.Get() };
	computeKernel->m_computeCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}


