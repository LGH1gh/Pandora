#pragma once

#include "BaseApp.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

class D3D12HalloDX12 : public BaseApp {
public:
	D3D12HalloDX12(UINT width, UINT height, std::wstring name);

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();

private:
	static const UINT FrameCount;
	static const UINT TextureWidth;
	static const UINT TextureHeight;
	static const UINT TexturePixelSize;


	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT2 uv;
	};

	struct SceneConstantBuffer
	{
		XMFLOAT4 offset;
	};


	// Pipeline objects
	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12Resource> m_renderTarget[FrameCount];
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
	ComPtr<ID3D12CommandAllocator> m_bundleAllocators[FrameCount];
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	// ComPtr<ID3D12DescriptorHeap> m_srvHeap;
	ComPtr<ID3D12DescriptorHeap> m_cbvHeap;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<ID3D12GraphicsCommandList> m_bundle;
	UINT m_rtvDescriptorSize;

	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	
	ComPtr<ID3D12Resource> m_constantBuffer;
	SceneConstantBuffer m_constantBufferData;
	UINT* m_pCbvDataBegin;

	// ComPtr<ID3D12Resource> m_texture;

	ComPtr<ID3D12Resource> m_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
	
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValues[FrameCount];
	


	void LoadPipeline();
	void LoadAssets();
	std::vector<UINT8> GenerateTextureData();
	void PopulateCommandList();
	void WaitForGPU();
	void MoveToNextFrame();
};