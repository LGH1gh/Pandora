#pragma once
#include "Renderer.h"

#include <DirectXMath.h>

#include <d3d12.h>
#include "d3dx12.h"

#include <dxgi1_4.h>
#include <d3dcompiler.h>

#pragma comment ( lib, "d3d12.lib")
#pragma comment ( lib, "dxgi.lib")
#pragma comment ( lib, "d3dcompiler.lib" )
#pragma comment ( lib, "dxguid.lib" )

using namespace DirectX;
using Microsoft::WRL::ComPtr;

static const DXGI_FORMAT FORMATS[] =
{
	DXGI_FORMAT_UNKNOWN,

	DXGI_FORMAT_R8_UNORM,
	DXGI_FORMAT_R8_UINT,

	DXGI_FORMAT_R8G8_UNORM,
	DXGI_FORMAT_R8G8_UINT,

	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R8G8B8A8_UINT,

	DXGI_FORMAT_R16_UNORM,
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R16_FLOAT,

	DXGI_FORMAT_R16G16_UNORM,
	DXGI_FORMAT_R16G16_UINT,
	DXGI_FORMAT_R16G16_FLOAT,

	DXGI_FORMAT_R16G16B16A16_UNORM,
	DXGI_FORMAT_R16G16B16A16_UINT,
	DXGI_FORMAT_R16G16B16A16_FLOAT,

	DXGI_FORMAT_R32_UINT,
	DXGI_FORMAT_R32_FLOAT,

	DXGI_FORMAT_R32G32_UINT,
	DXGI_FORMAT_R32G32_FLOAT,

	DXGI_FORMAT_R32G32B32A32_UINT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,

	DXGI_FORMAT_BC1_UNORM,
	DXGI_FORMAT_BC2_UNORM,
	DXGI_FORMAT_BC3_UNORM,
	DXGI_FORMAT_BC4_UNORM,
	DXGI_FORMAT_BC5_UNORM,
	DXGI_FORMAT_BC7_UNORM,

	DXGI_FORMAT_D16_UNORM,
};

struct SDescriptorHeap
{
	ComPtr<ID3D12DescriptorHeap> m_descHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpuDescStart;
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpuDescStart;
	size_t m_descSize;

	void Init(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = type;
		desc.NumDescriptors = numDescriptors;
		desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_descHeap)));

		m_cpuDescStart = m_descHeap->GetCPUDescriptorHandleForHeapStart();
		m_gpuDescStart = m_descHeap->GetGPUDescriptorHandleForHeapStart();
		m_descSize = device->GetDescriptorHandleIncrementSize(type);
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

struct SCommandAllocator
{
	ComPtr<ID3D12CommandAllocator> commandAllocator;

	void Init(ID3D12Device* device)
	{
		ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
	}
};

struct SContext
{
	ComPtr<ID3D12GraphicsCommandList> commandList;
};

struct SBlendState
{
	D3D12_BLEND_DESC blendDesc;
};

struct SDevice
{
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<IDXGISwapChain3> swapChain;
	D3D_FEATURE_LEVEL featureLevel;

	Context mainContext;
	CommandAllocator commandAllocators[FrameCount];

	DescriptorHeap srvHeap;
	DescriptorHeap rtvHeap;
	DescriptorHeap dsvHeap;
	DescriptorHeap samplerHeap;

	UINT msaaSupportMask;

	BlendState blendState;

	UINT frameIndex;
	HANDLE fenceEvent;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceValues[FrameCount];

	RenderPass backBufferRenderPass;
	Texture backBuffer[FrameCount];
	RenderSetup backBufferRenderSetup;

	SDevice() {
		mainContext = new SContext();
		for (size_t n = 0; n < FrameCount; n++)
		{
			commandAllocators[n] = new SCommandAllocator();
		}
		srvHeap = new SDescriptorHeap();
		rtvHeap = new SDescriptorHeap();
		dsvHeap = new SDescriptorHeap();
		samplerHeap = new SDescriptorHeap();
		blendState = new SBlendState();
	}
};

struct STexture
{
	UINT width;
	UINT height;
	UINT depth;
	UINT slices;
	UINT mipLevels;
	TextureType m_Type;
	ImageFormat m_Format;
};

struct SRenderPass
{
	DXGI_FORMAT depthFormat;
	UINT colorTargetCount;
	DXGI_FORMAT colorFormats[1];
	RenderPassFlags flags;
	UINT msaaSamples;
};

struct SRootSignature
{
	ComPtr<ID3D12RootSignature> rootSignature;
};

struct SResourceTable
{
	UINT m_Offset;
	UINT m_Size;
};

struct SSamplerTable
{
	UINT m_Offset;
	UINT m_Count;
};

struct SRenderSetup
{
	ComPtr<ID3D12Resource> renderTargets[FrameCount];
};

struct SPipeline
{
	ComPtr<ID3D12PipelineState> pipeline;
	D3D12_PRIMITIVE_TOPOLOGY primitiveTopology;
};

struct SVertexSetup
{
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
};

struct SBuffer
{
	ComPtr<ID3D12Resource> buffer;
	UINT size;
	HeapType heapType;
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

Device CreateDevice(DeviceParams& params, HWND hwnd)
{
	Device device = new SDevice();
	UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
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

	if (params.useWarpDevice)
	{
		ComPtr<IDXGIAdapter> warpAdapter;
		ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			warpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&device->device)
		));
	}
	else
	{
		ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(factory.Get(), &hardwareAdapter);

		ThrowIfFailed(D3D12CreateDevice(
			hardwareAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&device->device)
		));
	}

	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	ThrowIfFailed(device->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&device->commandQueue)));

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.Width = params.width;
	swapChainDesc.Height = params.height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(factory->CreateSwapChainForHwnd(
		device->commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
		hwnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	));

	// This sample does not support fullscreen transitions.
	ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

	ThrowIfFailed(swapChain.As(&device->swapChain));
	device->frameIndex = device->swapChain->GetCurrentBackBufferIndex();

	//device->srvHeap = new SDescriptorHeap();
	//device->srvHeap->Init(device->device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 64, true);
	device->rtvHeap->Init(device->device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FrameCount, false);
	//device->dsvHeap->Init(device->device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 16, false);
	//device->samplerHeap->Init(device->device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 16, true);


	device->backBufferRenderPass = CreateRenderPass(device, IMAGE_FORMAT_R8G8B8A8_UNORM, IMAGE_FORMAT_UNKNOWN, FINAL_PRESENT);
	device->backBufferRenderSetup = CreateRenderSetup(device);
	for (UINT n = 0; n < FrameCount; n++)
	{
		device->commandAllocators[n]->Init(device->device.Get());
	}

	{
		ThrowIfFailed(device->device->CreateFence(device->fenceValues[device->frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&device->fence)));
		device->fenceValues[device->frameIndex]++;
		device->fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (device->fenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}
		WaitForGPU(device);

	}
	return device;
}

RenderSetup CreateRenderSetup(Device device)
{
	RenderSetup setup = new SRenderSetup();
	for (UINT n = 0; n < FrameCount; n++)
	{
		ThrowIfFailed(device->swapChain->GetBuffer(n, IID_PPV_ARGS(&setup->renderTargets[n])));
		device->device->CreateRenderTargetView(setup->renderTargets[n].Get(), nullptr, device->rtvHeap->GetCPUHandle(n));
	}
	return setup;
}

BlendState CreateBlendState(Blend src, Blend dst, BlendOperator mode, UINT mask, bool alphaToCoverageEnable)
{
	BlendState blendState = new SBlendState();
	bool blendEnble = (src != BLEND_ONE || dst != BLEND_ZERO);

	blendState->blendDesc.AlphaToCoverageEnable = alphaToCoverageEnable;
	blendState->blendDesc.IndependentBlendEnable = false;

	static const D3D12_BLEND blend_factors[] =
	{
		D3D12_BLEND_ZERO,
		D3D12_BLEND_ONE,
		D3D12_BLEND_SRC_COLOR,
		D3D12_BLEND_INV_SRC_COLOR,
		D3D12_BLEND_SRC_ALPHA,
		D3D12_BLEND_INV_SRC_ALPHA,
		D3D12_BLEND_DEST_COLOR,
		D3D12_BLEND_INV_DEST_COLOR,
		D3D12_BLEND_DEST_ALPHA,
		D3D12_BLEND_INV_DEST_ALPHA,
	};

	static const D3D12_BLEND_OP blend_modes[] =
	{
		D3D12_BLEND_OP_ADD,
		D3D12_BLEND_OP_SUBTRACT,
		D3D12_BLEND_OP_REV_SUBTRACT,
		D3D12_BLEND_OP_MIN,
		D3D12_BLEND_OP_MAX,
	};

	for (int i = 0; i < 8; ++i)
	{
		blendState->blendDesc.RenderTarget[i].BlendEnable = blendEnble;

		blendState->blendDesc.RenderTarget[i].BlendOp = blend_modes[mode];
		blendState->blendDesc.RenderTarget[i].BlendOpAlpha = blend_modes[mode];
		blendState->blendDesc.RenderTarget[i].SrcBlend = blend_factors[src];
		blendState->blendDesc.RenderTarget[i].SrcBlendAlpha = blend_factors[src];
		blendState->blendDesc.RenderTarget[i].DestBlend = blend_factors[dst];
		blendState->blendDesc.RenderTarget[i].DestBlendAlpha = blend_factors[dst];

		blendState->blendDesc.RenderTarget[i].RenderTargetWriteMask = (UINT8)mask;
	}

	return blendState;
}

RootSignature CreateRootSignature(SDevice* device)
{
	RootSignature rootSignature = new SRootSignature();

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
	ThrowIfFailed(device->device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature->rootSignature)));

	return rootSignature;
}

Blob CreateShaderFromFile(ShaderType shaderType, LPCWSTR filePath, std::string entryPoint, UINT flags)
{
	Blob blob = new SBlob();
	ID3DBlob* shader;
	switch (shaderType) {
	case SHADER_TYPE_VERTEX_SHADER:
		ThrowIfFailed(D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint.c_str(), "vs_5_0", flags, 0, &shader, nullptr));
		break;
	case SHADER_TYPE_PIXEL_SHADER:
		ThrowIfFailed(D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint.c_str(), "ps_5_0", flags, 0, &shader, nullptr));
		break;
	case SHADER_TYPE_HULL_SHADER:
		ThrowIfFailed(D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint.c_str(), "hs_5_0", flags, 0, &shader, nullptr));
		break;
	case SHADER_TYPE_DOMAIN_SHADER:
		ThrowIfFailed(D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint.c_str(), "ds_5_0", flags, 0, &shader, nullptr));
		break;
	case SHADER_TYPE_COMPUTE_SHADER:
		ThrowIfFailed(D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint.c_str(), "cs_5_0", flags, 0, &shader, nullptr));
		break;
	default:
		ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		return nullptr;
	}
	blob->address = shader->GetBufferPointer();
	blob->size = shader->GetBufferSize();

	return blob;
}

RenderPass CreateRenderPass(Device device, ImageFormat colorFormat, ImageFormat depthFormat, RenderPassFlags flags, UINT msaaSampler)
{
	RenderPass renderPass = new SRenderPass();
	renderPass->depthFormat = FORMATS[depthFormat];
	renderPass->colorTargetCount = 1;
	renderPass->colorFormats[0] = FORMATS[colorFormat];
	renderPass->msaaSamples = msaaSampler;
	renderPass->flags = flags;

	return renderPass;
}

Pipeline CreateGraphicsPipeline(SDevice* device, PipelineParams& params)
{
	Pipeline pipeline = new SPipeline();

	static const DXGI_FORMAT dxgi_format[] =
	{
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R32G32B32A32_UINT,
		DXGI_FORMAT_R16G16B16A16_UINT,

		DXGI_FORMAT_R32G32B32_FLOAT,
		DXGI_FORMAT_R32G32B32_UINT,

		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_R16G16_FLOAT,
		DXGI_FORMAT_R32G32_UINT,
		DXGI_FORMAT_R16G16_UINT,

		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_R32_UINT,
	};

	static const UINT dxgi_format_size[] =
	{
		16, 8, 16, 8,
		12, 12,
		8, 4, 8, 4,
		4, 4
	};

	D3D12_INPUT_ELEMENT_DESC inputElementDesc[16];
	UINT offsets[4] = {};

	for (UINT n = 0; n < params.attributeCount; ++n)
	{
		UINT stream = params.attributes[n].stream;
		AttributeFormat format = params.attributes[n].format;

		inputElementDesc[n].SemanticName = params.attributes[n].name;
		inputElementDesc[n].SemanticIndex = 0;
		inputElementDesc[n].Format = dxgi_format[format];
		inputElementDesc[n].InputSlot = stream;
		inputElementDesc[n].AlignedByteOffset = offsets[stream];
		inputElementDesc[n].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		inputElementDesc[n].InstanceDataStepRate = 0;

		offsets[stream] += dxgi_format_size[format];
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = params.rootSignature->rootSignature.Get();
	psoDesc.InputLayout = { inputElementDesc, params.attributeCount };
	psoDesc.VS.BytecodeLength = params.vs.size;
	psoDesc.VS.pShaderBytecode = params.vs.address;
	psoDesc.PS.BytecodeLength = params.ps.size;
	psoDesc.PS.pShaderBytecode = params.ps.address;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.RasterizerState.CullMode = (D3D12_CULL_MODE)(params.cullMode + 1);
	psoDesc.RasterizerState.DepthClipEnable = true;
	psoDesc.RasterizerState.MultisampleEnable = true;
	psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xF;
	if (params.blendState)
	{
		psoDesc.BlendState = params.blendState->blendDesc;
	}
	psoDesc.DepthStencilState.DepthEnable = params.depthTest;
	psoDesc.DepthStencilState.DepthWriteMask = params.depthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
	psoDesc.DepthStencilState.DepthFunc = (D3D12_COMPARISON_FUNC)(params.depthFunc + 1);

	const UINT32 renderTargetCount = params.renderPass->colorTargetCount;
	psoDesc.NumRenderTargets = renderTargetCount;
	memcpy(psoDesc.RTVFormats, params.renderPass->colorFormats, renderTargetCount * sizeof(DXGI_FORMAT));
	psoDesc.DSVFormat = params.renderPass->depthFormat;

	psoDesc.SampleDesc.Count = params.renderPass->msaaSamples;
	psoDesc.SampleMask = UINT_MAX;

	psoDesc.PrimitiveTopologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)((params.primitiveTopologyType + 3) >> 1);
	pipeline->primitiveTopology = (D3D12_PRIMITIVE_TOPOLOGY)(params.primitiveTopologyType + 1);

	ThrowIfFailed(device->device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipeline->pipeline)));
	ThrowIfFailed(device->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, device->commandAllocators[device->frameIndex]->commandAllocator.Get(), pipeline->pipeline.Get(), IID_PPV_ARGS(&device->mainContext->commandList)));
	
	return pipeline;
}

VertexSetup CreateVertexSetup(SBuffer* vertexBuffer, UINT vertexBufferStride, SBuffer* indexBuffer, UINT indexBufferStride)
{
	VertexSetup vertexSetup = new SVertexSetup();
	if (indexBuffer)
	{
		vertexSetup->indexBufferView.BufferLocation = indexBuffer->buffer->GetGPUVirtualAddress();
		vertexSetup->indexBufferView.SizeInBytes = indexBuffer->size;
		vertexSetup->indexBufferView.Format = indexBufferStride == sizeof(UINT16) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	}

	if (vertexBuffer)
	{
		vertexSetup->vertexBufferView.BufferLocation = vertexBuffer->buffer->GetGPUVirtualAddress();
		vertexSetup->vertexBufferView.SizeInBytes = vertexBuffer->size;
		vertexSetup->vertexBufferView.StrideInBytes = vertexBufferStride;
	}

	return vertexSetup;
}

Buffer CreateBuffer(Device device, const BufferParams& params)
{
	Buffer buffer = new SBuffer();;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = (D3D12_HEAP_TYPE)(params.heapType + 1);
	D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = params.size;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_RESOURCE_STATES resourceState = (D3D12_RESOURCE_STATES)params.resourceState;

	ID3D12Resource* d3dBuffer = nullptr;
	ThrowIfFailed(device->device->CreateCommittedResource(&heapProp, flags, &desc, resourceState, nullptr, IID_PPV_ARGS(&d3dBuffer)));

	buffer->buffer = d3dBuffer;
	buffer->size = params.size;
	buffer->heapType = params.heapType;

	return buffer;
}

void WaitForGPU(SDevice* device)
{
	// Schedule a Signal command in the queue.
	ThrowIfFailed(device->commandQueue->Signal(device->fence.Get(), device->fenceValues[device->frameIndex]));

	// Wait until the fence has been processed.
	ThrowIfFailed(device->fence->SetEventOnCompletion(device->fenceValues[device->frameIndex], device->fenceEvent));
	WaitForSingleObjectEx(device->fenceEvent, INFINITE, FALSE);

	// Increment the fence value for the current frame.
	device->fenceValues[device->frameIndex]++;
}

void MoveToNextFrame(SDevice* device)
{
	// Schedule a Signal command in the queue.
	const UINT64 currentFenceValue = device->fenceValues[device->frameIndex];
	ThrowIfFailed(device->commandQueue->Signal(device->fence.Get(), currentFenceValue));

	// Update the frame index.
	device->frameIndex = device->swapChain->GetCurrentBackBufferIndex();

	// If the next frame is not ready to be rendered yet, wait until it is ready.
	if (device->fence->GetCompletedValue() < device->fenceValues[device->frameIndex])
	{
		ThrowIfFailed(device->fence->SetEventOnCompletion(device->fenceValues[device->frameIndex], device->fenceEvent));
		WaitForSingleObjectEx(device->fenceEvent, INFINITE, FALSE);
	}

	// Set the fence value for the next frame.
	device->fenceValues[device->frameIndex] = currentFenceValue + 1;
}

void Subresource(SDevice* device, SBuffer* destinationResource, SBuffer* intermediate, SubresourceParams& params)
{

	D3D12_SUBRESOURCE_DATA data;
	data.pData = params.pData;
	data.RowPitch = params.rowPitch;
	data.SlicePitch = params.slicePitch;
	UpdateSubresources<1>(device->mainContext->commandList.Get(), destinationResource->buffer.Get(), intermediate->buffer.Get(), 0, 0, 1, &data);
	device->mainContext->commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(destinationResource->buffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
}

void Reset(SDevice* device, SPipeline* pipeline)
{
	ThrowIfFailed(device->commandAllocators[device->frameIndex]->commandAllocator->Reset());
	ThrowIfFailed(device->mainContext->commandList->Reset(device->commandAllocators[device->frameIndex]->commandAllocator.Get(), pipeline->pipeline.Get()));
}


void BeginRenderPass(SDevice* device, const DeviceParams& params, const float* clearColor)
{
	D3D12_VIEWPORT vp = { 0.0f, 0.0f, static_cast<float>(params.width), static_cast<float>(params.height) };
	D3D12_RECT sr = { 0, 0, static_cast<float>(params.width), static_cast<float>(params.height) };
	device->mainContext->commandList->RSSetViewports(1, &vp);
	device->mainContext->commandList->RSSetScissorRects(1, &sr);
	device->mainContext->commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(device->backBufferRenderSetup->renderTargets[device->frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(device->rtvHeap->GetCPUHandle(device->frameIndex));
	device->mainContext->commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
	device->mainContext->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}



void ResetCommands(SDevice* device, SPipeline* pipeline)
{
	
}

void SetGraphicsRootSignature(SDevice* device, SRootSignature* rootSignature)
{
	device->mainContext->commandList->SetGraphicsRootSignature(rootSignature->rootSignature.Get());
}

void SetPipeline(SDevice* device, SPipeline* pipeline)
{
	device->mainContext->commandList->IASetPrimitiveTopology(pipeline->primitiveTopology);
}

void SetVertexSetup(SDevice* device, SVertexSetup* vertexSetup)
{
	if (vertexSetup->indexBufferView.BufferLocation)
	{
		device->mainContext->commandList->IASetIndexBuffer(&vertexSetup->indexBufferView);
	}
	if (vertexSetup->vertexBufferView.BufferLocation)
	{
		device->mainContext->commandList->IASetVertexBuffers(0, 1, &vertexSetup->vertexBufferView);
	}
}

void Draw(SDevice* device, UINT start, UINT count)
{
	device->mainContext->commandList->DrawInstanced(count, 1, start, 0);
}

void DrawIndexed(SDevice* device, UINT start, UINT count)
{
	device->mainContext->commandList->DrawIndexedInstanced(count, 1, start, 0, 0);
}

void DrawIndexInstanced(SDevice* device, UINT start, UINT count, UINT startInstance, UINT instanceCount)
{
	device->mainContext->commandList->DrawIndexedInstanced(count, instanceCount, start, 0, startInstance);
}

void EndRenderPass(SDevice* device)
{
	device->mainContext->commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(device->backBufferRenderSetup->renderTargets[device->frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void ExecuteCommand(SDevice* device)
{
	ThrowIfFailed(device->mainContext->commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { device->mainContext->commandList.Get() };
	device->commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

void Present(SDevice* device)
{
	device->swapChain->Present(0, 0);
	MoveToNextFrame(device);
}

Context GetContext(SDevice* device)
{
	return device->mainContext;
}

RenderPass GetRenderPass(SDevice* device)
{
	return device->backBufferRenderPass;
}

void Destory(SDevice* device)
{
	CloseHandle(device->fenceEvent);
}