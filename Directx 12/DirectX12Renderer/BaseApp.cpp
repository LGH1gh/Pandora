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
    m_rootSignature = CreateRootSignature(m_device, 1);
    // m_blendState = CreateBlendState(BLEND_SRC_ALPHA, BLEND_INV_SRC_COLOR, BLEND_OP_ADD, 0xF, false);

    UINT compileFlags = COMPILE_DEBUG | COMPILE_SKIP_OPTIMIZATION;
    Blob vertexShader = CreateShaderFromFile(SHADER_TYPE_VERTEX_SHADER, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "VSMain", compileFlags);
    Blob pixelShader1 = CreateShaderFromFile(SHADER_TYPE_PIXEL_SHADER, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "PSMain1", compileFlags);
    Blob pixelShader2 = CreateShaderFromFile(SHADER_TYPE_PIXEL_SHADER, L"D:\\Pandora\\Directx 12\\DirectX12Renderer\\Shader.hlsl", "PSMain2", compileFlags);
    
    SAttributeDesc inputElementDesc[] =
    {
        { "POSITION", 0, ATTRIBUTE_FORMAT_FLOAT4_32 },
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
        { {  0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { 0.0f, -1.0f, 2.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        
        { { 0.5f, 0.5f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.5f, 0.75f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
        { { 0.75f, 0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },

        { { -0.5f, -0.5f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.5f, -0.75f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.75f, -0.75f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.75f, -0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }
    };
    Buffer vertexBuffer = CreateVertexBuffer(m_device, triangleVertices, sizeof(triangleVertices));

    unsigned long indices[] =
    {
        0, 1, 2,
        0, 1, 3,
        0, 2, 3,
        1, 2, 3
        //0, 2, 3,

        //4, 5, 6,

        //7, 8, 9,
        //7, 9, 10
    };
    Buffer indexBuffer = CreateIndexBuffer(m_device, indices, sizeof(indices));
    m_vertexSetup = CreateVertexSetup(vertexBuffer, sizeof(Vertex), indexBuffer, sizeof(unsigned long));

    m_constantBufferDesc = CreateConstantBuffer(m_device, &m_constantBufferData, 1, sizeof(m_constantBufferData));

    ExecuteCommand(m_device);
}

XMFLOAT4 MatrixXVector (XMFLOAT4X4 matrix, XMFLOAT4 vector)
{
	XMFLOAT4 result = { 0.0f, 0.0f, 0.0f, 0.0f };

	result.x = matrix._11 * vector.x + matrix._12 * vector.y + matrix._13 * vector.z + matrix._14 * vector.w;
	result.y = matrix._21 * vector.x + matrix._22 * vector.y + matrix._23 * vector.z + matrix._24 * vector.w;
	result.z = matrix._31 * vector.x + matrix._32 * vector.y + matrix._33 * vector.z + matrix._34 * vector.w;
	result.w = matrix._41 * vector.x + matrix._42 * vector.y + matrix._43 * vector.z + matrix._44 * vector.w;

    result.x /= result.w;
    result.y /= result.w;
    result.z /= result.w;
    result.w /= result.w;

	return result;
}

void BaseApp::OnUpdate()
{
    static float rotateAngle = 0.001f;
    ConstantBuffer constantBuffer = {};
    XMStoreFloat4x4(&constantBuffer.worldViewProjection, XMMatrixMultiply(m_coordinate.GetViewMatrix(), m_coordinate.GetProjectionMatrix(0.8f, static_cast<float>(GetWidth()) / static_cast<float>(GetHeight()), 1.0f, 1000.0f)));
    constantBuffer.rotateWithY = XMFLOAT4X4(
        (float)cos(rotateAngle), 0, (float)sin(rotateAngle), 0,
        0, 1, 0, 0,
        (float)-sin(rotateAngle), 0, (float)cos(rotateAngle), 0,
        0, 0, 0, 1
    );
    rotateAngle += 0.001f;
    XMMATRIX proj = m_coordinate.GetProjectionMatrix(0.8f, static_cast<float>(GetWidth()) / static_cast<float>(GetHeight()), 1.0f, 1000.0f);
    UpdateBuffer(m_constantBufferDesc, &constantBuffer, sizeof(constantBuffer));
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
        SetConstantBuffer(m_device, m_constantBufferDesc);
        DrawIndexed(m_device, 0, 3);
        DrawIndexed(m_device, 3, 3);
        DrawIndexed(m_device, 6, 3);
        DrawIndexed(m_device, 9, 3);
        /*DrawIndexed(m_device, 6, 3);
        SetPipeline(m_device, m_pipeline2);
        DrawIndexed(m_device, 9, 6);*/
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