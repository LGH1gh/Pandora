#include "stdafx.h"
#include "D3D12HalloDX12.h"

D3D12HalloDX12::D3D12HalloDX12(UINT width, UINT height, std::wstring name) :
    BaseApp(width, height, name),
    m_frameIndex(0),
    m_pCbvDataBegin(nullptr),
    m_viewport(0.f, 0.f, static_cast<float>(width), static_cast<float>(height)),
    m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)),
    m_fenceValues{},
    m_rtvDescriptorSize(0),
    m_constantBufferData{}
{
}

void D3D12HalloDX12::OnInit()
{
	LoadPipeline();
	LoadAssets();
}

void D3D12HalloDX12::LoadPipeline()
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

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.BufferCountFrameCount;
    swapChainDesc.Widthm_width;
    swapChainDesc.Heightm_height;
    swapChainDesc.FormatDXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsageDXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffectDXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count;

    ComPtr<IDXGISwapChain1> tempSwapChain;
    ThrowIfFailed(factory->CreateSwapChainForHwnd(
        m_commandQueue.Get(),
        Win32Application::GetHwnd(),
        &swapChainDesc,
        nullptr,
        nullptr,
        &tempSwapChain
    ));

    // This sample does not support fullscreen transitions.
    ThrowIfFailed(factory->MakeWindowAssociation(Win32Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

    ThrowIfFailed(tempSwapChain.As(&m_swapChain));
    m_frameIndexm_swapChain->GetCurrentBackBufferIndex();

    // Create descriptor heaps
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
        rtvHeapDesc.NumDescriptorsFrameCount;
        rtvHeapDesc.TypeD3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.FlagsD3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

        m_rtvDescriptorSizem_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        // Describe and create a shader resource view (SRV) heap for the texture.
        //D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
        //srvHeapDesc.NumDescriptors;
        //srvHeapDesc.TypeD3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        //srvHeapDesc.FlagsD3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        //ThrowIfFailed(m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap)));

        // Describe and create a constant buffer view (CBV) descriptor heap.
        // Flags indicate that this descriptor heap can be bound to the pipeline
        // and that descriptors contained in it can be referenced by a root table,
        D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
        cbvHeapDesc.NumDescriptors;
        cbvHeapDesc.FlagsD3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        cbvHeapDesc.TypeD3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        ThrowIfFailed(m_device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_cbvHeap)));
    }
    
    // Create frame resources
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

        for (UINT n0; n < FrameCount; n++) {
            ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTarget[n])));
            m_device->CreateRenderTargetView(m_renderTarget[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, m_rtvDescriptorSize);

            ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n])));
            ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&m_bundleAllocators[n])));
        }
    }
}

// Load the sample assets.
void D3D12HalloDX12::LoadAssets()
{
    // Create a root signature consisting of a descriptor table with a single CBV,
    {
        D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData{};

        // This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
        featureData.HighestVersionD3D_ROOT_SIGNATURE_VERSION_1_1;
        if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
        {
            featureData.HighestVersionD3D_ROOT_SIGNATURE_VERSION_1_0;
        }

        CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
        CD3DX12_ROOT_PARAMETER1 rootParameters[1];
        
        // Texture
        // ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
        // rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);

        //D3D12_STATIC_SAMPLER_DESC sampler{};
        //sampler.FilterD3D12_FILTER_MIN_MAG_MIP_POINT;
        //sampler.AddressUD3D12_TEXTURE_ADDRESS_MODE_BORDER;
        //sampler.AddressVD3D12_TEXTURE_ADDRESS_MODE_BORDER;
        //sampler.AddressWD3D12_TEXTURE_ADDRESS_MODE_BORDER;
        //sampler.MipLODBias0;
        //sampler.MaxAnisotropy0;
        //sampler.ComparisonFuncD3D12_COMPARISON_FUNC_NEVER;
        //sampler.BorderColorD3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
        //sampler.MinLOD0.f;
        //sampler.MaxLODD3D12_FLOAT32_MAX;
        //sampler.ShaderRegister0;
        //sampler.RegisterSpace0;
        //sampler.ShaderVisibilityD3D12_SHADER_VISIBILITY_PIXEL;

       
        // Constant Buffer
        ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
        rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);

        // Allow input layout and deny uneccessary access to certain pipeline stages.
        D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

        CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);

        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
        ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
    }

    // Create the pipeline state, which includes compiling and loading shaders
    {
        ComPtr<ID3DBlob> vertexShader;
        ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
        // Enable better shader debugging with the graphics debugging tools.
        UINT compileFlagsD3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags0;
#endif
        ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"Shader.hlsl").c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
        ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"Shader.hlsl").c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));
    
        // Define the vertex input layout
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        };

        // Describe and create the graphics pipeline state object
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
        psoDesc.InputLayout{ inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignaturem_rootSignature.Get();
        psoDesc.VSCD3DX12_SHADER_BYTECODE(vertexShader.Get());
        psoDesc.PSCD3DX12_SHADER_BYTECODE(pixelShader.Get());
        psoDesc.RasterizerStateCD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.BlendStateCD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState.DepthEnableFALSE;
        psoDesc.DepthStencilState.StencilEnableFALSE;
        psoDesc.SampleMaskUINT_MAX;
        psoDesc.PrimitiveTopologyTypeD3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets;
        psoDesc.RTVFormats[0]DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count;

        ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
    }

    ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[m_frameIndex].Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)));

     // Create the vertex buffer.
    {
        // Define the geometry for a triangle.
        Vertex triangleVertices[] =
        {
            { { 0.0f, 0.25f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.5f, 0.f } },
            { { 0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.f, 1.f } },
            { { -0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.f, 1.f } }
        };

        const UINT vertexBufferSizesizeof(triangleVertices);

        // Note: using upload heaps to transfer static data like vert buffers is not 
        // recommended. Every time the GPU needs it, the upload heap will be marshalled 
        // over. Please read up on Default Heap usage. An upload heap is used here for 
        // code simplicity and because there are very few verts to actually transfer.
        ThrowIfFailed(m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_vertexBuffer)));

        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;
        CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
        ThrowIfFailed(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
        memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
        m_vertexBuffer->Unmap(0, nullptr);

        m_vertexBufferView.BufferLocationm_vertexBuffer->GetGPUVirtualAddress();
        m_vertexBufferView.StrideInBytessizeof(Vertex);
        m_vertexBufferView.SizeInBytesvertexBufferSize;
    }

    // Create the vertex buffer
    {
        //Vertex triangleVertics[] =
        //{
        //    { { 0.0f, 0.25f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        //    { { 0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        //    { { -0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
        //    //{ { 0.f, -0.25f * m_aspectRatio, 0.f }, { 1.f, 1.f, 0.f, 1.f } },
        //};
        //const UINT vertexBufferSizesizeof(triangleVertics);

        //ThrowIfFailed(m_device->CreateCommittedResource(
        //    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        //    D3D12_HEAP_FLAG_NONE,
        //    &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
        //    D3D12_RESOURCE_STATE_COPY_DEST,
        //    nullptr,
        //    IID_PPV_ARGS(&m_vertexBuffer)));

        //ComPtr<ID3D12Resource> vertexBufferUpload;
        //ThrowIfFailed(m_device->CreateCommittedResource(
        //    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        //    D3D12_HEAP_FLAG_NONE,
        //    &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
        //    D3D12_RESOURCE_STATE_GENERIC_READ,
        //    nullptr,
        //    IID_PPV_ARGS(&vertexBufferUpload)));

        //NAME_D3D12_OBJECT(m_vertexBuffer);

        //D3D12_SUBRESOURCE_DATA vertexData{};
        //vertexData.pDatareinterpret_cast<UINT8*>(triangleVertics);
        //vertexData.RowPitchvertexBufferSize;
        //vertexData.SlicePitchvertexBufferSize;

        //UpdateSubresources<1>(m_commandList.Get(), m_vertexBuffer.Get(), vertexBufferUpload.Get(), 0, 0, 1, &vertexData);
        //m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

        //m_vertexBufferView.BufferLocationm_vertexBuffer->GetGPUVirtualAddress();
        //m_vertexBufferView.StrideInBytessizeof(Vertex);
        //m_vertexBufferView.SizeInBytesvertexBufferSize;
    }

    // Create the index buffer
    /*{
        DWORD triangleIndex[]{
            0, 1, 2,
            1, 2, 3
        };

        const UINT indexBufferSizesizeof(triangleIndex);

        ThrowIfFailed(m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&m_indexBuffer)));

        ComPtr<ID3D12Resource> indexBufferUploadHeap;
        ThrowIfFailed(m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&indexBufferUploadHeap)));

        D3D12_SUBRESOURCE_DATA indexData{};
        indexData.pDatareinterpret_cast<BYTE*>(triangleIndex);
        indexData.RowPitchindexBufferSize;
        indexData.SlicePitchindexBufferSize;

        UpdateSubresources(m_commandList.Get(), m_indexBuffer.Get(), indexBufferUploadHeap.Get(), 0, 0, 1, &indexData);
        m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));

        m_indexBufferView.BufferLocationm_indexBuffer->GetGPUVirtualAddress();
        m_indexBufferView.FormatDXGI_FORMAT_R32_UINT;
        m_indexBufferView.SizeInBytesindexBufferSize;
    }*/

    // Note: ComPtr's are CPU objects but this resource needs to stay in scope until
    // the command list that references it has finished executing on the GPU.
    // We will flush the GPU at the end of this method to ensure the resource is not
    // prematurely destroyed.
    // ComPtr<ID3D12Resource> textureUploadHeap;

    // Create the texture
    /*{
        // Describe and create a Texture2D
        D3D12_RESOURCE_DESC textureDesc{};
        textureDesc.MipLevels;
        textureDesc.FormatDXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.WidthTextureWidth;
        textureDesc.HeightTextureHeight;
        textureDesc.FlagsD3D12_RESOURCE_FLAG_NONE;
        textureDesc.DepthOrArraySize;
        textureDesc.SampleDesc.Count;
        textureDesc.SampleDesc.Quality;
        textureDesc.DimensionD3D12_RESOURCE_DIMENSION_TEXTURE2D;

        ThrowIfFailed(m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &textureDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&m_texture)
        ));

        const UINT64 uploadBufferSizeGetRequiredIntermediateSize(m_texture.Get(), 0, 1);

        // Create the GPU upload buffer.
        ThrowIfFailed(m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&textureUploadHeap)
        ));

        // Copy data to the intermediate upload heap and then schedule a copy 
        // from the upload heap to the Texture2D.
        std::vector<UINT8> textureGenerateTextureData();

        D3D12_SUBRESOURCE_DATA textureData{};
        textureData.pData&texture[0];
        textureData.RowPitchTextureWidth * TexturePixelSize;
        textureData.SlicePitchtextureData.RowPitch * TextureHeight;

        UpdateSubresources(m_commandList.Get(), m_texture.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);
        m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

        // Describe and create a SRV for the texture.
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Shader4ComponentMappingD3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.FormattextureDesc.Format;
        srvDesc.ViewDimensionD3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels;
        m_device->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_srvHeap->GetCPUDescriptorHandleForHeapStart());
    }*/

    // Create the constant buffer.
    {
        ThrowIfFailed(m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(1024 * 64),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_constantBuffer)
        ));

        // Describe and create a constant buffer view.
        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
        cbvDesc.BufferLocationm_constantBuffer->GetGPUVirtualAddress();
        cbvDesc.SizeInBytes(sizeof(SceneConstantBuffer) + 255) & ~255;
        m_device->CreateConstantBufferView(&cbvDesc, m_cbvHeap->GetCPUDescriptorHandleForHeapStart());

        // Map and initialize the constant buffer. We don't unmap this until the
        // app closes. Keeping things mapped for the lifetime of the resource is okay.
        CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
        ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvDataBegin)));
        memcpy(m_pCbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
    }

    // Close the command list and execute it to begin the initial GPU setup.
    ThrowIfFailed(m_commandList->Close());
    ID3D12CommandList* ppCommandLists[]{ m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
    
    // Create and record the bundle.
    {
        ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, m_bundleAllocators[m_frameIndex].Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_bundle)));
        m_bundle->SetGraphicsRootSignature(m_rootSignature.Get());
        m_bundle->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_bundle->IASetVertexBuffers(0, 1, &m_vertexBufferView);
        m_bundle->DrawInstanced(3, 1, 0, 0);
        ThrowIfFailed(m_bundle->Close());
    }

    {
        ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
        m_fenceValues[m_frameIndex]++;

        m_fenceEventCreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_fenceEvent == nullptr)
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }

        WaitForGPU();
    }
}

// Generate a simple black and white checkerboard texture.
std::vector<UINT8> D3D12HalloDX12::GenerateTextureData()
{
    const UINT rowPitchTextureWidth * TexturePixelSize;
    const UINT cellPitchrowPitch >> 3;        // The width of a cell in the checkboard texture.
    const UINT cellHeightTextureWidth >> 3;    // The height of a cell in the checkerboard texture.
    const UINT textureSizerowPitch * TextureHeight;

    std::vector<UINT8> data(textureSize);
    UINT8* pData&data[0];

    for (UINT n0; n < textureSize; n += TexturePixelSize)
    {
        UINT xn % rowPitch;
        UINT yn / rowPitch;
        UINT ix / cellPitch;
        UINT jy / cellHeight;

        if (i % 2 == j % 2)
        {
            pData[n]0x00;        // R
            pData[n + 1]0x00;    // G
            pData[n + 2]0x00;    // B
            pData[n + 3]0xff;    // A
        }
        else
        {
            pData[n]0xff;        // R
            pData[n + 1]0xff;    // G
            pData[n + 2]0xff;    // B
            pData[n + 3]0xff;    // A
        }
    }

    return data;
}

// Update frame-based values
void D3D12HalloDX12::OnUpdate()
{
    const float translationSpeed0.005f;
    const float offsetBounds.25f;

    m_constantBufferData.offset.x += translationSpeed;
    if (m_constantBufferData.offset.x > offsetBounds)
    {
        m_constantBufferData.offset.x-offsetBounds;
    }

    memcpy(m_pCbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
}

void D3D12HalloDX12::OnRender()
{
    PopulateCommandList();

    ID3D12CommandList* ppCommandLists[]{ m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Present the frame.
    ThrowIfFailed(m_swapChain->Present(1, 0));

    MoveToNextFrame();
}

void D3D12HalloDX12::OnDestroy()
{
    WaitForGPU();
    CloseHandle(m_fenceEvent);
}

void D3D12HalloDX12::PopulateCommandList()
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
    m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());

    ID3D12DescriptorHeap* ppHeaps[]{ m_cbvHeap.Get() };
    m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    m_commandList->SetGraphicsRootDescriptorTable(0, m_cbvHeap->GetGPUDescriptorHandleForHeapStart());
    m_commandList->RSSetViewports(1, &m_viewport);
    m_commandList->RSSetScissorRects(1, &m_scissorRect);

    // Indicate that the back buffer will be used as a render target.
    m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTarget[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[]{ 0.f, 0.2f, 0.4f, 1.f };
    m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    
    // Execute the commands stored in the bundle.
    m_commandList->ExecuteBundle(m_bundle.Get());

    // Indicate that the back buffer will now be used to present.
    m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTarget[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    ThrowIfFailed(m_commandList->Close());
}

void D3D12HalloDX12::WaitForGPU()
{
    ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_frameIndex]));

    // Wait until the fence has been processed.
    ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent));
    WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

    // Increment the fence value for the current frame.
    m_fenceValues[m_frameIndex]++;
}

void D3D12HalloDX12::MoveToNextFrame()
{
    const UINT64 currentFenceValuem_fenceValues[m_frameIndex];
    ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

    // Update the frame index.
    m_frameIndexm_swapChain->GetCurrentBackBufferIndex();

    // If the next frame is not ready to be rendered yet, wait until it is ready.
    if (m_fence->GetCompletedValue() < m_fenceValues[m_frameIndex])
    {
        ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent));
        WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
    }

    m_fenceValues[m_frameIndex]currentFenceValue + 1;
}
