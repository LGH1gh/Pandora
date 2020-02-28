#pragma once
#include "Math.h"
#include "RendererHelper.h"
#include "ImageTools.h"

static const int FrameCount = 2;

typedef struct SDevice* Device;
typedef struct SContext* Context;
typedef struct SDescriptorHeap* DescriptorHeap;
typedef struct SCommandAllocator* CommandAllocator;
typedef struct SBlendState* BlendState;
typedef struct STexture* Texture;
typedef struct SRootSignature* RootSignature;
typedef struct SRenderPass* RenderPass;
typedef struct SPipeline* Pipeline;
typedef struct SVertexSetup* VertexSetup;
typedef struct SBuffer* Buffer;
typedef struct SResourceTable* ResourceTable;
typedef struct SSamplerTable* SamplerTable;
typedef struct SRenderSetup* RenderSetup;
typedef struct SBlob* Blob;
typedef struct SDepthStencil* DepthStencil;

typedef
enum ResourceState
{
    RESOURCE_STATE_COMMON = 0,
    RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER = 0x1,
    RESOURCE_STATE_INDEX_BUFFER = 0x2,
    RESOURCE_STATE_RENDER_TARGET = 0x4,
    RESOURCE_STATE_UNORDERED_ACCESS = 0x8,
    RESOURCE_STATE_DEPTH_WRITE = 0x10,
    RESOURCE_STATE_DEPTH_READ = 0x20,
    RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE = 0x40,
    RESOURCE_STATE_PIXEL_SHADER_RESOURCE = 0x80,
    RESOURCE_STATE_STREAM_OUT = 0x100,
    RESOURCE_STATE_INDIRECT_ARGUMENT = 0x200,
    RESOURCE_STATE_COPY_DEST = 0x400,
    RESOURCE_STATE_COPY_SOURCE = 0x800,
    RESOURCE_STATE_RESOLVE_DEST = 0x1000,
    RESOURCE_STATE_RESOLVE_SOURCE = 0x2000,
    RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE = 0x400000,
    RESOURCE_STATE_SHADING_RATE_SOURCE = 0x1000000,
    RESOURCE_STATE_GENERIC_READ = (((((0x1 | 0x2) | 0x40) | 0x80) | 0x200) | 0x800),
    RESOURCE_STATE_PRESENT = 0,
    RESOURCE_STATE_PREDICATION = 0x200,
    RESOURCE_STATE_VIDEO_DECODE_READ = 0x10000,
    RESOURCE_STATE_VIDEO_DECODE_WRITE = 0x20000,
    RESOURCE_STATE_VIDEO_PROCESS_READ = 0x40000,
    RESOURCE_STATE_VIDEO_PROCESS_WRITE = 0x80000,
    RESOURCE_STATE_VIDEO_ENCODE_READ = 0x200000,
    RESOURCE_STATE_VIDEO_ENCODE_WRITE = 0x800000
} 	ResourceState;

typedef
enum CullMode : UINT8
{
    CULL_NONE,
    CULL_FRONT,
    CULL_BACK
}   CullMode;

typedef
enum TextureType
{
    TEX_1D,
    TEX_1D_ARRAY,
    TEX_2D,
    TEX_2D_ARRAY,
    TEX_CUBE,
    TEX_CUBE_ARRAY,
    TEX_3D,
}   TextureType;

typedef
enum ComparisonFunc
{
    COMPARISON_FUNC_NEVER,
    COMPARISON_FUNC_LESS,
    COMPARISON_FUNC_EQUAL,
    COMPARISON_FUNC_LESS_EQUAL,
    COMPARISON_FUNC_GREATER,
    COMPARISON_FUNC_NOT_EQUAL,
    COMPARISON_FUNC_GREATER_EQUAL,
    COMPARISON_FUNC_ALWAYS
} 	COMPARISON_FUNC;

typedef
enum Blend
{
    BLEND_ZERO,
    BLEND_ONE,
    BLEND_SRC_COLOR,
    BLEND_INV_SRC_COLOR,
    BLEND_SRC_ALPHA,
    BLEND_INV_SRC_ALPHA,
    BLEND_DEST_ALPHA,
    BLEND_INV_DEST_ALPHA,
    BLEND_DEST_COLOR,
    BLEND_INV_DEST_COLOR0,
    BLEND_SRC_ALPHA_SAT,
    BLEND_BLEND_FACTOR,
    BLEND_INV_BLEND_FACTOR,
    BLEND_SRC1_COLOR,
    BLEND_INV_SRC1_COLOR,
    BLEND_SRC1_ALPHA,
    BLEND_INV_SRC1_ALPHA
} 	Blend;

typedef
enum BlendOperator
{
    BLEND_OP_ADD,
    BLEND_OP_SUBTRACT,
    BLEND_OP_REV_SUBTRACT,
    BLEND_OP_MIN,
    BLEND_OP_MAX
} 	BlendOperator;

typedef
enum PrimitiveTopologyType
{
    PRIMITIVE_TOPOLOGY_TYPE_POINT = 0,
    PRIMITIVE_TOPOLOGY_TYPE_LINE,
    PRIMITIVE_TOPOLOGY_TYPE_LINESTRIP,
    PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
    PRIMITIVE_TOPOLOGY_TYPE_TRIANGLELIST
} 	PrimitiveTopologyType;

typedef
enum AttributeFormat
{
    ATTRIBUTE_FORMAT_FLOAT4_32,
    ATTRIBUTE_FORMAT_FLOAT4_16,
    ATTRIBUTE_FORMAT_UINT4_32,
    ATTRIBUTE_FORMAT_UINT4_16,

    ATTRIBUTE_FORMAT_FLOAT3_32,
    ATTRIBUTE_FORMAT_UINT3_32,

    ATTRIBUTE_FORMAT_FLOAT2_32,
    ATTRIBUTE_FORMAT_FLOAT2_16,
    ATTRIBUTE_FORMAT_UINT2_32,
    ATTRIBUTE_FORMAT_UINT2_16,

    ATTRIBUTE_FORMAT_FLOAT1_32,
    ATTRIBUTE_FORMAT_UINT1_32,
   
} AttributeFormat;

struct SAttributeDesc
{
    const char* name;
    UINT stream;
    AttributeFormat format;
};

typedef
enum HeapType
{
	HEAP_TYPE_DEFAULT,
	HEAP_TYPE_UPLOAD,
	HEAP_TYPE_READBACK,
	HEAP_TYPE_CUSTOM
} 	HeapType;

typedef
enum ShaderType
{
    SHADER_TYPE_VERTEX_SHADER = 0,
    SHADER_TYPE_PIXEL_SHADER = 1,
    SHADER_TYPE_HULL_SHADER = 2,
    SHADER_TYPE_GEOMETRY_SHADER = 3,
    SHADER_TYPE_DOMAIN_SHADER = 4,
    SHADER_TYPE_COMPUTE_SHADER = 5,
}   ShaderType;

typedef
enum RenderPassFlags
{
    CLEAR_COLOR = 0x1,
    CLEAR_DEPTH = 0x2,
    FINAL_PRESENT = 0x4,
}    RenderPassFlags;

typedef
enum CompileFlags {
    COMPILE_DEBUG = (1 << 0),
    COMPILE_SKIP_VALIDATION = (1 << 1),
    COMPILE_SKIP_OPTIMIZATION = (1 << 2),
    COMPILE_PACK_MATRIX_ROW_MAJOR = (1 << 3),
    COMPILE_PACK_MATRIX_COLUMN_MAJOR = (1 << 4),
    COMPILE_PARTIAL_PRECISION = (1 << 5),
    COMPILE_FORCE_VS_SOFTWARE_NO_OPT = (1 << 6),
    COMPILE_FORCE_PS_SOFTWARE_NO_OPT = (1 << 7),
    COMPILE_NO_PRESHADER = (1 << 8),
    COMPILE_AVOID_FLOW_CONTROL = (1 << 9),
    COMPILE_PREFER_FLOW_CONTROL = (1 << 10),
    COMPILE_ENABLE_STRICTNESS = (1 << 11),
    COMPILE_ENABLE_BACKWARDS_COMPATIBILITY = (1 << 12),
    COMPILE_IEEE_STRICTNESS = (1 << 13),
    COMPILE_OPTIMIZATION_LEVEL0 = (1 << 14),
    COMPILE_OPTIMIZATION_LEVEL1 = 0,
    COMPILE_OPTIMIZATION_LEVEL2 = ((1 << 14) | (1 << 15)),
    COMPILE_OPTIMIZATION_LEVEL3 = (1 << 15),
    COMPILE_RESERVED16 = (1 << 16),
    COMPILE_RESERVED17 = (1 << 17),
    COMPILE_WARNINGS_ARE_ERRORS = (1 << 18),
    COMPILE_RESOURCES_MAY_ALIAS = (1 << 19),
    COMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES = (1 << 20),
    COMPILE_ALL_RESOURCES_BOUND = (1 << 21),
    COMPILE_DEBUG_NAME_FOR_SOURCE = (1 << 22),
    COMPILE_DEBUG_NAME_FOR_BINARY = (1 << 23),
}   CompileFlags;

struct SBlob
{
    SBlob() = default;
    SBlob(const void* address, size_t size) :
        address(address),
        size(size)
    {}


    const void* address;
    size_t size;
};

struct SubresourceParams
{
    const void* pData;
    UINT64 rowPitch;
    UINT64 slicePitch;
};

struct DeviceParams
{
    bool useWarpDevice;
    UINT width;
    UINT height;
};

struct PipelineParams
{
    RootSignature rootSignature;
    SBlob vs{ nullptr, 0 };
    SBlob gs{ nullptr, 0 };
    SBlob ps{ nullptr, 0 };

   
    PrimitiveTopologyType primitiveTopologyType = PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    const SAttributeDesc* attributes = nullptr;
    UINT attributeCount = 0;

    RenderPass renderPass;

    BlendState blendState = nullptr;
    CullMode cullMode = CULL_NONE;

    bool depthTest = false;
    bool depthWrite = false;
    ComparisonFunc depthFunc = COMPARISON_FUNC_GREATER;
};

struct BufferParams
{
    BufferParams(UINT size, HeapType heapType, ResourceState resourceState)
        : size(size)
        , heapType(heapType)
        , resourceState(resourceState)
    {
    }
    BufferParams(UINT elementSize, UINT elementCount, HeapType heapType, ResourceState resourceState)
        : size(elementSize* elementCount)
        , heapType(heapType)
        , resourceState(resourceState)
    {
    }
    UINT size;
    HeapType heapType;
    ResourceState resourceState;
};

inline ResourceState operator | (ResourceState a, ResourceState b) {
    return ResourceState
    (int(a) | int(b));
}

Device CreateDevice(DeviceParams& params, HWND hwnd);
RenderSetup CreateRenderSetup(Device device);
RootSignature CreateRootSignature(SDevice* device, UINT countantBufferNum);
Blob CreateShaderFromFile(ShaderType shaderType, LPCWSTR filePath, std::string entryPoint, UINT flags);
RenderPass CreateRenderPass(Device device, ImageFormat colorFormat, ImageFormat depthFormat, RenderPassFlags flags, UINT msaaSampler = 1);
Pipeline CreateGraphicsPipeline(SDevice* device, PipelineParams& params);
Buffer CreateBuffer(Device device, const BufferParams& params);
Buffer CreateVertexBuffer(SDevice* device, const void* pData, UINT size);
Buffer CreateIndexBuffer(SDevice* device, const void* pData, UINT size);
DescriptorHeap CreateConstantBuffer(SDevice* device, const void* pData, UINT count, UINT size);
Buffer CreateDepthStencilBuffer(SDevice* device, UINT width, UINT height);
DescriptorHeap CreateDepthStencilDescriptorBuffer(SDevice* device, SBuffer* depthStencilBuffer);

VertexSetup CreateVertexSetup(SBuffer* vertexBuffer, UINT vertexBufferStride, SBuffer* indexBuffer, UINT indexBufferStride);
BlendState CreateBlendState(Blend src, Blend dst, BlendOperator mode, UINT mask, bool alphaToCoverageEnable);

void WaitForGPU(SDevice* device);
void MoveToNextFrame(SDevice* device);
void Subresource(SDevice* device, SBuffer* destinationResource, SBuffer* intermediate, SubresourceParams& data);
void UpdateBuffer(SDescriptorHeap* descriptor, void* pData, UINT size);


void Reset(SDevice* device, SPipeline* pipeline);

void BeginRenderPass(SDevice* device, const DeviceParams& params, SDepthStencil* depthStencil = nullptr, const float* clearColor = nullptr);
void SetConstantBuffer(SDevice* device, SDescriptorHeap* descriptor);
void SetPipeline(SDevice* device, SPipeline* pipeline);
void SetGraphicsRootSignature(SDevice* device, SRootSignature* rootSignature);
void SetVertexSetup(SDevice* device, SVertexSetup* vertexSetup);

void Draw(SDevice* device, UINT start, UINT count);
void DrawIndexed(SDevice* device, UINT start, UINT count);
void DrawIndexInstanced(SDevice* device, UINT StartIndexLocation, UINT IndexCountPerInstance, UINT StartInstanceLocation, UINT InstanceCount);
void EndRenderPass(SDevice* device);

void ExecuteCommand(SDevice* device);
void Present(SDevice* device);
Context GetContext(SDevice* device);
RenderPass GetRenderPass(SDevice* device);

void Destory(SDevice* device);

DepthStencil CreateDepthStencil(SDevice* device, UINT width, UINT height);