#pragma once

#include "DXBase.h"
#include "ConstantBuffer.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class D3D12Render : public DXBase
{
public:
	D3D12Render(UINT width, UINT height, std::wstring name);

    virtual void OnInit();
    virtual void OnUpdate();
    virtual void OnRender();
    virtual void OnDestroy();

    virtual void SetGeometry(Geometry* geometry);
    virtual void SetConstantBuffer(ConstantBuffer* constantBufferData);

private:
    static const UINT FrameCount = 2;

    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    ComPtr<ID3D12DescriptorHeap> m_cbvHeap;
    UINT m_rtvDescriptorSize;

    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12PipelineState> m_pipelineState;
    ComPtr<ID3D12GraphicsCommandList> m_commandList;

    ComPtr<ID3D12Resource> m_vertexBufferUpload;
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    ComPtr<ID3D12Resource> m_indexBufferUpload;
    ComPtr<ID3D12Resource> m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    ComPtr<ID3D12Resource> m_constantBuffer;
    ComPtr<ID3D12Resource> m_constantBufferUpload;
    ConstantBuffer* m_pConstantBufferData;
    UINT8* m_pCbvDataBegin;

    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValue;

    Geometry* m_pGeometry;

    void LoadPipeline();
    void LoadAssets();
    void PopulateCommandList();
    void WaitForPreviousFrame();

    void CreateRootSignature();
    void CreatePipeline();
    void CreateCommandList();
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateConstantBuffer();
    void CreateFence();


};