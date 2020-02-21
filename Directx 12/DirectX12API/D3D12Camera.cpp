//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#include "stdafx.h"
#include "D3D12Camera.h"

// InterlockedCompareExchange returns the object's value if the 
// comparison fails.  If it is already 0, then its value won't 
// change and 0 will be returned.
#define InterlockedGetValue(object) InterlockedCompareExchange(object, 0, 0)

const float D3D12Camera::ParticleSpread00.0f;

D3D12Camera::D3D12Camera(UINT width, UINT height, std::wstring name) :
    BaseApp(width, height, name),
    m_frameIndex(0),
    m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
    m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)),
    m_rtvDescriptorSize(0),
    m_srvUavDescriptorSize(0),
    m_pConstantBufferGSData(nullptr),
    m_renderContextFenceValue(0),
    m_terminating(0),
    m_srvIndex{},
    m_frameFenceValues{}
{
    for (int n0; n < ThreadCount; n++)
    {
        m_renderContextFenceValues[n]0;
        m_threadFenceValues[n]0;
    }

    float sqRootNumAsyncContextssqrt(static_cast<float>(ThreadCount));
    m_heightInstancesstatic_cast<UINT>(ceil(sqRootNumAsyncContexts));
    m_widthInstancesstatic_cast<UINT>(ceil(sqRootNumAsyncContexts));

    if (m_widthInstances * (m_heightInstances - 1) >= ThreadCount)
    {
        m_heightInstances--;
    }
}

void D3D12Camera::OnInit()
{
    m_camera.Init({ 0.0f, 0.0f, 1500.0f });
    m_camera.SetMoveSpeed(250.0f);

    LoadPipeline();
    LoadAssets();
    CreateAsyncContexts();
}

// Load the rendering pipeline dependencies.
void D3D12Camera::LoadPipeline()
{
    UINT dxgiFactoryFlags0;

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
    D3D12_COMMAND_QUEUE_DESC queueDesc{};
    queueDesc.FlagsD3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.TypeD3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
    NAME_D3D12_OBJECT(m_commandQueue);

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.BufferCountFrameCount;
    swapChainDesc.Widthm_width;
    swapChainDesc.Heightm_height;
    swapChainDesc.FormatDXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsageDXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffectDXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count;
    swapChainDesc.FlagsDXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

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
    m_frameIndexm_swapChain->GetCurrentBackBufferIndex();
    m_swapChainEventm_swapChain->GetFrameLatencyWaitableObject();

    // Create descriptor heaps.
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
        rtvHeapDesc.NumDescriptorsFrameCount;
        rtvHeapDesc.TypeD3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.FlagsD3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

        // Describe and create a shader resource view (SRV) and unordered
        // access view (UAV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC srvUavHeapDesc{};
        srvUavHeapDesc.NumDescriptorsDescriptorCount;
        srvUavHeapDesc.TypeD3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvUavHeapDesc.FlagsD3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        ThrowIfFailed(m_device->CreateDescriptorHeap(&srvUavHeapDesc, IID_PPV_ARGS(&m_srvUavHeap)));
        NAME_D3D12_OBJECT(m_srvUavHeap);

        m_rtvDescriptorSizem_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        m_srvUavDescriptorSizem_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    // Create frame resources.
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV and a command allocator for each frame.
        for (UINT n0; n < FrameCount; n++)
        {
            ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
            m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, m_rtvDescriptorSize);

            NAME_D3D12_OBJECT_INDEXED(m_renderTargets, n);

            ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n])));
        }
    }
}

// Load the sample assets.
void D3D12Camera::LoadAssets()
{
    // Create the root signatures.
    {
        D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData{};

        // This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
        featureData.HighestVersionD3D_ROOT_SIGNATURE_VERSION_1_1;

        if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
        {
            featureData.HighestVersionD3D_ROOT_SIGNATURE_VERSION_1_0;
        }

        // Graphics root signature.
        {
            CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
            ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

            CD3DX12_ROOT_PARAMETER1 rootParameters[GraphicsRootParametersCount];
            rootParameters[GraphicsRootCBV].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_ALL);
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
            ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE);
            ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);

            CD3DX12_ROOT_PARAMETER1 rootParameters[ComputeRootParametersCount];
            rootParameters[ComputeRootCBV].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_ALL);
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
        // Enable better shader debugging with the graphics debugging tools.
        UINT compileFlagsD3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags0;
#endif

        // Load and compile shaders.
        ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"ParticleDraw.hlsl").c_str(), nullptr, nullptr, "VSParticleDraw", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
        ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"ParticleDraw.hlsl").c_str(), nullptr, nullptr, "GSParticleDraw", "gs_5_0", compileFlags, 0, &geometryShader, nullptr));
        ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"ParticleDraw.hlsl").c_str(), nullptr, nullptr, "PSParticleDraw", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));
        ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"NBodyGravityCS.hlsl").c_str(), nullptr, nullptr, "CSMain", "cs_5_0", compileFlags, 0, &computeShader, nullptr));

        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

        // Describe the blend and depth states.
        CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
        blendDesc.RenderTarget[0].BlendEnableTRUE;
        blendDesc.RenderTarget[0].SrcBlendD3D12_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlendD3D12_BLEND_ONE;
        blendDesc.RenderTarget[0].SrcBlendAlphaD3D12_BLEND_ZERO;
        blendDesc.RenderTarget[0].DestBlendAlphaD3D12_BLEND_ZERO;

        CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc(D3D12_DEFAULT);
        depthStencilDesc.DepthEnableFALSE;
        depthStencilDesc.DepthWriteMaskD3D12_DEPTH_WRITE_MASK_ZERO;

        // Describe and create the graphics pipeline state object (PSO).
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
        psoDesc.InputLayout{ inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignaturem_rootSignature.Get();
        psoDesc.VSCD3DX12_SHADER_BYTECODE(vertexShader.Get());
        psoDesc.GSCD3DX12_SHADER_BYTECODE(geometryShader.Get());
        psoDesc.PSCD3DX12_SHADER_BYTECODE(pixelShader.Get());
        psoDesc.RasterizerStateCD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.BlendStateblendDesc;
        // psoDesc.BlendStateCD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilStatedepthStencilDesc;
        // psoDesc.DepthStencilStateCD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        psoDesc.SampleMaskUINT_MAX;
        psoDesc.PrimitiveTopologyTypeD3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
        psoDesc.NumRenderTargets;
        psoDesc.RTVFormats[0]DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.DSVFormatDXGI_FORMAT_D24_UNORM_S8_UINT;
        psoDesc.SampleDesc.Count;

        ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
        NAME_D3D12_OBJECT(m_pipelineState);

        // Describe and create the compute pipeline state object (PSO).
        D3D12_COMPUTE_PIPELINE_STATE_DESC computePsoDesc{};
        computePsoDesc.pRootSignaturem_computeRootSignature.Get();
        computePsoDesc.CSCD3DX12_SHADER_BYTECODE(computeShader.Get());

        ThrowIfFailed(m_device->CreateComputePipelineState(&computePsoDesc, IID_PPV_ARGS(&m_computeState)));
        NAME_D3D12_OBJECT(m_computeState);
    }

    // Create the command list.
    ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[m_frameIndex].Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)));
    NAME_D3D12_OBJECT(m_commandList);

    CreateVertexBuffer();
    CreateParticleBuffers();

    // Note: ComPtr's are CPU objects but this resource needs to stay in scope until
    // the command list that references it has finished executing on the GPU.
    // We will flush the GPU at the end of this method to ensure the resource is not
    // prematurely destroyed.
    ComPtr<ID3D12Resource> constantBufferCSUpload;

    // Create the compute shader's constant buffer.
    {
        const UINT bufferSizesizeof(ConstantBufferCS);

        ThrowIfFailed(m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&m_constantBufferCS)));

        ThrowIfFailed(m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&constantBufferCSUpload)));

        NAME_D3D12_OBJECT(m_constantBufferCS);

        ConstantBufferCS constantBufferCS{};
        constantBufferCS.param[0]ParticleCount;
        constantBufferCS.param[1]int(ceil(ParticleCount / 128.0f));
        constantBufferCS.paramf[0]0.1f;
        constantBufferCS.paramf[1].0f;

        D3D12_SUBRESOURCE_DATA computeCBData{};
        computeCBData.pDatareinterpret_cast<UINT8*>(&constantBufferCS);
        computeCBData.RowPitchbufferSize;
        computeCBData.SlicePitchcomputeCBData.RowPitch;

        UpdateSubresources<1>(m_commandList.Get(), m_constantBufferCS.Get(), constantBufferCSUpload.Get(), 0, 0, 1, &computeCBData);
        m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_constantBufferCS.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
    }

    // Create the geometry shader's constant buffer.
    {
        const UINT constantBufferGSSizesizeof(ConstantBufferGS) * FrameCount;

        ThrowIfFailed(m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(constantBufferGSSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_constantBufferGS)
        ));

        NAME_D3D12_OBJECT(m_constantBufferGS);

        CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
        ThrowIfFailed(m_constantBufferGS->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBufferGSData)));
        ZeroMemory(m_pConstantBufferGSData, constantBufferGSSize);
    }

    // Close the command list and execute it to begin the initial GPU setup.
    ThrowIfFailed(m_commandList->Close());
    ID3D12CommandList* ppCommandLists[]{ m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Create synchronization objects and wait until assets have been uploaded to the GPU.
    {
        ThrowIfFailed(m_device->CreateFence(m_renderContextFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_renderContextFence)));
        m_renderContextFenceValue++;

        m_renderContextFenceEventCreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_renderContextFenceEvent == nullptr)
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }

        WaitForRenderContext();
    }
}

// Create the particle vertex buffer.
void D3D12Camera::CreateVertexBuffer()
{
    std::vector<ParticleVertex> vertices;
    vertices.resize(ParticleCount);
    for (UINT i0; i < ParticleCount; i++)
    {
        vertices[i].colorXMFLOAT4(0.0f, 1.0f, 0.2f, 1.0f);
    }
    const UINT bufferSizeParticleCount * sizeof(ParticleVertex);

    ThrowIfFailed(m_device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer)));

    ThrowIfFailed(m_device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertexBufferUpload)));

    NAME_D3D12_OBJECT(m_vertexBuffer);

    D3D12_SUBRESOURCE_DATA vertexData{};
    vertexData.pDatareinterpret_cast<UINT8*>(&vertices[0]);
    vertexData.RowPitchbufferSize;
    vertexData.SlicePitchvertexData.RowPitch;

    UpdateSubresources<1>(m_commandList.Get(), m_vertexBuffer.Get(), m_vertexBufferUpload.Get(), 0, 0, 1, &vertexData);
    m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

    m_vertexBufferView.BufferLocationm_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.SizeInBytesstatic_cast<UINT>(bufferSize);
    m_vertexBufferView.StrideInBytessizeof(ParticleVertex);
}

// Random percent value, from -1 to 1.
float D3D12Camera::RandomPercent()
{
    float retstatic_cast<float>((rand() % 10000) - 5000);
    return ret / 5000.0f;
}

void D3D12Camera::LoadParticles(_Out_writes_(numParticles) Particle* pParticles, const XMFLOAT3& center, const XMFLOAT4& velocity, float spread, UINT numParticles)
{
    srand(0);
    for (UINT i0; i < numParticles; i++)
    {
        XMFLOAT3 delta(spread, spread, spread);

        while (XMVectorGetX(XMVector3LengthSq(XMLoadFloat3(&delta))) > spread* spread)
        {
            delta.xRandomPercent() * spread;
            delta.yRandomPercent() * spread;
            delta.zRandomPercent() * spread;
        }

        pParticles[i].position.xcenter.x + delta.x;
        pParticles[i].position.ycenter.y + delta.y;
        pParticles[i].position.zcenter.z + delta.z;
        pParticles[i].position.w0000.0f * 10000.0f;

        pParticles[i].velocityvelocity;
    }
}

// Create the position and velocity buffer shader resources.
void D3D12Camera::CreateParticleBuffers()
{
    // Initialize the data in the buffers.
    std::vector<Particle> data;
    data.resize(ParticleCount);
    const UINT dataSizeParticleCount * sizeof(Particle);

    // Split the particles into two groups.
    float centerSpreadParticleSpread * 0.50f;
    LoadParticles(&data[0], XMFLOAT3(centerSpread, 0, 0), XMFLOAT4(0, 0, -20, 1 / 100000000.0f), ParticleSpread, ParticleCount / 2);
    LoadParticles(&data[ParticleCount / 2], XMFLOAT3(-centerSpread, 0, 0), XMFLOAT4(0, 0, 20, 1 / 100000000.0f), ParticleSpread, ParticleCount / 2);

    D3D12_HEAP_PROPERTIES defaultHeapPropertiesCD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    D3D12_HEAP_PROPERTIES uploadHeapPropertiesCD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC bufferDescCD3DX12_RESOURCE_DESC::Buffer(dataSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
    D3D12_RESOURCE_DESC uploadBufferDescCD3DX12_RESOURCE_DESC::Buffer(dataSize);

    for (UINT index0; index < ThreadCount; index++)
    {
        // Create two buffers in the GPU, each with a copy of the particles data.
        // The compute shader will update one of them while the rendering thread 
        // renders the other. When rendering completes, the threads will swap 
        // which buffer they work on.

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

        D3D12_SUBRESOURCE_DATA particleData{};
        particleData.pDatareinterpret_cast<UINT8*>(&data[0]);
        particleData.RowPitchdataSize;
        particleData.SlicePitchparticleData.RowPitch;

        UpdateSubresources<1>(m_commandList.Get(), m_particleBuffer0[index].Get(), m_particleBuffer0Upload[index].Get(), 0, 0, 1, &particleData);
        UpdateSubresources<1>(m_commandList.Get(), m_particleBuffer1[index].Get(), m_particleBuffer1Upload[index].Get(), 0, 0, 1, &particleData);
        m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_particleBuffer0[index].Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
        m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_particleBuffer1[index].Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Shader4ComponentMappingD3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.FormatDXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimensionD3D12_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.FirstElement0;
        srvDesc.Buffer.NumElementsParticleCount;
        srvDesc.Buffer.StructureByteStridesizeof(Particle);
        srvDesc.Buffer.FlagsD3D12_BUFFER_SRV_FLAG_NONE;

        CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle0(m_srvUavHeap->GetCPUDescriptorHandleForHeapStart(), SrvParticlePosVelo0 + index, m_srvUavDescriptorSize);
        CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle1(m_srvUavHeap->GetCPUDescriptorHandleForHeapStart(), SrvParticlePosVelo1 + index, m_srvUavDescriptorSize);
        m_device->CreateShaderResourceView(m_particleBuffer0[index].Get(), &srvDesc, srvHandle0);
        m_device->CreateShaderResourceView(m_particleBuffer1[index].Get(), &srvDesc, srvHandle1);

        D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
        uavDesc.FormatDXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimensionD3D12_UAV_DIMENSION_BUFFER;
        uavDesc.Buffer.FirstElement0;
        uavDesc.Buffer.NumElementsParticleCount;
        uavDesc.Buffer.StructureByteStridesizeof(Particle);
        uavDesc.Buffer.CounterOffsetInBytes0;
        uavDesc.Buffer.FlagsD3D12_BUFFER_UAV_FLAG_NONE;

        CD3DX12_CPU_DESCRIPTOR_HANDLE uavHandle0(m_srvUavHeap->GetCPUDescriptorHandleForHeapStart(), UavParticlePosVelo0 + index, m_srvUavDescriptorSize);
        CD3DX12_CPU_DESCRIPTOR_HANDLE uavHandle1(m_srvUavHeap->GetCPUDescriptorHandleForHeapStart(), UavParticlePosVelo1 + index, m_srvUavDescriptorSize);
        m_device->CreateUnorderedAccessView(m_particleBuffer0[index].Get(), nullptr, &uavDesc, uavHandle0);
        m_device->CreateUnorderedAccessView(m_particleBuffer1[index].Get(), nullptr, &uavDesc, uavHandle1);
    }
}

void D3D12Camera::CreateAsyncContexts()
{
    for (UINT threadIndex0; threadIndex < ThreadCount; ++threadIndex)
    {
        // Create compute resources.
        D3D12_COMMAND_QUEUE_DESC queueDesc{ D3D12_COMMAND_LIST_TYPE_COMPUTE, 0, D3D12_COMMAND_QUEUE_FLAG_NONE };
        ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_computeCommandQueue[threadIndex])));
        ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&m_computeAllocator[threadIndex])));
        ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, m_computeAllocator[threadIndex].Get(), nullptr, IID_PPV_ARGS(&m_computeCommandList[threadIndex])));
        ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_SHARED, IID_PPV_ARGS(&m_threadFences[threadIndex])));

        m_threadFenceEvents[threadIndex]CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_threadFenceEvents[threadIndex] == nullptr)
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }

        m_threadData[threadIndex].pContextthis;
        m_threadData[threadIndex].threadIndexthreadIndex;

        m_threadHandles[threadIndex]CreateThread(
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(ThreadProc),
            reinterpret_cast<void*>(&m_threadData[threadIndex]),
            CREATE_SUSPENDED,
            nullptr);

        ResumeThread(m_threadHandles[threadIndex]);
    }
}

// Update frame-based values.
void D3D12Camera::OnUpdate()
{
    // Wait for the previous Present to complete.
    WaitForSingleObjectEx(m_swapChainEvent, 100, FALSE);

    m_timer.Tick(NULL);
    m_camera.Update(static_cast<float>(m_timer.GetElapsedSeconds()));

    ConstantBufferGS constantBufferGS{};
    XMStoreFloat4x4(&constantBufferGS.worldViewProjection, XMMatrixMultiply(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(0.8f, m_aspectRatio, 1.0f, 5000.0f)));
    XMStoreFloat4x4(&constantBufferGS.inverseView, XMMatrixInverse(nullptr, m_camera.GetViewMatrix()));

    UINT8* destinationm_pConstantBufferGSData + sizeof(ConstantBufferGS) * m_frameIndex;
    memcpy(destination, &constantBufferGS, sizeof(ConstantBufferGS));
}

// Render the scene.
void D3D12Camera::OnRender()
{
    // Let the compute thread know that a new frame is being rendered.
    for (int n0; n < ThreadCount; n++)
    {
        InterlockedExchange(&m_renderContextFenceValues[n], m_renderContextFenceValue);
    }

    // Compute work must be completed before the frame can render or else the SRV 
    // will be in the wrong state.
    for (UINT n0; n < ThreadCount; n++)
    {
        UINT64 threadFenceValueInterlockedGetValue(&m_threadFenceValues[n]);
        if (m_threadFences[n]->GetCompletedValue() < threadFenceValue)
        {
            // Instruct the rendering command queue to wait for the current 
            // compute work to complete.
            ThrowIfFailed(m_commandQueue->Wait(m_threadFences[n].Get(), threadFenceValue));
        }
    }

    PIXBeginEvent(m_commandQueue.Get(), 0, L"Render");

    // Record all the commands we need to render the scene into the command list.
    PopulateCommandList();

    // Execute the command list.
    ID3D12CommandList* ppCommandLists[]{ m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    PIXEndEvent(m_commandQueue.Get());

    // Present the frame.
    ThrowIfFailed(m_swapChain->Present(1, 0));

    MoveToNextFrame();
}

// Fill the command list with all the render commands and dependent state.
void D3D12Camera::PopulateCommandList()
{
    // Command list allocators can only be reset when the associated
    // command lists have finished execution on the GPU; apps should use
    // fences to determine GPU execution progress.
    ThrowIfFailed(m_commandAllocators[m_frameIndex]->Reset());

    // However, when ExecuteCommandList() is called on a particular command
    // list, that command list can then be reset at any time and must be before
    // re-recording.
    ThrowIfFailed(m_commandList->Reset(m_commandAllocators[m_frameIndex].Get(), m_pipelineState.Get()));

    // Set necessary state.
    m_commandList->SetPipelineState(m_pipelineState.Get());
    m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());

    m_commandList->SetGraphicsRootConstantBufferView(GraphicsRootCBV, m_constantBufferGS->GetGPUVirtualAddress() + m_frameIndex * sizeof(ConstantBufferGS));

    ID3D12DescriptorHeap* ppHeaps[]{ m_srvUavHeap.Get() };
    m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
    m_commandList->RSSetScissorRects(1, &m_scissorRect);

    // Indicate that the back buffer will be used as a render target.
    m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[]{ 0.0f, 0.0f, 0.1f, 0.0f };
    m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // Render the particles.
    float viewportHeightstatic_cast<float>(static_cast<UINT>(m_viewport.Height) / m_heightInstances);
    float viewportWidthstatic_cast<float>(static_cast<UINT>(m_viewport.Width) / m_widthInstances);
    for (UINT n0; n < ThreadCount; n++)
    {
        const UINT srvIndexn + (m_srvIndex[n] == 0 ? SrvParticlePosVelo0 : SrvParticlePosVelo1);

        CD3DX12_VIEWPORT viewport(
            (n % m_widthInstances) * viewportWidth,
            (n / m_widthInstances) * viewportHeight,
            viewportWidth,
            viewportHeight);

        m_commandList->RSSetViewports(1, &viewport);

        CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(m_srvUavHeap->GetGPUDescriptorHandleForHeapStart(), srvIndex, m_srvUavDescriptorSize);
        m_commandList->SetGraphicsRootDescriptorTable(GraphicsRootSRVTable, srvHandle);

        PIXBeginEvent(m_commandList.Get(), 0, L"Draw particles for thread %u", n);
        m_commandList->DrawInstanced(ParticleCount, 1, 0, 0);
        PIXEndEvent(m_commandList.Get());
    }

    m_commandList->RSSetViewports(1, &m_viewport);

    // Indicate that the back buffer will now be used to present.
    m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    ThrowIfFailed(m_commandList->Close());
}

DWORD D3D12Camera::AsyncComputeThreadProc(int threadIndex)
{
    ID3D12CommandQueue* pCommandQueuem_computeCommandQueue[threadIndex].Get();
    ID3D12CommandAllocator* pCommandAllocatorm_computeAllocator[threadIndex].Get();
    ID3D12GraphicsCommandList* pCommandListm_computeCommandList[threadIndex].Get();
    ID3D12Fence* pFencem_threadFences[threadIndex].Get();

    while (0 == InterlockedGetValue(&m_terminating))
    {
        // Run the particle simulation.
        Simulate(threadIndex);

        // Close and execute the command list.
        ThrowIfFailed(pCommandList->Close());
        ID3D12CommandList* ppCommandLists[]{ pCommandList };

        PIXBeginEvent(pCommandQueue, 0, L"Thread %d: Iterate on the particle simulation", threadIndex);
        pCommandQueue->ExecuteCommandLists(1, ppCommandLists);
        PIXEndEvent(pCommandQueue);

        // Wait for the compute shader to complete the simulation.
        UINT64 threadFenceValueInterlockedIncrement(&m_threadFenceValues[threadIndex]);
        ThrowIfFailed(pCommandQueue->Signal(pFence, threadFenceValue));
        ThrowIfFailed(pFence->SetEventOnCompletion(threadFenceValue, m_threadFenceEvents[threadIndex]));
        WaitForSingleObject(m_threadFenceEvents[threadIndex], INFINITE);

        // Wait for the render thread to be done with the SRV so that
        // the next frame in the simulation can run.
        UINT64 renderContextFenceValueInterlockedGetValue(&m_renderContextFenceValues[threadIndex]);
        if (m_renderContextFence->GetCompletedValue() < renderContextFenceValue)
        {
            ThrowIfFailed(pCommandQueue->Wait(m_renderContextFence.Get(), renderContextFenceValue));
            InterlockedExchange(&m_renderContextFenceValues[threadIndex], 0);
        }

        // Swap the indices to the SRV and UAV.
        m_srvIndex[threadIndex] - m_srvIndex[threadIndex];

        // Prepare for the next frame.
        ThrowIfFailed(pCommandAllocator->Reset());
        ThrowIfFailed(pCommandList->Reset(pCommandAllocator, m_computeState.Get()));
    }

    return 0;
}

// Run the particle simulation using the compute shader.
void D3D12Camera::Simulate(UINT threadIndex)
{
    ID3D12GraphicsCommandList* pCommandListm_computeCommandList[threadIndex].Get();

    UINT srvIndex;
    UINT uavIndex;
    ID3D12Resource* pUavResource;
    if (m_srvIndex[threadIndex] == 0)
    {
        srvIndexSrvParticlePosVelo0;
        uavIndexUavParticlePosVelo1;
        pUavResourcem_particleBuffer1[threadIndex].Get();
    }
    else
    {
        srvIndexSrvParticlePosVelo1;
        uavIndexUavParticlePosVelo0;
        pUavResourcem_particleBuffer0[threadIndex].Get();
    }

    pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pUavResource, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

    pCommandList->SetPipelineState(m_computeState.Get());
    pCommandList->SetComputeRootSignature(m_computeRootSignature.Get());

    ID3D12DescriptorHeap* ppHeaps[]{ m_srvUavHeap.Get() };
    pCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(m_srvUavHeap->GetGPUDescriptorHandleForHeapStart(), srvIndex + threadIndex, m_srvUavDescriptorSize);
    CD3DX12_GPU_DESCRIPTOR_HANDLE uavHandle(m_srvUavHeap->GetGPUDescriptorHandleForHeapStart(), uavIndex + threadIndex, m_srvUavDescriptorSize);

    pCommandList->SetComputeRootConstantBufferView(ComputeRootCBV, m_constantBufferCS->GetGPUVirtualAddress());
    pCommandList->SetComputeRootDescriptorTable(ComputeRootSRVTable, srvHandle);
    pCommandList->SetComputeRootDescriptorTable(ComputeRootUAVTable, uavHandle);

    pCommandList->Dispatch(static_cast<int>(ceil(ParticleCount / 128.0f)), 1, 1);

    pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pUavResource, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
}

void D3D12Camera::OnDestroy()
{
    // Notify the compute threads that the app is shutting down.
    InterlockedExchange(&m_terminating, 1);
    WaitForMultipleObjects(ThreadCount, m_threadHandles, TRUE, INFINITE);

    // Ensure that the GPU is no longer referencing resources that are about to be
    // cleaned up by the destructor.
    WaitForRenderContext();

    // Close handles to fence events and threads.
    CloseHandle(m_renderContextFenceEvent);
    for (int n0; n < ThreadCount; n++)
    {
        CloseHandle(m_threadHandles[n]);
        CloseHandle(m_threadFenceEvents[n]);
    }
}

void D3D12Camera::OnKeyDown(UINT8 key)
{
    m_camera.OnKeyDown(key);
}

void D3D12Camera::OnKeyUp(UINT8 key)
{
    m_camera.OnKeyUp(key);
}

void D3D12Camera::WaitForRenderContext()
{
    // Add a signal command to the queue.
    ThrowIfFailed(m_commandQueue->Signal(m_renderContextFence.Get(), m_renderContextFenceValue));

    // Instruct the fence to set the event object when the signal command completes.
    ThrowIfFailed(m_renderContextFence->SetEventOnCompletion(m_renderContextFenceValue, m_renderContextFenceEvent));
    m_renderContextFenceValue++;

    // Wait until the signal command has been processed.
    WaitForSingleObject(m_renderContextFenceEvent, INFINITE);
}

// Cycle through the frame resources. This method blocks execution if the 
// next frame resource in the queue has not yet had its previous contents 
// processed by the GPU.
void D3D12Camera::MoveToNextFrame()
{
    // Assign the current fence value to the current frame.
    m_frameFenceValues[m_frameIndex]m_renderContextFenceValue;

    // Signal and increment the fence value.
    ThrowIfFailed(m_commandQueue->Signal(m_renderContextFence.Get(), m_renderContextFenceValue));
    m_renderContextFenceValue++;

    // Update the frame index.
    m_frameIndexm_swapChain->GetCurrentBackBufferIndex();

    // If the next frame is not ready to be rendered yet, wait until it is ready.
    if (m_renderContextFence->GetCompletedValue() < m_frameFenceValues[m_frameIndex])
    {
        ThrowIfFailed(m_renderContextFence->SetEventOnCompletion(m_frameFenceValues[m_frameIndex], m_renderContextFenceEvent));
        WaitForSingleObject(m_renderContextFenceEvent, INFINITE);
    }
}
