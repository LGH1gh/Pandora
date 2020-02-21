#pragma once

#include "DXBase.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "StepTimer.h"

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

    virtual void CreateGeometry(Geometry* geometry);
    virtual void SetConstantBuffer(ConstantBuffer* constantBufferData);
    virtual void SetVertexShader(std::wstring fullPath, LPCSTR funcName);
    virtual void SetPixelShader(std::wstring fullPath, LPCSTR funcName);

private:
    static const UINT FrameCount;

    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
    ComPtr<ID3D12CommandAllocator> m_commandAllocator[FrameCount];
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    UINT m_rtvDescriptorSize;

    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12PipelineState> m_pipelineState;
    ComPtr<ID3D12GraphicsCommandList> m_commandList;

    ComPtr<ID3D12Resource> m_vertexBuffer;
    ComPtr<ID3D12Resource> m_vertexBufferUpload;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    ComPtr<ID3D12Resource> m_indexBuffer;
    ComPtr<ID3D12Resource> m_indexBufferUpload;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

    HANDLE m_swapChainEvent;

    ComPtr<ID3D12DescriptorHeap> m_srvHeap;
    ComPtr<ID3D12DescriptorHeap> m_uavHeap;
    ComPtr<ID3D12DescriptorHeap> m_samplerHeap;

    ComPtr<ID3D12DescriptorHeap> m_cbvHeap;
    ComPtr<ID3D12Resource> m_constantBuffer;
    UINT8* m_pCbvDataBegin;

    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValues[FrameCount];

    Geometry* m_pGeometry;
    ConstantBuffer* m_pConstantBufferData;
    std::wstring m_VertexShaderFullPath, m_PixelShaderFullPath;
    LPCSTR m_VertexShaderFuncName, m_PixelShaderFuncName;
    Camera m_camera;

    void LoadPipeline();
    void LoadAssets();
    void PopulateCommandList();
    void MoveToNextFrame();
    void WaitForGpu();


    void CreateRootSignature();
    void CreatePipeline();
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateConstantBuffer();
    void CreateFence();
};