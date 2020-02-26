#include "BaseApp.h"

BaseApp::BaseApp(UINT width, UINT height, std::wstring title)
{
    m_deviceParams = { false, width, height };
    m_title = title;
}

BaseApp::~BaseApp()
{
}

void BaseApp::OnInit()
{
	m_device = CreateDevice(m_deviceParams, m_hwnd);
    m_rootSignature = CreateRootSignature(m_device);

    UINT compileFlags = COMPILE_DEBUG | COMPILE_SKIP_OPTIMIZATION;
    Blob vertexShader = CreateShaderFromFile(SHADER_TYPE_VERTEX_SHADER, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "VSMain", compileFlags);
    Blob pixelShader1 = CreateShaderFromFile(SHADER_TYPE_PIXEL_SHADER, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "PSMain1", compileFlags);
    Blob pixelShader2 = CreateShaderFromFile(SHADER_TYPE_PIXEL_SHADER, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "PSMain2", compileFlags);
    
    SAttributeDesc inputElementDesc[] =
    {
        { "POSITION", 0, ATTRIBUTE_FORMAT_FLOAT3_32 },
        { "COLOR", 0, ATTRIBUTE_FORMAT_FLOAT4_32 },
    };

    //RenderPass renderPass = CreateRenderPass(m_device, IMAGE_FORMAT_R8G8B8A8_UNORM, IMAGE_FORMAT_R8G8B8A8_UNORM, CLEAR_COLOR);

    PipelineParams pipelineParams;
    pipelineParams.attributeCount = 2;
    pipelineParams.attributes = inputElementDesc;
    pipelineParams.rootSignature = m_rootSignature;
    pipelineParams.vs = { vertexShader->address, vertexShader->size };
    pipelineParams.ps = { pixelShader1->address, pixelShader1->size };
    pipelineParams.cullMode = CULL_NONE;
    pipelineParams.renderPass = GetRenderPass(m_device);
    m_pipeline1 = CreateGraphicsPipeline(m_device, pipelineParams);

    pipelineParams.ps = { pixelShader2->address, pixelShader2->size };
    m_pipeline2 = CreateGraphicsPipeline(m_device, pipelineParams);

    Vertex triangleVertices[] = 
    {
        { { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.0f, -0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { -0.25f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        
        { { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.5f, 0.75f, 0.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
        { { 0.75f, 0.5f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },

        { { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.5f, -0.75f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.75f, -0.75f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.75f, -0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }
    };
    const UINT vertexBufferSize = sizeof(triangleVertices);

    BufferParams vertexBufferParams(vertexBufferSize, HEAP_TYPE_DEFAULT, RESOURCE_STATE_COPY_DEST);
    Buffer vertexBuffer = CreateBuffer(m_device, vertexBufferParams);
    BufferParams vertexUploadBufferParams(vertexBufferSize, HEAP_TYPE_UPLOAD, RESOURCE_STATE_GENERIC_READ);
    Buffer vertexUploadBuffer = CreateBuffer(m_device, vertexUploadBufferParams);
    SubresourceParams vertexData = { triangleVertices, vertexBufferSize, vertexBufferSize };
    Subresource(m_device, vertexBuffer, vertexUploadBuffer, vertexData);


    unsigned long indices[] =
    {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,

        7, 8, 9,
        7, 9, 10
    };
    const UINT indexBufferSize = sizeof(indices);

    BufferParams indexBufferParams(indexBufferSize, HEAP_TYPE_DEFAULT, RESOURCE_STATE_COPY_DEST);
    Buffer indexBuffer = CreateBuffer(m_device, indexBufferParams);
    BufferParams indexUploadBufferParams(indexBufferSize, HEAP_TYPE_UPLOAD, RESOURCE_STATE_GENERIC_READ);
    Buffer indexUploadBuffer = CreateBuffer(m_device, indexUploadBufferParams);
    SubresourceParams indexData = { indices, indexBufferSize, indexBufferSize };
    Subresource(m_device, indexBuffer, indexUploadBuffer, indexData);

    ExecuteCommand(m_device);
    m_vertexSetup = CreateVertexSetup(vertexBuffer, sizeof(Vertex), indexBuffer, sizeof(unsigned long));


    
	// m_blendState = CreateBlendState(BLEND_SRC_ALPHA, BLEND_INV_SRC_COLOR, BLEND_OP_ADD, 0xF, false);

}

void BaseApp::OnUpdate()
{

}

void BaseApp::OnDestroy()
{
    WaitForGPU(m_device);
    Destory(m_device);
}

void BaseApp::OnRender()
{
    PopulateCommand();
    ExecuteCommand(m_device);
    Present(m_device);
    MoveToNextFrame(m_device);
}

void BaseApp::PopulateCommand()
{
    Reset(m_device, m_pipeline1);

    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    BeginRenderPass(m_device, m_deviceParams, clearColor);
    {
        SetGraphicsRootSignature(m_device, m_rootSignature);
        SetVertexSetup(m_device, m_vertexSetup);
        SetPipeline(m_device, m_pipeline1);
        DrawIndexed(m_device, 0, 6);
        DrawIndexed(m_device, 6, 3);
        SetPipeline(m_device, m_pipeline2);
        DrawIndexed(m_device, 9, 6);
    }
    EndRenderPass(m_device);

}

_Use_decl_annotations_
void BaseApp::ParseCommandLineArgs(WCHAR* argv[], int argc)
{
    for (int i = 1; i < argc; ++i)
    {
        if (_wcsnicmp(argv[i], L"-warp", wcslen(argv[i])) == 0 ||
            _wcsnicmp(argv[i], L"/warp", wcslen(argv[i])) == 0)
        {
            m_deviceParams.useWarpDevice = true;
            m_title = m_title + L" (WARP)";
        }
    }
}