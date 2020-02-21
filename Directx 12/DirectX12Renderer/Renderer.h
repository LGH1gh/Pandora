#pragma once
#include "Win32Application.h"
#include "RendererHelper.h"

typedef
enum CullMode : UINT8
{
    CULL_NONE,
    CULL_FRONT,
    CULL_BACK
}   CullMode;

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
    PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED = 0,
    PRIMITIVE_TOPOLOGY_TYPE_POINT,
    PRIMITIVE_TOPOLOGY_TYPE_LINE,
    PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
    PRIMITIVE_TOPOLOGY_TYPE_PATCH
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

struct AttributeDesc
{
    UINT stream;
    AttributeFormat format;
    const char* name;
};

typedef
enum HeapType
{
	HEAP_TYPE_DEFAULT,
	HEAP_TYPE_UPLOAD,
	HEAP_TYPE_READBACK,
	HEAP_TYPE_CUSTOM
} 	HeapType;



struct Blob
{
    Blob() = default;
    Blob(const void* address, size_t size) :
        address(address),
        size(size)
    {}


    const void* address;
    size_t size;
};

struct PipelineParams
{
    ComPtr<RootSignature> rootSignature;
    Blob vs{ nullptr, 0 };
    Blob gs{ nullptr, 0 };
    Blob ps{ nullptr, 0 };

   
    PrimitiveTopologyType primitiveTopologyType = PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    const AttributeDesc* attributes = nullptr;
    UINT attributeCount = 0;

    BlendState blendState;
    CullMode cullMode = CULL_NONE;

    bool depthTest = false;
    bool depthWrite = false;
    ComparisonFunc depthFunc = COMPARISON_FUNC_GREATER;
};


class Renderer
{
public:
	Renderer(UINT width, UINT height, std::wstring name);
	virtual ~Renderer();

	float GetWidth() { return m_width; }
	float GetHeight() { return m_height; }
	const WCHAR* GetTitle() const { return m_title.c_str(); }

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;

	virtual void OnKeyDown(UINT8) {}
	virtual void OnKeyUp(UINT8) {}

protected:
	bool m_useWarpDevice;
	float m_width, m_height;
	std::wstring m_title;
};