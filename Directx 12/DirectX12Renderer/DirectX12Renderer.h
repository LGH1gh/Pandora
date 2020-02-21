#pragma once

#include <d3d12.h>
#include "d3dx12.h"

#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Renderer.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

static const UINT FrameCount = 2;

struct DescriptorHeap
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

struct CommandAllocator
{
	ComPtr<ID3D12CommandAllocator> commandAllocator;

	void Init(ID3D12Device* device)
	{
		ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
	}
};

struct BlendState
{
	D3D12_BLEND_DESC blendDesc;
};

struct Texture
{
	ComPtr<ID3D12Resource> texture;
	
};

struct RootSignature
{
	ComPtr<ID3D12RootSignature> rootSignature;
};

class DirectX12Renderer : public Renderer
{
public:
	DirectX12Renderer(UINT width, UINT height, std::wstring name);
protected:
	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();
protected:

	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<IDXGISwapChain3> m_swapChain;

	ComPtr<ID3D12GraphicsCommandList> m_graphicsCommandList;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator[FrameCount];

	ComPtr<CommandAllocator> m_commandAllocators[FrameCount];

	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValues[FrameCount];

	ComPtr<DescriptorHeap> m_srvHeap;
	ComPtr<DescriptorHeap> m_rtvHeap;
	ComPtr<DescriptorHeap> m_dsvHeap;
	ComPtr<DescriptorHeap> m_samplerHeap;
	
	BlendState m_blendState;
	RootSignature m_rootSignature;

	void CreateBlendState(Blend src, Blend dst, BlendOperator mode, UINT mask, bool alphaToCoverageEnable);
	void CreateRootSignature(const Blob& blob);
	void CreatePipeline(PipelineParams* params);

	void LoadPipeline();
	void LoadAssets();
	void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);

	void WaitForGPU();
	void MoveToNextFrame();
};