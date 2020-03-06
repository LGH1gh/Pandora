#pragma once
#include <wincodec.h>

#include "Math.h"
#include "RendererHelper.h"
#include "RendererStruct.h"


static const int FrameCount = 2;
static const float Color[] = { 0.0f, 0.2f, 0.4f, 1.0f };
//
//typedef struct SBlendState* BlendState;
//typedef struct STexture* Texture;
//typedef struct SRenderPass* RenderPass;
//typedef struct SBuffer* Buffer;
//typedef struct SResourceTable* ResourceTable;
//typedef struct SSamplerTable* SamplerTable;
//typedef struct SRenderSetup* RenderSetup;
//typedef struct SDepthStencil* DepthStencil;
//typedef struct STexture* Texture;
//
//
//
//
//typedef
//enum ResourceState
//{
//    RESOURCE_STATE_COMMON = 0,
//    RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER = 0x1,
//    RESOURCE_STATE_INDEX_BUFFER = 0x2,
//    RESOURCE_STATE_RENDER_TARGET = 0x4,
//    RESOURCE_STATE_UNORDERED_ACCESS = 0x8,
//    RESOURCE_STATE_DEPTH_WRITE = 0x10,
//    RESOURCE_STATE_DEPTH_READ = 0x20,
//    RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE = 0x40,
//    RESOURCE_STATE_PIXEL_SHADER_RESOURCE = 0x80,
//    RESOURCE_STATE_STREAM_OUT = 0x100,
//    RESOURCE_STATE_INDIRECT_ARGUMENT = 0x200,
//    RESOURCE_STATE_COPY_DEST = 0x400,
//    RESOURCE_STATE_COPY_SOURCE = 0x800,
//    RESOURCE_STATE_RESOLVE_DEST = 0x1000,
//    RESOURCE_STATE_RESOLVE_SOURCE = 0x2000,
//    RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE = 0x400000,
//    RESOURCE_STATE_SHADING_RATE_SOURCE = 0x1000000,
//    RESOURCE_STATE_GENERIC_READ = (((((0x1 | 0x2) | 0x40) | 0x80) | 0x200) | 0x800),
//    RESOURCE_STATE_PRESENT = 0,
//    RESOURCE_STATE_PREDICATION = 0x200,
//    RESOURCE_STATE_VIDEO_DECODE_READ = 0x10000,
//    RESOURCE_STATE_VIDEO_DECODE_WRITE = 0x20000,
//    RESOURCE_STATE_VIDEO_PROCESS_READ = 0x40000,
//    RESOURCE_STATE_VIDEO_PROCESS_WRITE = 0x80000,
//    RESOURCE_STATE_VIDEO_ENCODE_READ = 0x200000,
//    RESOURCE_STATE_VIDEO_ENCODE_WRITE = 0x800000
//} 	ResourceState;
//
//
//
//typedef
//enum TextureType
//{
//    TEX_1D,
//    TEX_1D_ARRAY,
//    TEX_2D,
//    TEX_2D_ARRAY,
//    TEX_CUBE,
//    TEX_CUBE_ARRAY,
//    TEX_3D,
//}   TextureType;
//
//
//
//typedef
//enum Blend
//{
//    BLEND_ZERO,
//    BLEND_ONE,
//    BLEND_SRC_COLOR,
//    BLEND_INV_SRC_COLOR,
//    BLEND_SRC_ALPHA,
//    BLEND_INV_SRC_ALPHA,
//    BLEND_DEST_ALPHA,
//    BLEND_INV_DEST_ALPHA,
//    BLEND_DEST_COLOR,
//    BLEND_INV_DEST_COLOR0,
//    BLEND_SRC_ALPHA_SAT,
//    BLEND_BLEND_FACTOR,
//    BLEND_INV_BLEND_FACTOR,
//    BLEND_SRC1_COLOR,
//    BLEND_INV_SRC1_COLOR,
//    BLEND_SRC1_ALPHA,
//    BLEND_INV_SRC1_ALPHA
//} 	Blend;
//
//typedef
//enum BlendOperator
//{
//    BLEND_OP_ADD,
//    BLEND_OP_SUBTRACT,
//    BLEND_OP_REV_SUBTRACT,
//    BLEND_OP_MIN,
//    BLEND_OP_MAX
//} 	BlendOperator;
//
//
//
//typedef
//enum AttributeFormat
//{
//    ATTRIBUTE_FORMAT_FLOAT4_32,
//    ATTRIBUTE_FORMAT_FLOAT4_16,
//    ATTRIBUTE_FORMAT_UINT4_32,
//    ATTRIBUTE_FORMAT_UINT4_16,
//
//    ATTRIBUTE_FORMAT_FLOAT3_32,
//    ATTRIBUTE_FORMAT_UINT3_32,
//
//    ATTRIBUTE_FORMAT_FLOAT2_32,
//    ATTRIBUTE_FORMAT_FLOAT2_16,
//    ATTRIBUTE_FORMAT_UINT2_32,
//    ATTRIBUTE_FORMAT_UINT2_16,
//
//    ATTRIBUTE_FORMAT_FLOAT1_32,
//    ATTRIBUTE_FORMAT_UINT1_32,
//   
//} AttributeFormat;
//
//struct SAttributeDesc
//{
//    const char* name;
//    UINT stream;
//    AttributeFormat format;
//};
//
//typedef
//enum HeapType
//{
//	HEAP_TYPE_DEFAULT,
//	HEAP_TYPE_UPLOAD,
//	HEAP_TYPE_READBACK,
//	HEAP_TYPE_CUSTOM
//} 	HeapType;
//

//
//typedef
//enum RenderPassFlags
//{
//    CLEAR_COLOR = 0x1,
//    CLEAR_DEPTH = 0x2,
//    FINAL_PRESENT = 0x4,
//}    RenderPassFlags;
//
//
//
//
//struct SubresourceParams
//{
//    const void* pData;
//    UINT64 rowPitch;
//    UINT64 slicePitch;
//};
//
//struct DeviceParams
//{
//    bool useWarpDevice;
//    UINT width;
//    UINT height;
//};
//
//
//struct BufferParams
//{
//    BufferParams(UINT size, HeapType heapType, ResourceState resourceState)
//        : size(size)
//        , heapType(heapType)
//        , resourceState(resourceState)
//    {
//    }
//    BufferParams(UINT elementSize, UINT elementCount, HeapType heapType, ResourceState resourceState)
//        : size(elementSize* elementCount)
//        , heapType(heapType)
//        , resourceState(resourceState)
//    {
//    }
//    UINT size;
//    HeapType heapType;
//    ResourceState resourceState;
//};
//
//inline ResourceState operator | (ResourceState a, ResourceState b) {
//    return ResourceState
//    (int(a) | int(b));
//}
//
//Device CreateDevice(DeviceParams& params, HWND hwnd);
//RenderSetup CreateRenderSetup(Device device);
//Blob CreateShaderFromFile(ShaderType shaderType, LPCWSTR filePath, std::string entryPoint, UINT flags);
//RenderPass CreateRenderPass(Device device, Format colorFormat, Format depthFormat, RenderPassFlags flags, UINT msaaSampler = 1);
//Buffer CreateBuffer(Device device, const BufferParams& params);
//Buffer CreateVertexBuffer(SDevice* device, const void* pData, UINT size);
//Buffer CreateIndexBuffer(SDevice* device, const void* pData, UINT size);
//DescriptorHeap CreateConstantBuffer(SDevice* device, const void* pData, UINT count, UINT size);
//DepthStencil CreateDepthStencil(SDevice* device, UINT width, UINT height);
//BlendState CreateBlendState(Blend src, Blend dst, BlendOperator mode, UINT mask, bool alphaToCoverageEnable);
//
//void WaitForGPU(SDevice* device);
//void MoveToNextFrame(SDevice* device);
//void Subresource(SDevice* device, SBuffer* destinationResource, SBuffer* intermediate, SubresourceParams& data);
//void UpdateBuffer(SDescriptorHeap* descriptor, void* pData, UINT size);
//
//
//void Reset(SDevice* device, SPipeline* pipeline);
//
//void BeginRenderPass(SDevice* device, const DeviceParams& params, SDepthStencil* depthStencil = nullptr, const float* clearColor = nullptr);
//void SetConstantBuffer(SDevice* device, SDescriptorHeap* descriptor);
//void SetTextureBuffer(SDevice* device, SDescriptorHeap* descriptor);
//void SetPipeline(SDevice* device, SPipeline* pipeline);
//void SetGraphicsRootSignature(SDevice* device, SRootSignature* rootSignature);
//void SetVertexSetup(SDevice* device, SVertexSetup* vertexSetup);
//
//void Draw(SDevice* device, UINT start, UINT count);
//void DrawIndexed(SDevice* device, UINT start, UINT count);
//void DrawIndexInstanced(SDevice* device, UINT StartIndexLocation, UINT IndexCountPerInstance, UINT StartInstanceLocation, UINT InstanceCount);
//void EndRenderPass(SDevice* device);
//
//void ExecuteCommand(SDevice* device);
//void Present(SDevice* device);
//Context GetContext(SDevice* device);
//RenderPass GetRenderPass(SDevice* device);
//
//void Destory(SDevice* device);
//DescriptorHeap CreateTexture(SDevice* device);
//void SetDescriptorHeaps(SDevice* device, DescriptorHeap heap1, DescriptorHeap heap2);


typedef struct SKernel* Kernel;
typedef struct SRootSignature* RootSignature;
typedef struct SDescriptorHeap* DescriptorHeap;
typedef struct SResource* Resource;
typedef struct SFence* Fence;
typedef struct SVertexSetup* VertexSetup;
typedef struct SPipeline* Pipeline;


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


Kernel CreateKernel(UINT width, UINT height, bool useWarpDevice, HWND hwnd);
RootSignature CreateRootSignature(Kernel kernel, UINT cbvCount = 0, UINT srvCount = 0, UINT uavCount = 0, StaticSampleDesc* staticSampleDesc = nullptr);
Pipeline CreateGraphicsPipeline(Kernel kernel, GraphicsPipelineStateDesc& graphicsPipelineStateDesc);
VertexSetup CreateVertexSetup(Kernel kernel, const void* pVertexData, UINT vertexSize, UINT vertexBufferStride, const void* pIndexData, UINT indexSize, UINT indexBufferStride);

DescriptorHeap CreateDepthStencil(Kernel kernel);
DescriptorHeap CreateConstantBuffer(Kernel kernel, void* bufferData, UINT bufferSize);
<<<<<<< HEAD
DescriptorHeap CreateTexture(Kernel kernel, LPCWSTR filename);
=======
DescriptorHeap CreateTexture(Kernel kernel);
>>>>>>> de208ab0556ed22f3464c195743df09e49237a0b
void UpdateConstantBuffer(DescriptorHeap cbvHeap, void* bufferData, UINT bufferSize);

void EndOnInit(Kernel kernel);
void EndOnRender(Kernel kernel);
void EndOnDestory(Kernel kernel);

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

Format TranslateFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID);
WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
void LoadImageDataFromFile(ResourceDesc& resourceDesc, SubresourceData& subresourceData, LPCWSTR filename);
