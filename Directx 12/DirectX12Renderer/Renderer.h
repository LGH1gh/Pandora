#pragma once
#include "Win32Application.h"
#include "RendererHelper.h"

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
        m_address(address),
        m_size(size)
    {}


    const void* m_address;
    size_t m_size;
};

struct PipelineParams
{
    RootSignature m_rootSignature;
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