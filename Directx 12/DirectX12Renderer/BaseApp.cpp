#include "BaseApp.h"

BaseApp::BaseApp(UINT width, UINT height, std::wstring title) :
    m_coordinate()
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
    m_rootSignature = CreateRootSignature(m_device, 1, 1);

    UINT compileFlags = COMPILE_DEBUG | COMPILE_SKIP_OPTIMIZATION;
    Blob vertexShader = CreateShaderFromFile(SHADER_TYPE_VERTEX_SHADER, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "VSMain", compileFlags);
    Blob pixelShader = CreateShaderFromFile(SHADER_TYPE_PIXEL_SHADER, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "PSMain", compileFlags);
    
    SAttributeDesc inputElementDesc[] =
    {
        { "POSITION", 0, ATTRIBUTE_FORMAT_FLOAT3_32 },
        { "TEXCOORD", 0, ATTRIBUTE_FORMAT_FLOAT2_32 },
    };

    PipelineParams pipelineParams;
    pipelineParams.attributeCount = 2;
    pipelineParams.attributes = inputElementDesc;
    pipelineParams.rootSignature = m_rootSignature;
    pipelineParams.vs = { vertexShader->address, vertexShader->size };
    pipelineParams.ps = { pixelShader->address, pixelShader->size };
    pipelineParams.cullMode = CULL_NONE;
    pipelineParams.renderPass = GetRenderPass(m_device);
    m_pipeline1 = CreateGraphicsPipeline(m_device, pipelineParams);

    Vertex triangleVertices[] = 
    {
        { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f },
        {  0.5f, -0.5f, -0.5f, 1.0f, 1.0f },
        { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f, -0.5f, 1.0f, 0.0f },

        // right side face
        {  0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f,  0.5f, 1.0f, 0.0f },
        {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f },
        {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f },

        // left side face
        { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f },
        { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f },
        { -0.5f, -0.5f,  0.5f, 0.0f, 1.0f },
        { -0.5f,  0.5f, -0.5f, 1.0f, 0.0f },

        // back face
        {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f },
        { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f },
        {  0.5f, -0.5f,  0.5f, 0.0f, 1.0f },
        { -0.5f,  0.5f,  0.5f, 1.0f, 0.0f },

        // top face
        { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f,  0.5f, 1.0f, 0.0f },
        {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f },
        { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f },

        // bottom face
        {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f },
        { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f },
        {  0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
        { -0.5f, -0.5f,  0.5f, 1.0f, 0.0f },
        //{ {  0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        //{ {  1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        //{ { -1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
        //{ { 0.0f, -1.0f, 2.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },


        //// a triangle
        //// first quad (closer to camera, blue)
        //{ {-0.5f,  0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },
        //{  {0.5f, -0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },
        //{ {-0.5f, -0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },
        //{  {0.5f,  0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },

        //// second quad (further from camera, green)
        //{ {-0.75f,  0.75f,  0.7f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
        //{   {0.0f,  0.0f, 0.7f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
        //{ {-0.75f,  0.0f, 0.7f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
        //{   {0.0f,  0.75f,  0.7f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} }
    };
    Buffer vertexBuffer = CreateVertexBuffer(m_device, triangleVertices, sizeof(triangleVertices));

    unsigned long indices[] =
    {
        // ffront face
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
        //0, 1, 2,
        //0, 1, 3,
        //0, 2, 3,
        //1, 2, 3,

        //4, 5, 6,
        //4, 7, 5,

        //8, 9, 10,
        //8, 11, 9,
    };
    Buffer indexBuffer = CreateIndexBuffer(m_device, indices, sizeof(indices));
    m_vertexSetup = CreateVertexSetup(vertexBuffer, sizeof(Vertex), indexBuffer, sizeof(unsigned long));

    m_constantBufferDesc = CreateConstantBuffer(m_device, &m_constantBufferData, 1, sizeof(m_constantBufferData));
    m_depthStencil = CreateDepthStencil(m_device, GetWidth(), GetHeight());
    m_texture = CreateTexture(m_device);
    ExecuteCommand(m_device);
}

void BaseApp::OnUpdate()
{
    static float rotateAngle = 0;
    ConstantBuffer constantBuffer = {};
    XMStoreFloat4x4(&constantBuffer.worldViewProjection, XMMatrixMultiply(m_coordinate.GetViewMatrix(), m_coordinate.GetProjectionMatrix(0.8f, static_cast<float>(GetWidth()) / static_cast<float>(GetHeight()), 1.0f, 1000.0f)));
    constantBuffer.rotateWithY = XMFLOAT4X4(
        (float)cos(rotateAngle), 0, (float)sin(rotateAngle), 0,
        0, 1, 0, 0,
        (float)-sin(rotateAngle), 0, (float)cos(rotateAngle), 0,
        0, 0, 0, 1
    );
    rotateAngle += 0.001f;
    UpdateBuffer(m_constantBufferDesc, &constantBuffer, sizeof(constantBuffer));
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
    BeginRenderPass(m_device, m_deviceParams, m_depthStencil, clearColor);
    {
        SetGraphicsRootSignature(m_device, m_rootSignature);
        SetVertexSetup(m_device, m_vertexSetup);
        SetPipeline(m_device, m_pipeline1);
        SetConstantBuffer(m_device, m_constantBufferDesc);
        //SetTextureBuffer(m_device, m_texture);
        DrawIndexed(m_device, 0, 36);
    }
    EndRenderPass(m_device);

}

void BaseApp::OnDestroy()
{
    WaitForGPU(m_device);
    Destory(m_device);
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