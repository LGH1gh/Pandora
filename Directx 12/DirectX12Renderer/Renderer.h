#pragma once
#include <wincodec.h>

#include "Math.h"
#include "Font.h"
#include "RendererHelper.h"
#include "RendererStruct.h"


static const int FrameCount = 3;
static const float Color[] = { 0.0f, 0.2f, 0.4f, 1.0f };

typedef struct SKernel* Kernel;
typedef struct SRootSignature* RootSignature;
typedef struct SDescriptorHeap* DescriptorHeap;
typedef struct SFence* Fence;
typedef struct SVertexSetup* VertexSetup;
typedef struct SPipeline* Pipeline;
typedef struct SUILayer* UILayer;


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
    // BlendDesc BlendState;
    // DepthStencilDesc DepthStencilState;
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

struct FontDesc
{
    std::wstring Content;
    FontWeight FontWeight = FONT_WEIGHT_NORMAL;
    FontStyle FontStyle = FONT_STYLE_NORMAL;
    FontStretch FontStretch = FONT_STRETCH_NORMAL;
    Rect Layout;
    TextAlignment TextAlignment;
    ParagraphAlignment ParagraphAlignment;
    float lineSpacing;
    float baseline;
};


Kernel CreateKernel(UINT width, UINT height, bool useWarpDevice, HWND hwnd);
RootSignature CreateRootSignature(Kernel kernel, UINT cbvCount = 0, UINT srvCount = 0, UINT uavCount = 0, StaticSampleDesc* staticSampleDesc = nullptr);
Pipeline CreateGraphicsPipeline(Kernel kernel, GraphicsPipelineStateDesc& graphicsPipelineStateDesc);
VertexSetup CreateVertexSetup(Kernel kernel, const void* pVertexData, UINT vertexSize, UINT vertexBufferStride, const void* pIndexData, UINT indexSize, UINT indexBufferStride);

DescriptorHeap CreateDepthStencil(Kernel kernel);
DescriptorHeap CreateConstantBuffer(Kernel kernel, void* bufferData, UINT bufferSize);
DescriptorHeap CreateTexture(Kernel kernel, LPCWSTR filename);
void UpdateConstantBuffer(DescriptorHeap cbvHeap, void* bufferData, UINT bufferSize);

void EndOnInit(Kernel kernel);
void EndOnPictureRender(Kernel kernel);
void EndOnRender(Kernel kernel);
void EndOnDestroy(Kernel kernel);

void Reset(Kernel kernel, Pipeline pipeline);
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
void RenderText(Kernel kernel);

Format TranslateFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID);
WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
void LoadImageDataFromFile(ResourceDesc& resourceDesc, SubresourceData& subresourceData, LPCWSTR filename);
