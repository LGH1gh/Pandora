#pragma once
#include "stdafx.h"
#include "Font.h"
#include "RendererHelper.h"
#include "RendererStruct.h"


static const int FrameCount = 3;
static const float Color[] = { 0.0f, 0.2f, 0.4f, 1.0f };

typedef struct SKernel* Kernel;
typedef struct SComputeKernel* ComputeKernel;
typedef struct SRootSignature* RootSignature;
typedef struct SResourceHeap* ResourceHeap;
typedef struct SFence* Fence;
typedef struct SVertexSetup* VertexSetup;
typedef struct SPipeline* Pipeline;
typedef struct SUILayer* UILayer;
struct TextBlock;

struct ShaderDesc
{
    LPCWSTR filePath;
    std::string entryPoint;
    UINT flags;
    bool active = false;
    ShaderDesc(LPCWSTR filePath, std::string entryPoint, UINT flags) :
        filePath(filePath), entryPoint(entryPoint), flags(flags), active(true)
    {
    }
    ShaderDesc() = default;
};

struct GraphicsPipelineStateDesc
{
    RootSignature RootSignature;
    InputLayoutDesc InputLayout;
    ShaderDesc VS;
    ShaderDesc PS, DS, HS, GS;
    // UINT SampleMask;
    // RasterizerDesc RasterizerState;
    CullMode CullMode = CULL_MODE_NONE;
    BlendDesc BlendState = BlendDesc(Default);
    DepthStencilDesc DepthStencilState = DepthStencilDesc(Default);
    PrimitiveTopologyType PrimitiveTopologyType = PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    // UINT NumRenderTargets;
    // Format RTVFormats[8];
    // Format DSVFormats;
    // SampleDesc SampleDesc;
    // UINT NodeMask;
    //  D3D12_CACHED_PIPELINE_STATE CachedPSO;
    //  D3D12_PIPELINE_STATE_FLAGS Flags;
    //  D3D12_STREAM_OUTPUT_DESC StreamOutput;
};

struct ComputePipelineStateDesc
{
    RootSignature RootSignature;
    ShaderDesc CS;
    UINT NodeMask;
    CachedPipelineState CachedPSO;
    PipelineStateFlags Flags;
};

struct FontDesc
{
    std::wstring Content;
    FontWeight FontWeight = FONT_WEIGHT_NORMAL;
    FontStyle FontStyle = FONT_STYLE_NORMAL;
    FontStretch FontStretch = FONT_STRETCH_NORMAL;
    float FontSize = 50;
    Rect Layout;
    TextAlignment TextAlignment;
    ParagraphAlignment ParagraphAlignment;
    float LineSpacing = 50;
    float Baseline = 50;
};


Kernel CreateKernel(UINT width, UINT height, bool useWarpDevice, HWND hwnd);
ComputeKernel CreateComputeKernel(Kernel kernel);
RootSignature CreateRootSignature(Kernel kernel, UINT cbvCount = 0, UINT srvCount = 0, UINT uavCount = 0);
Pipeline CreateGraphicsPipeline(Kernel kernel, GraphicsPipelineStateDesc& graphicsPipelineStateDesc);
Pipeline CreateComputePipeline(Kernel kernel, ComputePipelineStateDesc& computePipelineStateDesc);
VertexSetup CreateVertexSetup(Kernel kernel, const void* pVertexData, UINT vertexSize, UINT vertexBufferStride, const void* pIndexData = nullptr, UINT indexSize = 0, UINT indexBufferStride = 0);

ResourceHeap CreateDepthStencilViewHeap(Kernel kernel);
ResourceHeap CreateConstantBuffer(Kernel kernel, void* bufferData, UINT bufferSize);
ResourceHeap CreateConstantBuffers(Kernel kernel, void* bufferData, UINT bufferSize, UINT bufferCount);
ResourceHeap CreateTexture(Kernel kernel, LPCWSTR filename);
ResourceHeap CreateSkybox(Kernel kernel, LPCWSTR filename);
void UpdateConstantBuffer(ResourceHeap cbvHeap, void* bufferData, UINT bufferSize);
void UpdateConstantBuffers(ResourceHeap cbvHeap, void* bufferData, UINT bufferSize, UINT bufferCount);

void EndOnInit(Kernel kernel);
void EndOnRender(Kernel kernel);
void EndOnDestroy(Kernel kernel);

void Reset(Kernel kernel, Pipeline pipeline);
void Reset(ComputeKernel computeKernel, Pipeline pipeline);
void BeginPopulateGraphicsCommand(Kernel kernel, ResourceHeap dsvHeap = nullptr, const float* clearColor = Color);
void SetGraphicsRootSignature(Kernel kernel, RootSignature rootSignature);
void SetPipeline(Kernel kernel, Pipeline pipeline);
void SetVertexSetup(Kernel kernel, VertexSetup vertexSetup);
void SetDescriptorHeaps(Kernel, std::vector<ResourceHeap>);
void SetConstantBuffer(Kernel kernel, ResourceHeap heap);
void SetTexture(Kernel kernel, ResourceHeap heap);
void SetConstantBuffer(Kernel kernel, ResourceHeap heap, UINT index);
void SetShaderResource(Kernel kernel, ResourceHeap heap);
void Draw(Kernel kernel, UINT StartVertexLocation, UINT VertexCountPerInstance);
void DrawIndexed(Kernel kernel, UINT StartIndexLocation, UINT IndexCountPerInstance);
void DrawIndexInstanced(Kernel kernel, UINT StartIndexLocation, UINT IndexCountPerInstance, UINT StartInstanceLocation, UINT InstanceCount);
void EndPopulateGraphicsCommand(Kernel kernel);
void RenderText(Kernel kernel, std::vector<FontDesc> texts);

Format TranslateFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID);
WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
void LoadImageDataFromFile(ResourceDesc& resourceDesc, SubresourceData& subresourceData, LPCWSTR filename);

ResourceHeap CreateComputeBuffer(Kernel kernel, void* bufferData, UINT bufferCount, UINT bufferSize);
void BeginPopulateComputeCommand(ComputeKernel computeKernel, ResourceHeap descriptorHeap);
void SetComputePipeline(ComputeKernel computeKernel, Pipeline pipeline);
void SetComputeRootSignature(ComputeKernel computeKernel, RootSignature rootSignature);
void SetComputeBuffer(ComputeKernel computeKernel, ResourceHeap descriptorHeap);
void SetConstantBuffer(ComputeKernel computeKernel, ResourceHeap descriptorHeap);
void EndPopulateComputeCommand(ComputeKernel computeKernel, ResourceHeap descriptorHeap, const UINT dispatch[]);
void ExecuteCommand(Kernel kernel);
void ExecuteCommand(ComputeKernel computeKernel);