#include "BaseApp.h"
#include <d3d12.h>

BaseApp::BaseApp(UINT width, UINT height, std::wstring title) :
    m_width(width),
    m_height(height),
    m_title(title)
{
}

BaseApp::~BaseApp()
{
}

void BaseApp::OnInit()
{
    m_kernel = CreateKernel(m_width, m_height, false, m_hwnd);
    SampleDesc sampleDesc;

    m_rootSignature = CreateRootSignature(m_kernel, 1, 0, 0);

    UINT compileFlags = COMPILE_DEBUG | COMPILE_SKIP_OPTIMIZATION;
    InputElementDesc inputElementDesc[] =
    {
        { "POSITION", FORMAT_R32G32B32A32_FLOAT },
        { "COLOR", FORMAT_R32G32B32A32_FLOAT },
    };
    GraphicsPipelineStateDesc psoDesc;
    psoDesc.RootSignature = m_rootSignature;
    psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
    psoDesc.VS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "VSMain", compileFlags);
    psoDesc.PS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "PSMain", compileFlags);
    psoDesc.CullMode = CULL_MODE_NONE;

    m_pipeline = CreateGraphicsPipeline(m_kernel, psoDesc);

    Vertex triangleVertices[] = 
    {
        { { 0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { -0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.5f, -0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
        { { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },

        { { 0.0f, 0.0f, 0.7f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { -0.75f, 0.0f, 0.7f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.75f, 0.75f, 0.7f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { 0.0f, 0.75f, 0.7f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

    };
    DWORD indices[] =
    {
        0, 1, 2,
        1, 2, 3,

        4, 5, 6,
        4, 6, 7
    };
    m_vertexSetup = CreateVertexSetup(
        m_kernel,
        triangleVertices, sizeof(triangleVertices), sizeof(Vertex),
        indices, sizeof(indices), sizeof(DWORD));
    m_depthStencil = CreateDepthStencil(m_kernel);
    m_constantBuffer = CreateConstantBuffer(m_kernel, &m_constantData, sizeof(m_constantData));

    EndOnInit(m_kernel);
    //m_constantBufferDesc = CreateConstantBuffer(m_device, &m_constantBufferData, 1, sizeof(m_constantBufferData));
    //m_texture = CreateTexture(m_device);
    //ExecuteCommand(m_device);
    //MoveToNextFrame(m_device);
}

void BaseApp::OnUpdate()
{
    static float rIncrement = 0.002f;
    static float gIncrement = 0.006f;
    static float bIncrement = 0.009f;
    m_constantData.offset.x += rIncrement;
    m_constantData.offset.y += gIncrement;
    m_constantData.offset.z += bIncrement;

    if (m_constantData.offset.x >= 1.0 || m_constantData.offset.x <= 0.0)
    {
        m_constantData.offset.x = m_constantData.offset.x >= 1.0 ? 1.0 : 0.0;
        rIncrement = -rIncrement;
    }
    if (m_constantData.offset.y >= 1.0 || m_constantData.offset.y <= 0.0)
    {
        m_constantData.offset.y = m_constantData.offset.y >= 1.0 ? 1.0 : 0.0;
        gIncrement = -gIncrement;
    }
    if (m_constantData.offset.z >= 1.0 || m_constantData.offset.z <= 0.0)
    {
        m_constantData.offset.z = m_constantData.offset.z >= 1.0 ? 1.0 : 0.0;
        bIncrement = -bIncrement;
    }
    UpdateConstantBuffer(m_constantBuffer, &m_constantData, sizeof(m_constantData));
}

void BaseApp::OnRender()
{
    PopulateCommand();
    EndOnRender(m_kernel);
}

void BaseApp::PopulateCommand()
{
    Reset(m_kernel, m_pipeline);

    BeginRender(m_kernel, m_depthStencil);
    {
        SetGraphicsRootSignature(m_kernel, m_rootSignature);
        SetPipeline(m_kernel, m_pipeline);;
        SetVertexSetup(m_kernel, m_vertexSetup);
        SetDescriptorHeap(m_kernel, m_constantBuffer);
        //std::vector<DescriptorHeap> heaps = { m_constantBufferDesc, m_texture };
        //SetDescriptorHeaps(m_device, m_constantBufferDesc, m_texture);
        //SetConstantBuffer(m_device, m_constantBufferDesc);
        //SetTextureBuffer(m_device, m_texture);

        DrawIndexed(m_kernel, 6, 6);
        DrawIndexed(m_kernel, 0, 6);

    }
    EndRender(m_kernel);
}

void BaseApp::OnDestroy()
{
    EndOnDestory(m_kernel);
}
