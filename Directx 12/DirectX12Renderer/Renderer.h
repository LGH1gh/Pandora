#pragma once
#include <wincodec.h>

#include "Math.h"
#include "Font.h"
#include "RendererHelper.h"
#include "RendererStruct.h"


static const int FrameCount = 3;
static const float Color[] = { 0.0f, 0.2f, 0.4f, 1.0f };

typedef struct SKernel* Kernel;
typedef struct SComputeKernel* ComputeKernel;
typedef struct SRootSignature* RootSignature;
typedef struct SDescriptorHeap* DescriptorHeap;
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
RootSignature CreateRootSignature(Kernel kernel, UINT cbvCount = 0, UINT srvCount = 0, UINT uavCount = 0, StaticSampleDesc* staticSampleDesc = nullptr);
Pipeline CreateGraphicsPipeline(Kernel kernel, GraphicsPipelineStateDesc& graphicsPipelineStateDesc);
Pipeline CreateComputePipeline(Kernel kernel, ComputePipelineStateDesc& computePipelineStateDesc);
VertexSetup CreateVertexSetup(Kernel kernel, const void* pVertexData, UINT vertexSize, UINT vertexBufferStride, const void* pIndexData = nullptr, UINT indexSize = 0, UINT indexBufferStride = 0);

DescriptorHeap CreateDepthStencil(Kernel kernel);
DescriptorHeap CreateConstantBuffer(Kernel kernel, void* bufferData, UINT bufferSize);
DescriptorHeap CreateTexture(Kernel kernel, LPCWSTR filename);
void UpdateConstantBuffer(DescriptorHeap cbvHeap, void* bufferData, UINT bufferSize);

void EndOnInit(Kernel kernel);
void EndOnPictureRender(Kernel kernel);
void EndOnRender(Kernel kernel);
void EndOnDestroy(Kernel kernel, ComputeKernel computeKernel = nullptr);

void WaitForComputeShader(Kernel kernel, ComputeKernel computeKernel);
void AsyncComputeAndGraphicsThread(Kernel kernel, ComputeKernel computeKernel);

void Reset(Kernel kernel, Pipeline pipeline);
void Reset(ComputeKernel computeKernel, Pipeline pipeline);
void BeginRender(Kernel kernel, DescriptorHeap dsvHeap = nullptr, const float* clearColor = Color);
void SetGraphicsRootSignature(Kernel kernel, RootSignature rootSignature);
void SetPipeline(Kernel kernel, Pipeline pipeline);
void SetVertexSetup(Kernel kernel, VertexSetup vertexSetup);
void SetConstantBuffer(Kernel kernel, DescriptorHeap heap);
void SetShaderResource(Kernel kernel, DescriptorHeap heap);
void Draw(Kernel kernel, UINT StartVertexLocation, UINT VertexCountPerInstance);
void DrawIndexed(Kernel kernel, UINT StartIndexLocation, UINT IndexCountPerInstance);
void DrawIndexInstanced(Kernel kernel, UINT StartIndexLocation, UINT IndexCountPerInstance, UINT StartInstanceLocation, UINT InstanceCount);
void EndRender(Kernel kernel);
void RenderText(Kernel kernel, std::vector<FontDesc> texts);

Format TranslateFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID);
WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
void LoadImageDataFromFile(ResourceDesc& resourceDesc, SubresourceData& subresourceData, LPCWSTR filename);

DescriptorHeap CreateComputeBuffer(Kernel kernel, void* bufferData, UINT bufferCount, UINT bufferSize);
void BeginComputeShader(ComputeKernel computeKernel, DescriptorHeap descriptorHeap);
void SetComputePipeline(ComputeKernel computeKernel, Pipeline pipeline);
void SetComputeRootSignature(ComputeKernel computeKernel, RootSignature rootSignature);
void SetComputeBuffer(ComputeKernel computeKernel, DescriptorHeap descriptorHeap);
void SetConstantBuffer(ComputeKernel computeKernel, DescriptorHeap descriptorHeap);
void EndComputeShader(ComputeKernel computeKernel, DescriptorHeap descriptorHeap, const UINT dispatch[]);