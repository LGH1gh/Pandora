#include "BaseApp.h"
#include <d3d12.h>

BaseApp::BaseApp(UINT width, UINT height, std::wstring title) :
    m_width(width),
    m_height(height),
    m_title(title),
    m_coordinate()
{
}

BaseApp::~BaseApp()
{
}

void BaseApp::OnInit()
{
    m_kernel = CreateKernel(m_width, m_height, false, m_hwnd);
    m_rootSignature = CreateRootSignature(m_kernel, 1, 1, 0, &StaticSampleDesc::Init(Default));

    UINT compileFlags = COMPILE_DEBUG | COMPILE_SKIP_OPTIMIZATION;
    InputElementDesc inputElementDesc[] =
    {
        { "POSITION", FORMAT_R32G32B32A32_FLOAT },
        { "TEXCOORD", FORMAT_R32G32_FLOAT },
    };
    GraphicsPipelineStateDesc psoDesc;
    psoDesc.RootSignature = m_rootSignature;
    psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
    psoDesc.VS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\VertexShader.hlsl", "VSMain", compileFlags);
    psoDesc.PS = ShaderDesc(L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\PixelShader.hlsl", "PSMain", compileFlags);
    psoDesc.CullMode = CULL_MODE_NONE;

    m_pipeline = CreateGraphicsPipeline(m_kernel, psoDesc);

    Vertex triangleVertices[] = 
    {
        // front face
        { -0.5f,  0.5f, -0.5f, 0.5f, 0.0f, 0.0f },
        {  0.5f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f },
        { -0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f, -0.5f, 0.5f, 1.0f, 0.0f },

        // right side face
        {  0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f,  0.5f, 0.5f, 1.0f, 0.0f },
        {  0.5f, -0.5f,  0.5f, 0.5f, 1.0f, 1.0f },
        {  0.5f,  0.5f, -0.5f, 0.5f, 0.0f, 0.0f },

        // left side face
        { -0.5f,  0.5f,  0.5f, 0.5f, 0.0f, 0.0f },
        { -0.5f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f },
        { -0.5f, -0.5f,  0.5f, 0.5f, 0.0f, 1.0f },
        { -0.5f,  0.5f, -0.5f, 0.5f, 1.0f, 0.0f },

        // back face
        {  0.5f,  0.5f,  0.5f, 0.5f, 0.0f, 0.0f },
        { -0.5f, -0.5f,  0.5f, 0.5f, 1.0f, 1.0f },
        {  0.5f, -0.5f,  0.5f, 0.5f, 0.0f, 1.0f },
        { -0.5f,  0.5f,  0.5f, 0.5f, 1.0f, 0.0f },

        // top face
        { -0.5f,  0.5f, -0.5f, 0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f,  0.5f, 0.5f, 1.0f, 0.0f },
        {  0.5f,  0.5f, -0.5f, 0.5f, 1.0f, 1.0f },
        { -0.5f,  0.5f,  0.5f, 0.5f, 0.0f, 0.0f },

        // bottom face
        {  0.5f, -0.5f,  0.5f, 0.5f, 0.0f, 0.0f },
        { -0.5f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f },
        {  0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 1.0f },
        { -0.5f, -0.5f,  0.5f, 0.5f, 1.0f, 0.0f },
    };
    DWORD indices[] =
    {
        0, 1, 2, // first triangle
        0, 3, 1, // second triangle

        // left face
        4, 5, 6, // first triangle
        4, 7, 5, // second triangle

        // right face
        8, 9, 10, // first triangle
        8, 11, 9, // second triangle

        // back face
        12, 13, 14, // first triangle
        12, 15, 13, // second triangle

        // top face
        16, 17, 18, // first triangle
        16, 19, 17, // second triangle

        // bottom face
        20, 21, 22, // first triangle
        20, 23, 21, // second triangle
    };
    m_vertexSetup = CreateVertexSetup(
        m_kernel,
        triangleVertices, sizeof(triangleVertices), sizeof(Vertex),
        indices, sizeof(indices), sizeof(DWORD));

    m_depthStencil = CreateDepthStencilViewHeap(m_kernel);
    m_constantBuffer = CreateConstantBuffer(m_kernel, &m_constantData, sizeof(m_constantData));
    m_texture = CreateTexture(m_kernel, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\timg.jpg");
    
    FontDesc fontDesc;
    fontDesc.Content = L"Hello, World";
    fontDesc.Layout = { 0.f, 0.f, (float)m_width, (float)m_height };
    fontDesc.TextAlignment = TEXT_ALIGNMENT_LEADING;
    fontDesc.ParagraphAlignment = PARAGRAPH_ALIGNMENT_NEAR;
    m_texts.push_back(fontDesc);
    EndOnInit(m_kernel);
}

XMFLOAT4 Calculate(XMFLOAT4X4 matrix, XMFLOAT4 vector)
{
    XMFLOAT4 result;
    result.x = matrix._11 * vector.x + matrix._12 * vector.y + matrix._13 * vector.z + matrix._14 * vector.w;
    result.y = matrix._21 * vector.x + matrix._22 * vector.y + matrix._23 * vector.z + matrix._24 * vector.w;
    result.z = matrix._31 * vector.x + matrix._32 * vector.y + matrix._33 * vector.z + matrix._34 * vector.w;
    result.w = matrix._41 * vector.x + matrix._42 * vector.y + matrix._43 * vector.z + matrix._44 * vector.w;
    return result;
}

void BaseApp::OnUpdate()
{
    static float rotateAngle = 0;

    XMStoreFloat4x4(&m_constantData.worldViewProjection, XMMatrixMultiply(m_coordinate.GetViewMatrix(), m_coordinate.GetProjectionMatrix(0.8f, static_cast<float>(GetWidth()) / static_cast<float>(GetHeight()), 1.0f, 1000.0f)));
    m_constantData.rotateWithY = XMFLOAT4X4(
        (float)cos(rotateAngle), 0, (float)sin(rotateAngle), 0,
        0, 1, 0, 0,
        (float)-sin(rotateAngle), 0, (float)cos(rotateAngle), 0,
        0, 0, 0, 1
    );
    m_constantData.rotateWithZ = XMFLOAT4X4(
        (float)cos(rotateAngle), (float)-sin(rotateAngle), 0, 0,
        (float)sin(rotateAngle), (float)cos(rotateAngle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    rotateAngle += 0.01f;
    UpdateConstantBuffer(m_constantBuffer, &m_constantData, sizeof(m_constantData));
}

void BaseApp::OnRender()
{
    PopulateCommand();
    RenderText(m_kernel, m_texts);
    EndOnRender(m_kernel);
}

void BaseApp::PopulateCommand()
{
    Reset(m_kernel, m_pipeline);

    BeginPopulateGraphicsCommand(m_kernel, m_depthStencil);
    {
        SetGraphicsRootSignature(m_kernel, m_rootSignature);
        SetPipeline(m_kernel, m_pipeline);;
        SetVertexSetup(m_kernel, m_vertexSetup);
        SetConstantBuffer(m_kernel, m_constantBuffer);
        SetShaderResource(m_kernel, m_texture);

        DrawIndexed(m_kernel, 0, 36);
    }
    EndPopulateGraphicsCommand(m_kernel);
    ExecuteCommand(m_kernel);
}

void BaseApp::OnDestroy()
{
    EndOnDestroy(m_kernel);
}
