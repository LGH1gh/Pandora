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
    m_rootSignature = CreateRootSignature(m_kernel);

    UINT compileFlags = COMPILE_DEBUG | COMPILE_SKIP_OPTIMIZATION;
    Blob vertexShader = CreateShaderFromFile(SHADER_TYPE_VERTEX_SHADER, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "VSMain", compileFlags);
    Blob pixelShader = CreateShaderFromFile(SHADER_TYPE_PIXEL_SHADER, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "PSMain", compileFlags);
    
    InputElementDesc inputElementDesc[] =
    {
        { "POSITION", FORMAT_R32G32B32A32_FLOAT },
        { "COLOR", FORMAT_R32G32B32A32_FLOAT },
    };
    GraphicsPipelineStateDesc psoDesc;
    psoDesc.RootSignature = m_rootSignature;
    psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
    psoDesc.VS = vertexShader;
    psoDesc.PS = pixelShader;
    psoDesc.CullMode = CULL_MODE_NONE;

    m_pipeline = CreateGraphicsPipeline(m_kernel, psoDesc);

    Vertex triangleVertices[] = 
    {
        { { 0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { -0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.5f, -0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
        { { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },

    };
    DWORD indices[] =
    {
        0, 1, 2,
        1, 2, 3
    };
    m_vertexSetup = CreateVertexSetup(
        m_kernel,
        triangleVertices, sizeof(triangleVertices), sizeof(Vertex),
        indices, sizeof(indices), sizeof(DWORD));

    EndOnInit(m_kernel);
    //m_constantBufferDesc = CreateConstantBuffer(m_device, &m_constantBufferData, 1, sizeof(m_constantBufferData));
    //m_depthStencil = CreateDepthStencil(m_device, GetWidth(), GetHeight());
    //m_texture = CreateTexture(m_device);
    //ExecuteCommand(m_device);
    //MoveToNextFrame(m_device);
}

void BaseApp::OnUpdate()
{
    //static float rotateAngle = 0;
    //ConstantBuffer constantBuffer = {};
    //XMStoreFloat4x4(&constantBuffer.worldViewProjection, XMMatrixMultiply(m_coordinate.GetViewMatrix(), m_coordinate.GetProjectionMatrix(0.8f, static_cast<float>(GetWidth()) / static_cast<float>(GetHeight()), 1.0f, 1000.0f)));
    //constantBuffer.rotateWithY = XMFLOAT4X4(
    //    (float)cos(rotateAngle), 0, (float)sin(rotateAngle), 0,
    //    0, 1, 0, 0,
    //    (float)-sin(rotateAngle), 0, (float)cos(rotateAngle), 0,
    //    0, 0, 0, 1
    //);
    //rotateAngle += 0.001f;
    //UpdateBuffer(m_constantBufferDesc, &constantBuffer, sizeof(constantBuffer));
}

void BaseApp::OnRender()
{
    PopulateCommand();
    EndOnRender(m_kernel);
}

void BaseApp::PopulateCommand()
{
    Reset(m_kernel, m_pipeline);

    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    BeginRender(m_kernel, clearColor);
    {
        SetGraphicsRootSignature(m_kernel, m_rootSignature);
        SetPipeline(m_kernel, m_pipeline);;
        SetVertexSetup(m_kernel, m_vertexSetup);

        //std::vector<DescriptorHeap> heaps = { m_constantBufferDesc, m_texture };
        //SetDescriptorHeaps(m_device, m_constantBufferDesc, m_texture);
        //SetConstantBuffer(m_device, m_constantBufferDesc);
        //SetTextureBuffer(m_device, m_texture);
        DrawIndexed(m_kernel, 0, 6);
    }
    EndRender(m_kernel);
}

void BaseApp::OnDestroy()
{
    EndOnDestory(m_kernel);
}
