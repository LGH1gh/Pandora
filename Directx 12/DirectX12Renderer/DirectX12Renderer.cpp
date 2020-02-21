#include "DirectX12Renderer.h"
DirectX12Renderer::DirectX12Renderer(UINT width, UINT height, std::wstring name) :
	Renderer(width, height, name)
{
}

void DirectX12Renderer::OnInit()
{
	LoadPipeline();
}

void DirectX12Renderer::LoadPipeline()
{
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
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = m_width;
    swapChainDesc.Height = m_height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    ThrowIfFailed(factory->CreateSwapChainForHwnd(
        m_commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
        Win32Application::GetHwnd(),
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain
    ));

    // This sample does not support fullscreen transitions.
    ThrowIfFailed(factory->MakeWindowAssociation(Win32Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

    ThrowIfFailed(swapChain.As(&m_swapChain));
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

    m_srvHeap->Init(m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 64, true);
    m_rtvHeap->Init(m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 16, false);
    m_dsvHeap->Init(m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 16, false);
    m_samplerHeap->Init(m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 16, true);

    
    for (UINT n = 0; n < FrameCount; n++)
    {
        m_commandAllocators[n]->Init(m_device.Get());
    }
    
    ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[m_frameIndex]->m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_graphicsCommandList)));
    m_graphicsCommandList->Close();
    {
        ThrowIfFailed(m_device->CreateFence(m_fenceValues[m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
        m_fenceValues[m_frameIndex]++;
        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_fenceEvent == nullptr)
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }
        WaitForGPU();

    }
}

void DirectX12Renderer::LoadAssets()
{
    CreateBlendState(BLEND_ONE, BLEND_ZERO, BLEND_OP_ADD, 0xF, false);
}

void DirectX12Renderer::WaitForGPU()
{
    // Schedule a Signal command in the queue.
    ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_frameIndex]));

    // Wait until the fence has been processed.
    ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent));
    WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

    // Increment the fence value for the current frame.
    m_fenceValues[m_frameIndex]++;
}

// Prepare to render the next frame.
void DirectX12Renderer::MoveToNextFrame()
{
    // Schedule a Signal command in the queue.
    const UINT64 currentFenceValue = m_fenceValues[m_frameIndex];
    ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

    // Update the frame index.
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

    // If the next frame is not ready to be rendered yet, wait until it is ready.
    if (m_fence->GetCompletedValue() < m_fenceValues[m_frameIndex])
    {
        ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent));
        WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
    }

    // Set the fence value for the next frame.
    m_fenceValues[m_frameIndex] = currentFenceValue + 1;
}

void DirectX12Renderer::CreateBlendState(Blend src, Blend dst, BlendOperator mode, UINT mask, bool alphaToCoverageEnable)
{
    bool blendEnble = (src != BLEND_ONE || dst != BLEND_ZERO);

    m_blendState.m_blendDesc.AlphaToCoverageEnable = alphaToCoverageEnable;
    m_blendState.m_blendDesc.IndependentBlendEnable = false;

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
        m_blendState.m_blendDesc.RenderTarget[i].BlendEnable          = blendEnble;

        m_blendState.m_blendDesc.RenderTarget[i].BlendOp              = blend_modes[mode];
        m_blendState.m_blendDesc.RenderTarget[i].BlendOpAlpha         = blend_modes[mode];
        m_blendState.m_blendDesc.RenderTarget[i].SrcBlend             = blend_factors[src];
        m_blendState.m_blendDesc.RenderTarget[i].SrcBlendAlpha        = blend_factors[src];
        m_blendState.m_blendDesc.RenderTarget[i].DestBlend            = blend_factors[dst];
        m_blendState.m_blendDesc.RenderTarget[i].DestBlendAlpha       = blend_factors[dst];

        m_blendState.m_blendDesc.RenderTarget[i].RenderTargetWriteMask = (UINT8)mask;
    }
}