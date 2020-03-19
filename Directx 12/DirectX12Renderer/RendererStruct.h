#pragma once

#include <intsafe.h>
#include <minwindef.h>

// RasterizerDesc //
struct DEFAULT {};
extern const DECLSPEC_SELECTANY DEFAULT Default;

typedef
enum LogicOp
{
    LOGIC_OP_CLEAR = 0,
    LOGIC_OP_SET = (LOGIC_OP_CLEAR + 1),
    LOGIC_OP_COPY = (LOGIC_OP_SET + 1),
    LOGIC_OP_COPY_INVERTED = (LOGIC_OP_COPY + 1),
    LOGIC_OP_NOOP = (LOGIC_OP_COPY_INVERTED + 1),
    LOGIC_OP_INVERT = (LOGIC_OP_NOOP + 1),
    LOGIC_OP_AND = (LOGIC_OP_INVERT + 1),
    LOGIC_OP_NAND = (LOGIC_OP_AND + 1),
    LOGIC_OP_OR = (LOGIC_OP_NAND + 1),
    LOGIC_OP_NOR = (LOGIC_OP_OR + 1),
    LOGIC_OP_XOR = (LOGIC_OP_NOR + 1),
    LOGIC_OP_EQUIV = (LOGIC_OP_XOR + 1),
    LOGIC_OP_AND_REVERSE = (LOGIC_OP_EQUIV + 1),
    LOGIC_OP_AND_INVERTED = (LOGIC_OP_AND_REVERSE + 1),
    LOGIC_OP_OR_REVERSE = (LOGIC_OP_AND_INVERTED + 1),
    LOGIC_OP_OR_INVERTED = (LOGIC_OP_OR_REVERSE + 1)
} 	LogicOp;

typedef
enum CullMode
{
    CULL_MODE_NONE = 1,
    CULL_MODE_FRONT = 2,
    CULL_MODE_BACK = 3
}   CullMode;

typedef
enum FillMode
{
    FILL_MODE_WIREFRAME = 2,
    FILL_MODE_SOLID = 3
} 	FillMode;

typedef
enum ConservativeRasterizationMode
{
    CONSERVATIVE_RASTERIZATION_MODE_OFF = 0,
    CONSERVATIVE_RASTERIZATION_MODE_ON = 1
} 	ConservativeRasterizationMode;

typedef
struct RasterizerDesc
{
    //  FillMode FillMode;
    CullMode CullMode;
    BOOL FrontCounterClockwise;
    INT DepthBias;
    FLOAT DepthBiasClamp;
    FLOAT SlopeScaledDepthBias;
    //  BOOL DepthClipEnable;
    //  BOOL MultisampleEnable;
    BOOL AntialiasedLineEnable;
    UINT ForcedSampleCount;
    ConservativeRasterizationMode ConservativeRaster;
} 	RasterizerDesc;


typedef
enum Blend
{
    BLEND_ZERO = 1,
    BLEND_ONE = 2,
    BLEND_SRC_COLOR = 3,
    BLEND_INV_SRC_COLOR = 4,
    BLEND_SRC_ALPHA = 5,
    BLEND_INV_SRC_ALPHA = 6,
    BLEND_DEST_ALPHA = 7,
    BLEND_INV_DEST_ALPHA = 8,
    BLEND_DEST_COLOR = 9,
    BLEND_INV_DEST_COLOR = 10,
    BLEND_SRC_ALPHA_SAT = 11,
    BLEND_BLEND_FACTOR = 14,
    BLEND_INV_BLEND_FACTOR = 15,
    BLEND_SRC1_COLOR = 16,
    BLEND_INV_SRC1_COLOR = 17,
    BLEND_SRC1_ALPHA = 18,
    BLEND_INV_SRC1_ALPHA = 19
} 	Blend;

typedef
enum BlendOp
{
    BLEND_OP_ADD = 1,
    BLEND_OP_SUBTRACT = 2,
    BLEND_OP_REV_SUBTRACT = 3,
    BLEND_OP_MIN = 4,
    BLEND_OP_MAX = 5
} 	BlendOp;

typedef
enum ColorWriteEnable
{
    COLOR_WRITE_ENABLE_RED = 1,
    COLOR_WRITE_ENABLE_GREEN = 2,
    COLOR_WRITE_ENABLE_BLUE = 4,
    COLOR_WRITE_ENABLE_ALPHA = 8,
    COLOR_WRITE_ENABLE_ALL = (((COLOR_WRITE_ENABLE_RED | COLOR_WRITE_ENABLE_GREEN) | COLOR_WRITE_ENABLE_BLUE) | COLOR_WRITE_ENABLE_ALPHA)
} 	ColorWriteEnable;

typedef
enum PipelineStateFlags
{
    PIPELINE_STATE_FLAG_NONE = 0,
    PIPELINE_STATE_FLAG_TOOL_DEBUG = 0x1
} 	PipelineStateFlags;


typedef
struct RenderTargetBlendDesc
{
    BlendOp BlendOpAlpha;
    LogicOp LogicOp;
    BOOL BlendEnable;
    BOOL LogicOpEnable;
    Blend SrcBlend;
    Blend DestBlend;
    BlendOp BlendOp;
    Blend SrcBlendAlpha;
    Blend DestBlendAlpha;
    UINT8 RenderTargetWriteMask;
}   RenderTargetBlendDesc;

typedef
struct BlendDesc
{
    BOOL AlphaToCoverageEnable;
    BOOL IndependentBlendEnable;
    RenderTargetBlendDesc RenderTarget[8];
    explicit BlendDesc(DEFAULT)
    {
        AlphaToCoverageEnable = FALSE;
        IndependentBlendEnable = FALSE;
        const RenderTargetBlendDesc defaultRenderTargetBlendDesc =
        {
            BLEND_OP_ADD, LOGIC_OP_NOOP,
            FALSE,FALSE,
            BLEND_ONE, BLEND_ZERO, BLEND_OP_ADD,
            BLEND_ONE, BLEND_ZERO, 
            COLOR_WRITE_ENABLE_ALL,
        };
        for (UINT i = 0; i < 8; ++i)
            RenderTarget[i] = defaultRenderTargetBlendDesc;
    }
}   BlendDesc;

typedef
enum ComparisonFunc
{
    COMPARISON_FUNC_NEVER = 1,
    COMPARISON_FUNC_LESS,
    COMPARISON_FUNC_EQUAL,
    COMPARISON_FUNC_LESS_EQUAL,
    COMPARISON_FUNC_GREATER,
    COMPARISON_FUNC_NOT_EQUAL,
    COMPARISON_FUNC_GREATER_EQUAL,
    COMPARISON_FUNC_ALWAYS
} 	ComparisonFunc;

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


typedef
enum TextureAddressMode
{
    TEXTURE_ADDRESS_MODE_WRAP = 1,
    TEXTURE_ADDRESS_MODE_MIRROR = 2,
    TEXTURE_ADDRESS_MODE_CLAMP = 3,
    TEXTURE_ADDRESS_MODE_BORDER = 4,
    TEXTURE_ADDRESS_MODE_MIRROR_ONCE = 5
} 	TextureAddressMode;

typedef
enum TextureLayout
{
    TEXTURE_LAYOUT_UNKNOWN = 0,
    TEXTURE_LAYOUT_ROW_MAJOR = 1,
    TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE = 2,
    TEXTURE_LAYOUT_64KB_STANDARD_SWIZZLE = 3
} 	TextureLayout;

typedef
enum Format
{
    FORMAT_UNKNOWN = 0,

    FORMAT_R32G32B32A32_TYPELESS = 1,
    FORMAT_R32G32B32A32_FLOAT = 2,
    FORMAT_R32G32B32A32_UINT = 3,
    FORMAT_R32G32B32A32_SINT = 4,

    FORMAT_R32G32B32_TYPELESS = 5,
    FORMAT_R32G32B32_FLOAT = 6,
    FORMAT_R32G32B32_UINT = 7,
    FORMAT_R32G32B32_SINT = 8,

    FORMAT_R16G16B16A16_TYPELESS = 9,
    FORMAT_R16G16B16A16_FLOAT = 10,
    FORMAT_R16G16B16A16_UNORM = 11,
    FORMAT_R16G16B16A16_UINT = 12,
    FORMAT_R16G16B16A16_SNORM = 13,
    FORMAT_R16G16B16A16_SINT = 14,
    FORMAT_R32G32_TYPELESS = 15,
    FORMAT_R32G32_FLOAT = 16,
    FORMAT_R32G32_UINT = 17,
    FORMAT_R32G32_SINT = 18,
    FORMAT_R32G8X24_TYPELESS = 19,
    FORMAT_D32_FLOAT_S8X24_UINT = 20,
    FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
    FORMAT_X32_TYPELESS_G8X24_UINT = 22,

    FORMAT_R10G10B10A2_TYPELESS = 23,
    FORMAT_R10G10B10A2_UNORM = 24,
    FORMAT_R10G10B10A2_UINT = 25,
    FORMAT_R11G11B10_FLOAT = 26,
    FORMAT_R8G8B8A8_TYPELESS = 27,
    FORMAT_R8G8B8A8_UNORM = 28,
    FORMAT_R8G8B8A8_UNORM_SRGB = 29,
    FORMAT_R8G8B8A8_UINT = 30,
    FORMAT_R8G8B8A8_SNORM = 31,
    FORMAT_R8G8B8A8_SINT = 32,
    FORMAT_R16G16_TYPELESS = 33,
    FORMAT_R16G16_FLOAT = 34,
    FORMAT_R16G16_UNORM = 35,
    FORMAT_R16G16_UINT = 36,
    FORMAT_R16G16_SNORM = 37,
    FORMAT_R16G16_SINT = 38,
    FORMAT_R32_TYPELESS = 39,
    FORMAT_D32_FLOAT = 40,
    FORMAT_R32_FLOAT = 41,
    FORMAT_R32_UINT = 42,
    FORMAT_R32_SINT = 43,
    FORMAT_R24G8_TYPELESS = 44,
    FORMAT_D24_UNORM_S8_UINT = 45,
    FORMAT_R24_UNORM_X8_TYPELESS = 46,
    FORMAT_X24_TYPELESS_G8_UINT = 47,

    FORMAT_R8G8_TYPELESS = 48,
    FORMAT_R8G8_UNORM = 49,
    FORMAT_R8G8_UINT = 50,
    FORMAT_R8G8_SNORM = 51,
    FORMAT_R8G8_SINT = 52,
    FORMAT_R16_TYPELESS = 53,
    FORMAT_R16_FLOAT = 54,
    FORMAT_D16_UNORM = 55,
    FORMAT_R16_UNORM = 56,
    FORMAT_R16_UINT = 57,
    FORMAT_R16_SNORM = 58,
    FORMAT_R16_SINT = 59,

    FORMAT_R8_TYPELESS = 60,
    FORMAT_R8_UNORM = 61,
    FORMAT_R8_UINT = 62,
    FORMAT_R8_SNORM = 63,
    FORMAT_R8_SINT = 64,
    FORMAT_A8_UNORM = 65,

    FORMAT_R1_UNORM = 66,
    FORMAT_R9G9B9E5_SHAREDEXP = 67,
    FORMAT_R8G8_B8G8_UNORM = 68,
    FORMAT_G8R8_G8B8_UNORM = 69,
    FORMAT_BC1_TYPELESS = 70,
    FORMAT_BC1_UNORM = 71,
    FORMAT_BC1_UNORM_SRGB = 72,
    FORMAT_BC2_TYPELESS = 73,
    FORMAT_BC2_UNORM = 74,
    FORMAT_BC2_UNORM_SRGB = 75,
    FORMAT_BC3_TYPELESS = 76,
    FORMAT_BC3_UNORM = 77,
    FORMAT_BC3_UNORM_SRGB = 78,
    FORMAT_BC4_TYPELESS = 79,
    FORMAT_BC4_UNORM = 80,
    FORMAT_BC4_SNORM = 81,
    FORMAT_BC5_TYPELESS = 82,
    FORMAT_BC5_UNORM = 83,
    FORMAT_BC5_SNORM = 84,
    FORMAT_B5G6R5_UNORM = 85,
    FORMAT_B5G5R5A1_UNORM = 86,
    FORMAT_B8G8R8A8_UNORM = 87,
    FORMAT_B8G8R8X8_UNORM = 88,
    FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
    FORMAT_B8G8R8A8_TYPELESS = 90,
    FORMAT_B8G8R8A8_UNORM_SRGB = 91,
    FORMAT_B8G8R8X8_TYPELESS = 92,
    FORMAT_B8G8R8X8_UNORM_SRGB = 93,
    FORMAT_BC6H_TYPELESS = 94,
    FORMAT_BC6H_UF16 = 95,
    FORMAT_BC6H_SF16 = 96,
    FORMAT_BC7_TYPELESS = 97,
    FORMAT_BC7_UNORM = 98,
    FORMAT_BC7_UNORM_SRGB = 99,
    FORMAT_AYUV = 100,
    FORMAT_Y410 = 101,
    FORMAT_Y416 = 102,
    FORMAT_NV12 = 103,
    FORMAT_P010 = 104,
    FORMAT_P016 = 105,
    FORMAT_420_OPAQUE = 106,
    FORMAT_YUY2 = 107,
    FORMAT_Y210 = 108,
    FORMAT_Y216 = 109,
    FORMAT_NV11 = 110,
    FORMAT_AI44 = 111,
    FORMAT_IA44 = 112,
    FORMAT_P8 = 113,
    FORMAT_A8P8 = 114,
    FORMAT_B4G4R4A4_UNORM = 115,

    FORMAT_P208 = 130,
    FORMAT_V208 = 131,
    FORMAT_V408 = 132,


    FORMAT_FORCE_UINT = 0xffffffff
}   Format;



typedef
enum Filter
{
    FILTER_MIN_MAG_MIP_POINT = 0,
    FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
    FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
    FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
    FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
    FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
    FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
    FILTER_MIN_MAG_MIP_LINEAR = 0x15,
    FILTER_ANISOTROPIC = 0x55,
    FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
    FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
    FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
    FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
    FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
    FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
    FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
    FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
    FILTER_COMPARISON_ANISOTROPIC = 0xd5,
    FILTER_MINIMUM_MIN_MAG_MIP_POINT = 0x100,
    FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x101,
    FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x104,
    FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x105,
    FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x110,
    FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x111,
    FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x114,
    FILTER_MINIMUM_MIN_MAG_MIP_LINEAR = 0x115,
    FILTER_MINIMUM_ANISOTROPIC = 0x155,
    FILTER_MAXIMUM_MIN_MAG_MIP_POINT = 0x180,
    FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x181,
    FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x184,
    FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x185,
    FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x190,
    FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x191,
    FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x194,
    FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR = 0x195,
    FILTER_MAXIMUM_ANISOTROPIC = 0x1d5
} 	Filter;


static UINT FormatToBits[] =
{
    0,
    128, 128, 128, 128,
    96, 96, 96, 96,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    8, 8, 8, 8, 8, 8,
};




// Depth Stencil Desc //
typedef
enum DepthWriteMask
{
    DEPTH_WRITE_MASK_ZERO = 0,
    DEPTH_WRITE_MASK_ALL = 1
} 	DepthWriteMask;

typedef
enum StencilOp
{
    STENCIL_OP_KEEP = 1,
    STENCIL_OP_ZERO = 2,
    STENCIL_OP_REPLACE = 3,
    STENCIL_OP_INCR_SAT = 4,
    STENCIL_OP_DECR_SAT = 5,
    STENCIL_OP_INVERT = 6,
    STENCIL_OP_INCR = 7,
    STENCIL_OP_DECR = 8
} 	StencilOp;

typedef struct DepthStencilOpDesc
{
    StencilOp StencilFailOp;
    StencilOp StencilDepthFailOp;
    StencilOp StencilPassOp;
    ComparisonFunc StencilFunc;
} 	DepthStencilOpDesc;

typedef struct DepthStencilDesc
{
    BOOL DepthEnable;
    DepthWriteMask DepthWriteMask;
    ComparisonFunc DepthFunc;
    BOOL StencilEnable;
    UINT8 StencilReadMask;
    UINT8 StencilWriteMask;
    DepthStencilOpDesc FrontFace;
    DepthStencilOpDesc BackFace;
    
    DepthStencilDesc(DEFAULT)
    {
        DepthEnable = TRUE;
        DepthWriteMask = DEPTH_WRITE_MASK_ALL;
        DepthFunc = COMPARISON_FUNC_LESS;
        StencilEnable = FALSE;
        StencilReadMask = 0xff;
        StencilWriteMask = 0xff;
        const DepthStencilOpDesc defaultStencilOp =
        { STENCIL_OP_KEEP, STENCIL_OP_KEEP, STENCIL_OP_KEEP, COMPARISON_FUNC_ALWAYS };
        FrontFace = defaultStencilOp;
        BackFace = defaultStencilOp;
    }
} 	DepthStencilDesc;


// Input Layout Desc
typedef
enum InputClassification
{
    INPUT_CLASSIFICATION_PER_VERTEX_DATA = 0,
    INPUT_CLASSIFICATION_PER_INSTANCE_DATA = 1
} 	InputClassification;

typedef struct InputElementDesc
{
    LPCSTR SemanticName;
    //  UINT SemanticIndex;
    Format Format;
    //  UINT InputSlot;
    //  UINT AlignedByteOffset;
    //  InputClassification InputSlotClass;
    //  UINT InstanceDataStepRate;
} 	InputElementDesc;

typedef struct InputLayoutDesc
{
    _Field_size_full_(NumElements)  const InputElementDesc* pInputElementDescs;
    UINT NumElements;
} 	InputLayoutDesc;


// Primitive Topology Type //
typedef
enum PrimitiveTopologyType
{
    PRIMITIVE_TOPOLOGY_TYPE_POINT = 0,
    PRIMITIVE_TOPOLOGY_TYPE_LINE = 1,
    PRIMITIVE_TOPOLOGY_TYPE_LINESTRIP = 2,
    PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE = 3,
    PRIMITIVE_TOPOLOGY_TYPE_TRIANGLELIST = 4
} 	PrimitiveTopologyType;


// Sample Desc //
typedef struct SampleDesc
{
    UINT Count;
    UINT Quality;
} SampleDesc;



typedef
enum ResourceDimension
{
    RESOURCE_DIMENSION_UNKNOWN = 0,
    RESOURCE_DIMENSION_BUFFER = 1,
    RESOURCE_DIMENSION_TEXTURE1D = 2,
    RESOURCE_DIMENSION_TEXTURE2D = 3,
    RESOURCE_DIMENSION_TEXTURE3D = 4
} 	ResourceDimension;

typedef
enum ResourceFlags
{
    RESOURCE_FLAG_NONE = 0,
    RESOURCE_FLAG_ALLOW_RENDER_TARGET = 0x1,
    RESOURCE_FLAG_ALLOW_DEPTH_STENCIL = 0x2,
    RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS = 0x4,
    RESOURCE_FLAG_DENY_SHADER_RESOURCE = 0x8,
    RESOURCE_FLAG_ALLOW_CROSS_ADAPTER = 0x10,
    RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS = 0x20,
    RESOURCE_FLAG_VIDEO_DECODE_REFERENCE_ONLY = 0x40
} 	ResourceFlags;

typedef
enum RootSignatureFlags
{
    ROOT_SIGNATURE_FLAG_NONE = 0,
    ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT = 0x1,
    ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS = 0x2,
    ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS = 0x4,
    ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS = 0x8,
    ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS = 0x10,
    ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS = 0x20,
    ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT = 0x40,
    ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE = 0x80
} 	RootSignatureFlags;

DEFINE_ENUM_FLAG_OPERATORS(RootSignatureFlags);
typedef
enum StaticBorderColor
{
    STATIC_BORDER_COLOR_TRANSPARENT_BLACK = 0,
    STATIC_BORDER_COLOR_OPAQUE_BLACK = (STATIC_BORDER_COLOR_TRANSPARENT_BLACK + 1),
    STATIC_BORDER_COLOR_OPAQUE_WHITE = (STATIC_BORDER_COLOR_OPAQUE_BLACK + 1)
} 	StaticBorderColor;

typedef
enum ShaderVisibility
{
    SHADER_VISIBILITY_ALL = 0,
    SHADER_VISIBILITY_VERTEX = 1,
    SHADER_VISIBILITY_HULL = 2,
    SHADER_VISIBILITY_DOMAIN = 3,
    SHADER_VISIBILITY_GEOMETRY = 4,
    SHADER_VISIBILITY_PIXEL = 5
} 	ShaderVisibility;

typedef
struct ResourceDesc
{
    ResourceDimension Dimension;
    UINT64 Alignment;
    UINT64 Width;
    UINT Height;
    UINT16 DepthOrArraySize;
    UINT16 MipLevels;
    Format Format;
    SampleDesc SampleDesc;
    TextureLayout Layout;
    ResourceFlags Flags;
} 	ResourceDesc;

typedef struct StaticSampleDesc
{
    Filter Filter;
    TextureAddressMode AddressU;
    TextureAddressMode AddressV;
    TextureAddressMode AddressW;
    FLOAT MipLODBias;
    UINT MaxAnisotropy;
    ComparisonFunc ComparisonFunc;
    StaticBorderColor BorderColor;
    FLOAT MinLOD;
    FLOAT MaxLOD;
    UINT ShaderRegister;
    UINT RegisterSpace;
    ShaderVisibility ShaderVisibility;
    static StaticSampleDesc Init(DEFAULT)
    {
        StaticSampleDesc sampler = {};
        sampler.Filter = FILTER_MIN_MAG_MIP_POINT;
        sampler.AddressU = TEXTURE_ADDRESS_MODE_BORDER;
        sampler.AddressV = TEXTURE_ADDRESS_MODE_BORDER;
        sampler.AddressW = TEXTURE_ADDRESS_MODE_BORDER;
        sampler.MipLODBias = 0;
        sampler.MaxAnisotropy = 0;
        sampler.ComparisonFunc = COMPARISON_FUNC_NEVER;
        sampler.BorderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
        sampler.MinLOD = 0.0f;
        sampler.MaxLOD = 3.402823466e+38f;
        sampler.ShaderRegister = 0;
        sampler.RegisterSpace = 0;
        sampler.ShaderVisibility = SHADER_VISIBILITY_ALL;
        return sampler;
    }
} 	StaticSampleDesc;

typedef 
struct SubresourceData
{
    const void* pData;
    LONG_PTR RowPitch;
    LONG_PTR SlicePitch;
} 	SubresourceData;

typedef struct CachedPipelineState
{
    _Field_size_bytes_full_(CachedBlobSizeInBytes)  const void* pCachedBlob;
    SIZE_T CachedBlobSizeInBytes;
} 	CachedPipelineState;