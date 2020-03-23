#pragma once

#include <wrl/client.h>

#include <d3d12.h>

#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <d2d1_3.h>
#include <dwrite.h>
#include <d3d11on12.h>
#include <DDSTextureLoader.h>
#include <fstream>
#include <memory>

#pragma comment ( lib, "d2d1.lib" )
#pragma comment ( lib, "d3d12.lib")
#pragma comment ( lib, "dxgi.lib")
#pragma comment ( lib, "d3dcompiler.lib" )
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "d3d11.lib" )
#pragma comment ( lib, "dwrite.lib" )



using Microsoft::WRL::ComPtr;

struct XD3D12_DEFAULT {};
extern const DECLSPEC_SELECTANY XD3D12_DEFAULT D3D12_DEFAULT;

struct XD3D12_BLEND_DESC : public D3D12_BLEND_DESC
{
	XD3D12_BLEND_DESC() = default;
	explicit XD3D12_BLEND_DESC(const D3D12_BLEND_DESC& o) :
		D3D12_BLEND_DESC(o)
	{}
	explicit XD3D12_BLEND_DESC(XD3D12_DEFAULT)
	{
		AlphaToCoverageEnable = FALSE;
		IndependentBlendEnable = FALSE;
		const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			RenderTarget[i] = defaultRenderTargetBlendDesc;
	}
};

struct XD3D12_CPU_DESCRIPTOR_HANDLE : public D3D12_CPU_DESCRIPTOR_HANDLE
{
	XD3D12_CPU_DESCRIPTOR_HANDLE() = default;
	explicit XD3D12_CPU_DESCRIPTOR_HANDLE(const D3D12_CPU_DESCRIPTOR_HANDLE& o) :
		D3D12_CPU_DESCRIPTOR_HANDLE(o)
	{}
	XD3D12_CPU_DESCRIPTOR_HANDLE(XD3D12_DEFAULT) { ptr = 0; }
	XD3D12_CPU_DESCRIPTOR_HANDLE(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other, INT offsetScaledByIncrementSize)
	{
		InitOffsetted(other, offsetScaledByIncrementSize);
	}
	XD3D12_CPU_DESCRIPTOR_HANDLE(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other, INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		InitOffsetted(other, offsetInDescriptors, descriptorIncrementSize);
	}
	XD3D12_CPU_DESCRIPTOR_HANDLE& Offset(INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		ptr = SIZE_T(INT64(ptr) + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize));
		return *this;
	}
	XD3D12_CPU_DESCRIPTOR_HANDLE& Offset(INT offsetScaledByIncrementSize)
	{
		ptr = SIZE_T(INT64(ptr) + INT64(offsetScaledByIncrementSize));
		return *this;
	}
	bool operator==(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other) const
	{
		return (ptr == other.ptr);
	}
	bool operator!=(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other) const
	{
		return (ptr != other.ptr);
	}
	XD3D12_CPU_DESCRIPTOR_HANDLE& operator=(const D3D12_CPU_DESCRIPTOR_HANDLE& other)
	{
		ptr = other.ptr;
		return *this;
	}

	inline void InitOffsetted(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetScaledByIncrementSize)
	{
		InitOffsetted(*this, base, offsetScaledByIncrementSize);
	}

	inline void InitOffsetted(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		InitOffsetted(*this, base, offsetInDescriptors, descriptorIncrementSize);
	}

	static inline void InitOffsetted(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE& handle, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetScaledByIncrementSize)
	{
		handle.ptr = SIZE_T(INT64(base.ptr) + INT64(offsetScaledByIncrementSize));
	}

	static inline void InitOffsetted(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE& handle, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		handle.ptr = SIZE_T(INT64(base.ptr) + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize));
	}
};

struct XD3D12_DEPTH_STENCIL_DESC : public D3D12_DEPTH_STENCIL_DESC
{
	XD3D12_DEPTH_STENCIL_DESC() = default;
	explicit XD3D12_DEPTH_STENCIL_DESC(const D3D12_DEPTH_STENCIL_DESC& o) :
		D3D12_DEPTH_STENCIL_DESC(o)
	{}
	explicit XD3D12_DEPTH_STENCIL_DESC(XD3D12_DEFAULT)
	{
		DepthEnable = TRUE;
		DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		StencilEnable = FALSE;
		StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
		const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
		{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
		FrontFace = defaultStencilOp;
		BackFace = defaultStencilOp;
	}
	explicit XD3D12_DEPTH_STENCIL_DESC(
		BOOL depthEnable,
		D3D12_DEPTH_WRITE_MASK depthWriteMask,
		D3D12_COMPARISON_FUNC depthFunc,
		BOOL stencilEnable,
		UINT8 stencilReadMask,
		UINT8 stencilWriteMask,
		D3D12_STENCIL_OP frontStencilFailOp,
		D3D12_STENCIL_OP frontStencilDepthFailOp,
		D3D12_STENCIL_OP frontStencilPassOp,
		D3D12_COMPARISON_FUNC frontStencilFunc,
		D3D12_STENCIL_OP backStencilFailOp,
		D3D12_STENCIL_OP backStencilDepthFailOp,
		D3D12_STENCIL_OP backStencilPassOp,
		D3D12_COMPARISON_FUNC backStencilFunc)
	{
		DepthEnable = depthEnable;
		DepthWriteMask = depthWriteMask;
		DepthFunc = depthFunc;
		StencilEnable = stencilEnable;
		StencilReadMask = stencilReadMask;
		StencilWriteMask = stencilWriteMask;
		FrontFace.StencilFailOp = frontStencilFailOp;
		FrontFace.StencilDepthFailOp = frontStencilDepthFailOp;
		FrontFace.StencilPassOp = frontStencilPassOp;
		FrontFace.StencilFunc = frontStencilFunc;
		BackFace.StencilFailOp = backStencilFailOp;
		BackFace.StencilDepthFailOp = backStencilDepthFailOp;
		BackFace.StencilPassOp = backStencilPassOp;
		BackFace.StencilFunc = backStencilFunc;
	}
};

struct XD3D12_DESCRIPTOR_RANGE : D3D12_DESCRIPTOR_RANGE
{

	XD3D12_DESCRIPTOR_RANGE() = default;
	explicit XD3D12_DESCRIPTOR_RANGE(const D3D12_DESCRIPTOR_RANGE& o) :
		D3D12_DESCRIPTOR_RANGE(o)
	{}
	XD3D12_DESCRIPTOR_RANGE(
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseShaderRegister,
		UINT registerSpace = 0,
		UINT offsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
	)
	{
		Init(rangeType, numDescriptors, baseShaderRegister, registerSpace, offsetInDescriptorsFromTableStart);
	}

	inline void Init(D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseShaderRegister,
		UINT registerSpace = 0,
		UINT offsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
	)
	{
		Init(*this, rangeType, numDescriptors, baseShaderRegister, registerSpace, offsetInDescriptorsFromTableStart);

	}

	static inline void Init(
		_Out_ D3D12_DESCRIPTOR_RANGE& range,
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseShaderRegister,
		UINT registerSpace = 0,
		UINT offsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
	{
		range.RangeType = rangeType;
		range.NumDescriptors = numDescriptors;
		range.BaseShaderRegister = baseShaderRegister;
		range.RegisterSpace = registerSpace;
		range.OffsetInDescriptorsFromTableStart = offsetInDescriptorsFromTableStart;
	}
};

struct XD3D12_DESCRIPTOR_RANGE1 : public D3D12_DESCRIPTOR_RANGE1
{
	XD3D12_DESCRIPTOR_RANGE1() = default;
	explicit XD3D12_DESCRIPTOR_RANGE1(const D3D12_DESCRIPTOR_RANGE1& o) :
		D3D12_DESCRIPTOR_RANGE1(o)
	{}
	XD3D12_DESCRIPTOR_RANGE1(
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseShaderRegister,
		UINT registerSpace = 0,
		D3D12_DESCRIPTOR_RANGE_FLAGS flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
		UINT offsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
	{
		Init(rangeType, numDescriptors, baseShaderRegister, registerSpace, flags, offsetInDescriptorsFromTableStart);
	}

	inline void Init(
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseShaderRegister,
		UINT registerSpace = 0,
		D3D12_DESCRIPTOR_RANGE_FLAGS flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
		UINT offsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
	{
		Init(*this, rangeType, numDescriptors, baseShaderRegister, registerSpace, flags, offsetInDescriptorsFromTableStart);
	}

	static inline void Init(
		_Out_ D3D12_DESCRIPTOR_RANGE1& range,
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseShaderRegister,
		UINT registerSpace = 0,
		D3D12_DESCRIPTOR_RANGE_FLAGS flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
		UINT offsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
	{
		range.RangeType = rangeType;
		range.NumDescriptors = numDescriptors;
		range.BaseShaderRegister = baseShaderRegister;
		range.RegisterSpace = registerSpace;
		range.Flags = flags;
		range.OffsetInDescriptorsFromTableStart = offsetInDescriptorsFromTableStart;
	}
};

struct XD3D12_HEAP_PROPERTIES : public D3D12_HEAP_PROPERTIES
{
	XD3D12_HEAP_PROPERTIES() = default;
	explicit XD3D12_HEAP_PROPERTIES(const D3D12_HEAP_PROPERTIES& o) :
		D3D12_HEAP_PROPERTIES(o)
	{}
	XD3D12_HEAP_PROPERTIES(
		D3D12_CPU_PAGE_PROPERTY cpuPageProperty,
		D3D12_MEMORY_POOL memoryPoolPreference,
		UINT creationNodeMask = 1,
		UINT nodeMask = 1)
	{
		Type = D3D12_HEAP_TYPE_CUSTOM;
		CPUPageProperty = cpuPageProperty;
		MemoryPoolPreference = memoryPoolPreference;
		CreationNodeMask = creationNodeMask;
		VisibleNodeMask = nodeMask;
	}
	explicit XD3D12_HEAP_PROPERTIES(
		D3D12_HEAP_TYPE type,
		UINT creationNodeMask = 1,
		UINT nodeMask = 1)
	{
		Type = type;
		CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		CreationNodeMask = creationNodeMask;
		VisibleNodeMask = nodeMask;
	}
	bool IsCPUAccessible() const
	{
		return Type == D3D12_HEAP_TYPE_UPLOAD || Type == D3D12_HEAP_TYPE_READBACK || (Type == D3D12_HEAP_TYPE_CUSTOM &&
			(CPUPageProperty == D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE || CPUPageProperty == D3D12_CPU_PAGE_PROPERTY_WRITE_BACK));
	}
};
inline bool operator==(const D3D12_HEAP_PROPERTIES& l, const D3D12_HEAP_PROPERTIES& r)
{
	return l.Type == r.Type && l.CPUPageProperty == r.CPUPageProperty &&
		l.MemoryPoolPreference == r.MemoryPoolPreference &&
		l.CreationNodeMask == r.CreationNodeMask &&
		l.VisibleNodeMask == r.VisibleNodeMask;
}
inline bool operator!=(const D3D12_HEAP_PROPERTIES& l, const D3D12_HEAP_PROPERTIES& r)
{
	return !(l == r);
}

struct XD3D12_RANGE : public D3D12_RANGE
{
	XD3D12_RANGE() = default;
	explicit XD3D12_RANGE(const D3D12_RANGE& o) :
		D3D12_RANGE(o)
	{}
	XD3D12_RANGE(
		SIZE_T begin,
		SIZE_T end)
	{
		Begin = begin;
		End = end;
	}
};

struct XD3D12_RASTERIZER_DESC : public D3D12_RASTERIZER_DESC
{
	XD3D12_RASTERIZER_DESC() = default;
	explicit XD3D12_RASTERIZER_DESC(const D3D12_RASTERIZER_DESC& o) :
		D3D12_RASTERIZER_DESC(o)
	{}
	explicit XD3D12_RASTERIZER_DESC(XD3D12_DEFAULT)
	{
		FillMode = D3D12_FILL_MODE_SOLID;
		CullMode = D3D12_CULL_MODE_BACK;
		FrontCounterClockwise = FALSE;
		DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		DepthClipEnable = TRUE;
		MultisampleEnable = FALSE;
		AntialiasedLineEnable = FALSE;
		ForcedSampleCount = 0;
		ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}
	explicit XD3D12_RASTERIZER_DESC(
		D3D12_FILL_MODE fillMode,
		D3D12_CULL_MODE cullMode,
		BOOL frontCounterClockwise,
		INT depthBias,
		FLOAT depthBiasClamp,
		FLOAT slopeScaledDepthBias,
		BOOL depthClipEnable,
		BOOL multisampleEnable,
		BOOL antialiasedLineEnable,
		UINT forcedSampleCount,
		D3D12_CONSERVATIVE_RASTERIZATION_MODE conservativeRaster)
	{
		FillMode = fillMode;
		CullMode = cullMode;
		FrontCounterClockwise = frontCounterClockwise;
		DepthBias = depthBias;
		DepthBiasClamp = depthBiasClamp;
		SlopeScaledDepthBias = slopeScaledDepthBias;
		DepthClipEnable = depthClipEnable;
		MultisampleEnable = multisampleEnable;
		AntialiasedLineEnable = antialiasedLineEnable;
		ForcedSampleCount = forcedSampleCount;
		ConservativeRaster = conservativeRaster;
	}
};

struct XD3D12_RESOURCE_BARRIER : public D3D12_RESOURCE_BARRIER
{
	XD3D12_RESOURCE_BARRIER() = default;
	explicit XD3D12_RESOURCE_BARRIER(const D3D12_RESOURCE_BARRIER& o) :
		D3D12_RESOURCE_BARRIER(o)
	{}
	static inline XD3D12_RESOURCE_BARRIER Transition(
		_In_ ID3D12Resource* pResource,
		D3D12_RESOURCE_STATES stateBefore,
		D3D12_RESOURCE_STATES stateAfter,
		UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
		D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE)
	{
		XD3D12_RESOURCE_BARRIER result = {};
		D3D12_RESOURCE_BARRIER& barrier = result;
		result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		result.Flags = flags;
		barrier.Transition.pResource = pResource;
		barrier.Transition.StateBefore = stateBefore;
		barrier.Transition.StateAfter = stateAfter;
		barrier.Transition.Subresource = subresource;
		return result;
	}
	static inline XD3D12_RESOURCE_BARRIER Aliasing(
		_In_ ID3D12Resource* pResourceBefore,
		_In_ ID3D12Resource* pResourceAfter)
	{
		XD3D12_RESOURCE_BARRIER result = {};
		D3D12_RESOURCE_BARRIER& barrier = result;
		result.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
		barrier.Aliasing.pResourceBefore = pResourceBefore;
		barrier.Aliasing.pResourceAfter = pResourceAfter;
		return result;
	}
	static inline XD3D12_RESOURCE_BARRIER UAV(
		_In_ ID3D12Resource* pResource)
	{
		XD3D12_RESOURCE_BARRIER result = {};
		D3D12_RESOURCE_BARRIER& barrier = result;
		result.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		barrier.UAV.pResource = pResource;
		return result;
	}
};

inline UINT8 D3D12GetFormatPlaneCount(
	_In_ ID3D12Device* pDevice,
	DXGI_FORMAT Format
)
{
	D3D12_FEATURE_DATA_FORMAT_INFO formatInfo = { Format, 0 };
	if (FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_FORMAT_INFO, &formatInfo, sizeof(formatInfo))))
	{
		return 0;
	}
	return formatInfo.PlaneCount;
}
inline UINT D3D12CalcSubresource(UINT MipSlice, UINT ArraySlice, UINT PlaneSlice, UINT MipLevels, UINT ArraySize)
{
	return MipSlice + ArraySlice * MipLevels + PlaneSlice * MipLevels * ArraySize;
}
struct XD3D12_RESOURCE_DESC : public D3D12_RESOURCE_DESC
{
	XD3D12_RESOURCE_DESC() = default;
	explicit XD3D12_RESOURCE_DESC(const D3D12_RESOURCE_DESC& o) :
		D3D12_RESOURCE_DESC(o)
	{}
	XD3D12_RESOURCE_DESC(
		D3D12_RESOURCE_DIMENSION dimension,
		UINT64 alignment,
		UINT64 width,
		UINT height,
		UINT16 depthOrArraySize,
		UINT16 mipLevels,
		DXGI_FORMAT format,
		UINT sampleCount,
		UINT sampleQuality,
		D3D12_TEXTURE_LAYOUT layout,
		D3D12_RESOURCE_FLAGS flags)
	{
		Dimension = dimension;
		Alignment = alignment;
		Width = width;
		Height = height;
		DepthOrArraySize = depthOrArraySize;
		MipLevels = mipLevels;
		Format = format;
		SampleDesc.Count = sampleCount;
		SampleDesc.Quality = sampleQuality;
		Layout = layout;
		Flags = flags;
	}
	static inline XD3D12_RESOURCE_DESC Buffer(
		const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE)
	{
		return XD3D12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER, resAllocInfo.Alignment, resAllocInfo.SizeInBytes,
			1, 1, 1, DXGI_FORMAT_UNKNOWN, 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags);
	}
	static inline XD3D12_RESOURCE_DESC Buffer(
		UINT64 width,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
		UINT64 alignment = 0)
	{
		return XD3D12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER, alignment, width, 1, 1, 1,
			DXGI_FORMAT_UNKNOWN, 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags);
	}
	static inline XD3D12_RESOURCE_DESC Tex1D(
		DXGI_FORMAT format,
		UINT64 width,
		UINT16 arraySize = 1,
		UINT16 mipLevels = 0,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
		D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
		UINT64 alignment = 0)
	{
		return XD3D12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE1D, alignment, width, 1, arraySize,
			mipLevels, format, 1, 0, layout, flags);
	}
	static inline XD3D12_RESOURCE_DESC Tex2D(
		DXGI_FORMAT format,
		UINT64 width,
		UINT height,
		UINT16 arraySize = 1,
		UINT16 mipLevels = 0,
		UINT sampleCount = 1,
		UINT sampleQuality = 0,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
		D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
		UINT64 alignment = 0)
	{
		return XD3D12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE2D, alignment, width, height, arraySize,
			mipLevels, format, sampleCount, sampleQuality, layout, flags);
	}
	static inline XD3D12_RESOURCE_DESC Tex3D(
		DXGI_FORMAT format,
		UINT64 width,
		UINT height,
		UINT16 depth,
		UINT16 mipLevels = 0,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
		D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
		UINT64 alignment = 0)
	{
		return XD3D12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE3D, alignment, width, height, depth,
			mipLevels, format, 1, 0, layout, flags);
	}
	inline UINT16 Depth() const
	{
		return (Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D ? DepthOrArraySize : 1);
	}
	inline UINT16 ArraySize() const
	{
		return (Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE3D ? DepthOrArraySize : 1);
	}
	inline UINT8 PlaneCount(_In_ ID3D12Device* pDevice) const
	{
		return D3D12GetFormatPlaneCount(pDevice, Format);
	}
	inline UINT Subresources(_In_ ID3D12Device* pDevice) const
	{
		return MipLevels * ArraySize() * PlaneCount(pDevice);
	}
	inline UINT CalcSubresource(UINT MipSlice, UINT ArraySlice, UINT PlaneSlice)
	{
		return D3D12CalcSubresource(MipSlice, ArraySlice, PlaneSlice, MipLevels, ArraySize());
	}
};
inline bool operator==(const D3D12_RESOURCE_DESC& l, const D3D12_RESOURCE_DESC& r)
{
	return l.Dimension == r.Dimension &&
		l.Alignment == r.Alignment &&
		l.Width == r.Width &&
		l.Height == r.Height &&
		l.DepthOrArraySize == r.DepthOrArraySize &&
		l.MipLevels == r.MipLevels &&
		l.Format == r.Format &&
		l.SampleDesc.Count == r.SampleDesc.Count &&
		l.SampleDesc.Quality == r.SampleDesc.Quality &&
		l.Layout == r.Layout &&
		l.Flags == r.Flags;
}
inline bool operator!=(const D3D12_RESOURCE_DESC& l, const D3D12_RESOURCE_DESC& r)
{
	return !(l == r);
}

struct XD3D12_ROOT_CONSTANTS : public D3D12_ROOT_CONSTANTS
{
	XD3D12_ROOT_CONSTANTS() = default;
	explicit XD3D12_ROOT_CONSTANTS(const D3D12_ROOT_CONSTANTS& o) :
		D3D12_ROOT_CONSTANTS(o)
	{}
	XD3D12_ROOT_CONSTANTS(
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0)
	{
		Init(num32BitValues, shaderRegister, registerSpace);
	}

	inline void Init(
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0)
	{
		Init(*this, num32BitValues, shaderRegister, registerSpace);
	}

	static inline void Init(
		_Out_ D3D12_ROOT_CONSTANTS& rootConstants,
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0)
	{
		rootConstants.Num32BitValues = num32BitValues;
		rootConstants.ShaderRegister = shaderRegister;
		rootConstants.RegisterSpace = registerSpace;
	}
};

struct XD3D12_ROOT_DESCRIPTOR : public D3D12_ROOT_DESCRIPTOR
{
	XD3D12_ROOT_DESCRIPTOR() = default;
	explicit XD3D12_ROOT_DESCRIPTOR(const D3D12_ROOT_DESCRIPTOR& o) :
		D3D12_ROOT_DESCRIPTOR(o)
	{}
	XD3D12_ROOT_DESCRIPTOR(
		UINT shaderRegister,
		UINT registerSpace = 0)
	{
		Init(shaderRegister, registerSpace);
	}

	inline void Init(
		UINT shaderRegister,
		UINT registerSpace = 0)
	{
		Init(*this, shaderRegister, registerSpace);
	}

	static inline void Init(_Out_ D3D12_ROOT_DESCRIPTOR& table, UINT shaderRegister, UINT registerSpace = 0)
	{
		table.ShaderRegister = shaderRegister;
		table.RegisterSpace = registerSpace;
	}
};

struct XD3D12_ROOT_DESCRIPTOR1 : public D3D12_ROOT_DESCRIPTOR1
{
	XD3D12_ROOT_DESCRIPTOR1() = default;
	explicit XD3D12_ROOT_DESCRIPTOR1(const D3D12_ROOT_DESCRIPTOR1& o) :
		D3D12_ROOT_DESCRIPTOR1(o)
	{}
	XD3D12_ROOT_DESCRIPTOR1(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE)
	{
		Init(shaderRegister, registerSpace, flags);
	}

	inline void Init(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE)
	{
		Init(*this, shaderRegister, registerSpace, flags);
	}

	static inline void Init(
		_Out_ D3D12_ROOT_DESCRIPTOR1& table,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE)
	{
		table.ShaderRegister = shaderRegister;
		table.RegisterSpace = registerSpace;
		table.Flags = flags;
	}
};

struct XD3D12_ROOT_DESCRIPTOR_TABLE : public D3D12_ROOT_DESCRIPTOR_TABLE
{
	XD3D12_ROOT_DESCRIPTOR_TABLE() = default;
	explicit XD3D12_ROOT_DESCRIPTOR_TABLE(const D3D12_ROOT_DESCRIPTOR_TABLE& o) :
		D3D12_ROOT_DESCRIPTOR_TABLE(o)
	{}
	XD3D12_ROOT_DESCRIPTOR_TABLE(
		UINT numDescriptorRanges,
		_In_reads_opt_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* _pDescriptorRanges)
	{
		Init(numDescriptorRanges, _pDescriptorRanges);
	}

	inline void Init(
		UINT numDescriptorRanges,
		_In_reads_opt_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* _pDescriptorRanges)
	{
		Init(*this, numDescriptorRanges, _pDescriptorRanges);
	}

	static inline void Init(
		_Out_ D3D12_ROOT_DESCRIPTOR_TABLE& rootDescriptorTable,
		UINT numDescriptorRanges,
		_In_reads_opt_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* _pDescriptorRanges)
	{
		rootDescriptorTable.NumDescriptorRanges = numDescriptorRanges;
		rootDescriptorTable.pDescriptorRanges = _pDescriptorRanges;
	}
};

struct XD3D12_ROOT_DESCRIPTOR_TABLE1 : public D3D12_ROOT_DESCRIPTOR_TABLE1
{
	XD3D12_ROOT_DESCRIPTOR_TABLE1() = default;
	explicit XD3D12_ROOT_DESCRIPTOR_TABLE1(const D3D12_ROOT_DESCRIPTOR_TABLE1& o) :
		D3D12_ROOT_DESCRIPTOR_TABLE1(o)
	{}
	XD3D12_ROOT_DESCRIPTOR_TABLE1(
		UINT numDescriptorRanges,
		_In_reads_opt_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE1* _pDescriptorRanges)
	{
		Init(numDescriptorRanges, _pDescriptorRanges);
	}

	inline void Init(
		UINT numDescriptorRanges,
		_In_reads_opt_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE1* _pDescriptorRanges)
	{
		Init(*this, numDescriptorRanges, _pDescriptorRanges);
	}

	static inline void Init(
		_Out_ D3D12_ROOT_DESCRIPTOR_TABLE1& rootDescriptorTable,
		UINT numDescriptorRanges,
		_In_reads_opt_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE1* _pDescriptorRanges)
	{
		rootDescriptorTable.NumDescriptorRanges = numDescriptorRanges;
		rootDescriptorTable.pDescriptorRanges = _pDescriptorRanges;
	}
};

struct XD3D12_ROOT_PARAMETER : public D3D12_ROOT_PARAMETER
{
	XD3D12_ROOT_PARAMETER() = default;
	explicit XD3D12_ROOT_PARAMETER(const D3D12_ROOT_PARAMETER& o) :
		D3D12_ROOT_PARAMETER(o)
	{}

	static inline void InitAsDescriptorTable(
		_Out_ D3D12_ROOT_PARAMETER& rootParam,
		UINT numDescriptorRanges,
		_In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* pDescriptorRanges,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam.ShaderVisibility = visibility;
		XD3D12_ROOT_DESCRIPTOR_TABLE::Init(rootParam.DescriptorTable, numDescriptorRanges, pDescriptorRanges);
	}

	static inline void InitAsConstants(
		_Out_ D3D12_ROOT_PARAMETER& rootParam,
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParam.ShaderVisibility = visibility;
		XD3D12_ROOT_CONSTANTS::Init(rootParam.Constants, num32BitValues, shaderRegister, registerSpace);
	}

	static inline void InitAsConstantBufferView(
		_Out_ D3D12_ROOT_PARAMETER& rootParam,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParam.ShaderVisibility = visibility;
		XD3D12_ROOT_DESCRIPTOR::Init(rootParam.Descriptor, shaderRegister, registerSpace);
	}

	static inline void InitAsShaderResourceView(
		_Out_ D3D12_ROOT_PARAMETER& rootParam,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		rootParam.ShaderVisibility = visibility;
		XD3D12_ROOT_DESCRIPTOR::Init(rootParam.Descriptor, shaderRegister, registerSpace);
	}

	static inline void InitAsUnorderedAccessView(
		_Out_ D3D12_ROOT_PARAMETER& rootParam,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
		rootParam.ShaderVisibility = visibility;
		XD3D12_ROOT_DESCRIPTOR::Init(rootParam.Descriptor, shaderRegister, registerSpace);
	}

	inline void InitAsDescriptorTable(
		UINT numDescriptorRanges,
		_In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* pDescriptorRanges,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsDescriptorTable(*this, numDescriptorRanges, pDescriptorRanges, visibility);
	}

	inline void InitAsConstants(
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsConstants(*this, num32BitValues, shaderRegister, registerSpace, visibility);
	}

	inline void InitAsConstantBufferView(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsConstantBufferView(*this, shaderRegister, registerSpace, visibility);
	}

	inline void InitAsShaderResourceView(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsShaderResourceView(*this, shaderRegister, registerSpace, visibility);
	}

	inline void InitAsUnorderedAccessView(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsUnorderedAccessView(*this, shaderRegister, registerSpace, visibility);
	}
};

struct XD3D12_ROOT_PARAMETER1 : public D3D12_ROOT_PARAMETER1
{
	XD3D12_ROOT_PARAMETER1() = default;
	explicit XD3D12_ROOT_PARAMETER1(const D3D12_ROOT_PARAMETER1& o) :
		D3D12_ROOT_PARAMETER1(o)
	{}

	static inline void InitAsDescriptorTable(
		_Out_ D3D12_ROOT_PARAMETER1& rootParam,
		UINT numDescriptorRanges,
		_In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE1* pDescriptorRanges,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam.ShaderVisibility = visibility;
		XD3D12_ROOT_DESCRIPTOR_TABLE1::Init(rootParam.DescriptorTable, numDescriptorRanges, pDescriptorRanges);
	}

	static inline void InitAsConstants(
		_Out_ D3D12_ROOT_PARAMETER1& rootParam,
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParam.ShaderVisibility = visibility;
		XD3D12_ROOT_CONSTANTS::Init(rootParam.Constants, num32BitValues, shaderRegister, registerSpace);
	}

	static inline void InitAsConstantBufferView(
		_Out_ D3D12_ROOT_PARAMETER1& rootParam,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParam.ShaderVisibility = visibility;
		XD3D12_ROOT_DESCRIPTOR1::Init(rootParam.Descriptor, shaderRegister, registerSpace, flags);
	}

	static inline void InitAsShaderResourceView(
		_Out_ D3D12_ROOT_PARAMETER1& rootParam,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		rootParam.ShaderVisibility = visibility;
		XD3D12_ROOT_DESCRIPTOR1::Init(rootParam.Descriptor, shaderRegister, registerSpace, flags);
	}

	static inline void InitAsUnorderedAccessView(
		_Out_ D3D12_ROOT_PARAMETER1& rootParam,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
		rootParam.ShaderVisibility = visibility;
		XD3D12_ROOT_DESCRIPTOR1::Init(rootParam.Descriptor, shaderRegister, registerSpace, flags);
	}

	inline void InitAsDescriptorTable(
		UINT numDescriptorRanges,
		_In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE1* pDescriptorRanges,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsDescriptorTable(*this, numDescriptorRanges, pDescriptorRanges, visibility);
	}

	inline void InitAsConstants(
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsConstants(*this, num32BitValues, shaderRegister, registerSpace, visibility);
	}

	inline void InitAsConstantBufferView(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsConstantBufferView(*this, shaderRegister, registerSpace, flags, visibility);
	}

	inline void InitAsShaderResourceView(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsShaderResourceView(*this, shaderRegister, registerSpace, flags, visibility);
	}

	inline void InitAsUnorderedAccessView(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_ROOT_DESCRIPTOR_FLAGS flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsUnorderedAccessView(*this, shaderRegister, registerSpace, flags, visibility);
	}
};

struct XD3D12_ROOT_SIGNATURE_DESC : public D3D12_ROOT_SIGNATURE_DESC
{
	XD3D12_ROOT_SIGNATURE_DESC() = default;
	explicit XD3D12_ROOT_SIGNATURE_DESC(const D3D12_ROOT_SIGNATURE_DESC& o) :
		D3D12_ROOT_SIGNATURE_DESC(o)
	{}
	XD3D12_ROOT_SIGNATURE_DESC(
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplers = nullptr,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		Init(numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
	}
	XD3D12_ROOT_SIGNATURE_DESC(XD3D12_DEFAULT)
	{
		Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_NONE);
	}

	inline void Init(
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplers = nullptr,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		Init(*this, numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
	}

	static inline void Init(
		_Out_ D3D12_ROOT_SIGNATURE_DESC& desc,
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplers = nullptr,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		desc.NumParameters = numParameters;
		desc.pParameters = _pParameters;
		desc.NumStaticSamplers = numStaticSamplers;
		desc.pStaticSamplers = _pStaticSamplers;
		desc.Flags = flags;
	}
};

struct XD3D12_STATIC_SAMPLER_DESC : D3D12_STATIC_SAMPLER_DESC
{
	XD3D12_STATIC_SAMPLER_DESC() = default;
	explicit XD3D12_STATIC_SAMPLER_DESC(const D3D12_STATIC_SAMPLER_DESC & o) :
		D3D12_STATIC_SAMPLER_DESC(o)
	{}
	XD3D12_STATIC_SAMPLER_DESC(
		UINT shaderRegister,
		D3D12_FILTER filter = D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		FLOAT mipLODBias = 0,
		UINT maxAnisotropy = 16,
		D3D12_COMPARISON_FUNC comparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL,
		D3D12_STATIC_BORDER_COLOR borderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
		FLOAT minLOD = 0.f,
		FLOAT maxLOD = D3D12_FLOAT32_MAX,
		D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL,
		UINT registerSpace = 0)
	{
		Init(
			shaderRegister,
			filter,
			addressU,
			addressV,
			addressW,
			mipLODBias,
			maxAnisotropy,
			comparisonFunc,
			borderColor,
			minLOD,
			maxLOD,
			shaderVisibility,
			registerSpace);
	}

	static inline void Init(
		_Out_ D3D12_STATIC_SAMPLER_DESC & samplerDesc,
		UINT shaderRegister,
		D3D12_FILTER filter = D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		FLOAT mipLODBias = 0,
		UINT maxAnisotropy = 16,
		D3D12_COMPARISON_FUNC comparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL,
		D3D12_STATIC_BORDER_COLOR borderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
		FLOAT minLOD = 0.f,
		FLOAT maxLOD = D3D12_FLOAT32_MAX,
		D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL,
		UINT registerSpace = 0)
	{
		samplerDesc.ShaderRegister = shaderRegister;
		samplerDesc.Filter = filter;
		samplerDesc.AddressU = addressU;
		samplerDesc.AddressV = addressV;
		samplerDesc.AddressW = addressW;
		samplerDesc.MipLODBias = mipLODBias;
		samplerDesc.MaxAnisotropy = maxAnisotropy;
		samplerDesc.ComparisonFunc = comparisonFunc;
		samplerDesc.BorderColor = borderColor;
		samplerDesc.MinLOD = minLOD;
		samplerDesc.MaxLOD = maxLOD;
		samplerDesc.ShaderVisibility = shaderVisibility;
		samplerDesc.RegisterSpace = registerSpace;
	}
	inline void Init(
		UINT shaderRegister,
		D3D12_FILTER filter = D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		FLOAT mipLODBias = 0,
		UINT maxAnisotropy = 16,
		D3D12_COMPARISON_FUNC comparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL,
		D3D12_STATIC_BORDER_COLOR borderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
		FLOAT minLOD = 0.f,
		FLOAT maxLOD = D3D12_FLOAT32_MAX,
		D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL,
		UINT registerSpace = 0)
	{
		Init(
			*this,
			shaderRegister,
			filter,
			addressU,
			addressV,
			addressW,
			mipLODBias,
			maxAnisotropy,
			comparisonFunc,
			borderColor,
			minLOD,
			maxLOD,
			shaderVisibility,
			registerSpace);
	}
};

struct XD3D12_SHADER_BYTECODE : public D3D12_SHADER_BYTECODE
{
	XD3D12_SHADER_BYTECODE() = default;
	explicit XD3D12_SHADER_BYTECODE(const D3D12_SHADER_BYTECODE& o) :
		D3D12_SHADER_BYTECODE(o)
	{}
	XD3D12_SHADER_BYTECODE(
		_In_ ID3DBlob* pShaderBlob)
	{
		pShaderBytecode = pShaderBlob->GetBufferPointer();
		BytecodeLength = pShaderBlob->GetBufferSize();
	}
	XD3D12_SHADER_BYTECODE(
		const void* _pShaderBytecode,
		SIZE_T bytecodeLength)
	{
		pShaderBytecode = _pShaderBytecode;
		BytecodeLength = bytecodeLength;
	}
};

struct XD3D12_TEXTURE_COPY_LOCATION : public D3D12_TEXTURE_COPY_LOCATION
{
	XD3D12_TEXTURE_COPY_LOCATION() = default;
	explicit XD3D12_TEXTURE_COPY_LOCATION(const D3D12_TEXTURE_COPY_LOCATION& o) :
		D3D12_TEXTURE_COPY_LOCATION(o)
	{}
	XD3D12_TEXTURE_COPY_LOCATION(_In_ ID3D12Resource* pRes)
	{
		pResource = pRes;
		Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		PlacedFootprint = {};
	}
	XD3D12_TEXTURE_COPY_LOCATION(_In_ ID3D12Resource* pRes, D3D12_PLACED_SUBRESOURCE_FOOTPRINT const& Footprint)
	{
		pResource = pRes;
		Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		PlacedFootprint = Footprint;
	}
	XD3D12_TEXTURE_COPY_LOCATION(_In_ ID3D12Resource* pRes, UINT Sub)
	{
		pResource = pRes;
		Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		PlacedFootprint = {};
		SubresourceIndex = Sub;
	}
};

struct XD3D12_VERSIONED_ROOT_SIGNATURE_DESC : public D3D12_VERSIONED_ROOT_SIGNATURE_DESC
{
	XD3D12_VERSIONED_ROOT_SIGNATURE_DESC() = default;
	explicit XD3D12_VERSIONED_ROOT_SIGNATURE_DESC(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& o) :
		D3D12_VERSIONED_ROOT_SIGNATURE_DESC(o)
	{}
	explicit XD3D12_VERSIONED_ROOT_SIGNATURE_DESC(const D3D12_ROOT_SIGNATURE_DESC& o)
	{
		Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
		Desc_1_0 = o;
	}
	explicit XD3D12_VERSIONED_ROOT_SIGNATURE_DESC(const D3D12_ROOT_SIGNATURE_DESC1& o)
	{
		Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
		Desc_1_1 = o;
	}
	XD3D12_VERSIONED_ROOT_SIGNATURE_DESC(
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplers = nullptr,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		Init_1_0(numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
	}
	XD3D12_VERSIONED_ROOT_SIGNATURE_DESC(
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER1* _pParameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplers = nullptr,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		Init_1_1(numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
	}
	XD3D12_VERSIONED_ROOT_SIGNATURE_DESC(XD3D12_DEFAULT)
	{
		Init_1_1(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_NONE);
	}

	inline void Init_1_0(
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplers = nullptr,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		Init_1_0(*this, numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
	}

	static inline void Init_1_0(
		_Out_ D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc,
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplers = nullptr,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		desc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
		desc.Desc_1_0.NumParameters = numParameters;
		desc.Desc_1_0.pParameters = _pParameters;
		desc.Desc_1_0.NumStaticSamplers = numStaticSamplers;
		desc.Desc_1_0.pStaticSamplers = _pStaticSamplers;
		desc.Desc_1_0.Flags = flags;
	}

	inline void Init_1_1(
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER1* _pParameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplers = nullptr,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		Init_1_1(*this, numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
	}

	static inline void Init_1_1(
		_Out_ D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc,
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER1* _pParameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplers = nullptr,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		desc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
		desc.Desc_1_1.NumParameters = numParameters;
		desc.Desc_1_1.pParameters = _pParameters;
		desc.Desc_1_1.NumStaticSamplers = numStaticSamplers;
		desc.Desc_1_1.pStaticSamplers = _pStaticSamplers;
		desc.Desc_1_1.Flags = flags;
	}
};


inline UINT64 GetRequiredIntermediateSize(
	_In_ ID3D12Resource* pDestinationResource,
	_In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
	_In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources)
{
	auto Desc = pDestinationResource->GetDesc();
	UINT64 RequiredSize = 0;

	ID3D12Device* pDevice = nullptr;
	pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
	pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, 0, nullptr, nullptr, nullptr, &RequiredSize);
	pDevice->Release();

	return RequiredSize;
}

inline void MemcpySubresource(
	_In_ const D3D12_MEMCPY_DEST* pDest,
	_In_ const D3D12_SUBRESOURCE_DATA* pSrc,
	SIZE_T RowSizeInBytes,
	UINT NumRows,
	UINT NumSlices)
{
	for (UINT z = 0; z < NumSlices; ++z)
	{
		auto pDestSlice = reinterpret_cast<BYTE*>(pDest->pData) + pDest->SlicePitch * z;
		auto pSrcSlice = reinterpret_cast<const BYTE*>(pSrc->pData) + pSrc->SlicePitch * LONG_PTR(z);
		for (UINT y = 0; y < NumRows; ++y)
		{
			memcpy(pDestSlice + pDest->RowPitch * y,
				pSrcSlice + pSrc->RowPitch * LONG_PTR(y),
				RowSizeInBytes);
		}
	}
}

//------------------------------------------------------------------------------------------------
// All arrays must be populated (e.g. by calling GetCopyableFootprints)
inline UINT64 UpdateSubresources(
	_In_ ID3D12GraphicsCommandList* pCmdList,
	_In_ ID3D12Resource* pDestinationResource,
	_In_ ID3D12Resource* pIntermediate,
	_In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
	_In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources,
	UINT64 RequiredSize,
	_In_reads_(NumSubresources) const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts,
	_In_reads_(NumSubresources) const UINT* pNumRows,
	_In_reads_(NumSubresources) const UINT64* pRowSizesInBytes,
	_In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData)
{
	// Minor validation
	auto IntermediateDesc = pIntermediate->GetDesc();
	auto DestinationDesc = pDestinationResource->GetDesc();
	if (IntermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER ||
		IntermediateDesc.Width < RequiredSize + pLayouts[0].Offset ||
		RequiredSize > SIZE_T(-1) ||
		(DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER &&
		(FirstSubresource != 0 || NumSubresources != 1)))
	{
		return 0;
	}

	BYTE* pData;
	HRESULT hr = pIntermediate->Map(0, nullptr, reinterpret_cast<void**>(&pData));
	if (FAILED(hr))
	{
		return 0;
	}

	for (UINT i = 0; i < NumSubresources; ++i)
	{
		if (pRowSizesInBytes[i] > SIZE_T(-1)) return 0;
		D3D12_MEMCPY_DEST DestData = { pData + pLayouts[i].Offset, pLayouts[i].Footprint.RowPitch, SIZE_T(pLayouts[i].Footprint.RowPitch) * SIZE_T(pNumRows[i]) };
		MemcpySubresource(&DestData, &pSrcData[i], static_cast<SIZE_T>(pRowSizesInBytes[i]), pNumRows[i], pLayouts[i].Footprint.Depth);
	}
	pIntermediate->Unmap(0, nullptr);

	if (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
	{
		pCmdList->CopyBufferRegion(
			pDestinationResource, 0, pIntermediate, pLayouts[0].Offset, pLayouts[0].Footprint.Width);
	}
	else
	{
		for (UINT i = 0; i < NumSubresources; ++i)
		{
			XD3D12_TEXTURE_COPY_LOCATION Dst(pDestinationResource, i + FirstSubresource);
			XD3D12_TEXTURE_COPY_LOCATION Src(pIntermediate, pLayouts[i]);
			pCmdList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
		}
	}
	return RequiredSize;
}

//------------------------------------------------------------------------------------------------
// Heap-allocating UpdateSubresources implementation
inline UINT64 UpdateSubresources(
	_In_ ID3D12GraphicsCommandList* pCmdList,
	_In_ ID3D12Resource* pDestinationResource,
	_In_ ID3D12Resource* pIntermediate,
	UINT64 IntermediateOffset,
	_In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
	_In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources,
	_In_reads_(NumSubresources) D3D12_SUBRESOURCE_DATA* pSrcData)
{
	UINT64 RequiredSize = 0;
	UINT64 MemToAlloc = static_cast<UINT64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64))* NumSubresources;
	if (MemToAlloc > SIZE_MAX)
	{
		return 0;
	}
	void* pMem = HeapAlloc(GetProcessHeap(), 0, static_cast<SIZE_T>(MemToAlloc));
	if (pMem == nullptr)
	{
		return 0;
	}
	auto pLayouts = reinterpret_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(pMem);
	UINT64* pRowSizesInBytes = reinterpret_cast<UINT64*>(pLayouts + NumSubresources);
	UINT* pNumRows = reinterpret_cast<UINT*>(pRowSizesInBytes + NumSubresources);

	auto Desc = pDestinationResource->GetDesc();
	ID3D12Device* pDevice = nullptr;
	pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
	pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, pLayouts, pNumRows, pRowSizesInBytes, &RequiredSize);
	pDevice->Release();

	UINT64 Result = UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, pLayouts, pNumRows, pRowSizesInBytes, pSrcData);
	HeapFree(GetProcessHeap(), 0, pMem);
	return Result;
}

//------------------------------------------------------------------------------------------------
// Stack-allocating UpdateSubresources implementation
template <UINT MaxSubresources>
inline UINT64 UpdateSubresources(
	_In_ ID3D12GraphicsCommandList* pCmdList,
	_In_ ID3D12Resource* pDestinationResource,
	_In_ ID3D12Resource* pIntermediate,
	UINT64 IntermediateOffset,
	_In_range_(0, MaxSubresources) UINT FirstSubresource,
	_In_range_(1, MaxSubresources - FirstSubresource) UINT NumSubresources,
	_In_reads_(NumSubresources) D3D12_SUBRESOURCE_DATA* pSrcData)
{
	UINT64 RequiredSize = 0;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT Layouts[MaxSubresources];
	UINT NumRows[MaxSubresources];
	UINT64 RowSizesInBytes[MaxSubresources];

	auto Desc = pDestinationResource->GetDesc();
	ID3D12Device* pDevice = nullptr;
	pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
	pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, Layouts, NumRows, RowSizesInBytes, &RequiredSize);
	pDevice->Release();

	return UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, Layouts, NumRows, RowSizesInBytes, pSrcData);
}

inline HRESULT D3DX12SerializeVersionedRootSignature(
	_In_ const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pRootSignatureDesc,
	D3D_ROOT_SIGNATURE_VERSION MaxVersion,
	_Outptr_ ID3DBlob** ppBlob,
	_Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob)
{
	if (ppErrorBlob != nullptr)
	{
		*ppErrorBlob = nullptr;
	}

	switch (MaxVersion)
	{
	case D3D_ROOT_SIGNATURE_VERSION_1_0:
		switch (pRootSignatureDesc->Version)
		{
		case D3D_ROOT_SIGNATURE_VERSION_1_0:
			return D3D12SerializeRootSignature(&pRootSignatureDesc->Desc_1_0, D3D_ROOT_SIGNATURE_VERSION_1, ppBlob, ppErrorBlob);

		case D3D_ROOT_SIGNATURE_VERSION_1_1:
		{
			HRESULT hr = S_OK;
			const D3D12_ROOT_SIGNATURE_DESC1& desc_1_1 = pRootSignatureDesc->Desc_1_1;

			const SIZE_T ParametersSize = sizeof(D3D12_ROOT_PARAMETER) * desc_1_1.NumParameters;
			void* pParameters = (ParametersSize > 0) ? HeapAlloc(GetProcessHeap(), 0, ParametersSize) : nullptr;
			if (ParametersSize > 0 && pParameters == nullptr)
			{
				hr = E_OUTOFMEMORY;
			}
			auto pParameters_1_0 = reinterpret_cast<D3D12_ROOT_PARAMETER*>(pParameters);

			if (SUCCEEDED(hr))
			{
				for (UINT n = 0; n < desc_1_1.NumParameters; n++)
				{
					__analysis_assume(ParametersSize == sizeof(D3D12_ROOT_PARAMETER) * desc_1_1.NumParameters);
					pParameters_1_0[n].ParameterType = desc_1_1.pParameters[n].ParameterType;
					pParameters_1_0[n].ShaderVisibility = desc_1_1.pParameters[n].ShaderVisibility;

					switch (desc_1_1.pParameters[n].ParameterType)
					{
					case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
						pParameters_1_0[n].Constants.Num32BitValues = desc_1_1.pParameters[n].Constants.Num32BitValues;
						pParameters_1_0[n].Constants.RegisterSpace = desc_1_1.pParameters[n].Constants.RegisterSpace;
						pParameters_1_0[n].Constants.ShaderRegister = desc_1_1.pParameters[n].Constants.ShaderRegister;
						break;

					case D3D12_ROOT_PARAMETER_TYPE_CBV:
					case D3D12_ROOT_PARAMETER_TYPE_SRV:
					case D3D12_ROOT_PARAMETER_TYPE_UAV:
						pParameters_1_0[n].Descriptor.RegisterSpace = desc_1_1.pParameters[n].Descriptor.RegisterSpace;
						pParameters_1_0[n].Descriptor.ShaderRegister = desc_1_1.pParameters[n].Descriptor.ShaderRegister;
						break;

					case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
						const D3D12_ROOT_DESCRIPTOR_TABLE1& table_1_1 = desc_1_1.pParameters[n].DescriptorTable;

						const SIZE_T DescriptorRangesSize = sizeof(D3D12_DESCRIPTOR_RANGE) * table_1_1.NumDescriptorRanges;
						void* pDescriptorRanges = (DescriptorRangesSize > 0 && SUCCEEDED(hr)) ? HeapAlloc(GetProcessHeap(), 0, DescriptorRangesSize) : nullptr;
						if (DescriptorRangesSize > 0 && pDescriptorRanges == nullptr)
						{
							hr = E_OUTOFMEMORY;
						}
						auto pDescriptorRanges_1_0 = reinterpret_cast<D3D12_DESCRIPTOR_RANGE*>(pDescriptorRanges);

						if (SUCCEEDED(hr))
						{
							for (UINT x = 0; x < table_1_1.NumDescriptorRanges; x++)
							{
								__analysis_assume(DescriptorRangesSize == sizeof(D3D12_DESCRIPTOR_RANGE) * table_1_1.NumDescriptorRanges);
								pDescriptorRanges_1_0[x].BaseShaderRegister = table_1_1.pDescriptorRanges[x].BaseShaderRegister;
								pDescriptorRanges_1_0[x].NumDescriptors = table_1_1.pDescriptorRanges[x].NumDescriptors;
								pDescriptorRanges_1_0[x].OffsetInDescriptorsFromTableStart = table_1_1.pDescriptorRanges[x].OffsetInDescriptorsFromTableStart;
								pDescriptorRanges_1_0[x].RangeType = table_1_1.pDescriptorRanges[x].RangeType;
								pDescriptorRanges_1_0[x].RegisterSpace = table_1_1.pDescriptorRanges[x].RegisterSpace;
							}
						}

						D3D12_ROOT_DESCRIPTOR_TABLE& table_1_0 = pParameters_1_0[n].DescriptorTable;
						table_1_0.NumDescriptorRanges = table_1_1.NumDescriptorRanges;
						table_1_0.pDescriptorRanges = pDescriptorRanges_1_0;
					}
				}
			}

			if (SUCCEEDED(hr))
			{
				XD3D12_ROOT_SIGNATURE_DESC desc_1_0(desc_1_1.NumParameters, pParameters_1_0, desc_1_1.NumStaticSamplers, desc_1_1.pStaticSamplers, desc_1_1.Flags);
				hr = D3D12SerializeRootSignature(&desc_1_0, D3D_ROOT_SIGNATURE_VERSION_1, ppBlob, ppErrorBlob);
			}

			if (pParameters)
			{
				for (UINT n = 0; n < desc_1_1.NumParameters; n++)
				{
					if (desc_1_1.pParameters[n].ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
					{
						HeapFree(GetProcessHeap(), 0, reinterpret_cast<void*>(const_cast<D3D12_DESCRIPTOR_RANGE*>(pParameters_1_0[n].DescriptorTable.pDescriptorRanges)));
					}
				}
				HeapFree(GetProcessHeap(), 0, pParameters);
			}
			return hr;
		}
		}
		break;

	case D3D_ROOT_SIGNATURE_VERSION_1_1:
		return D3D12SerializeVersionedRootSignature(pRootSignatureDesc, ppBlob, ppErrorBlob);
	}

	return E_INVALIDARG;
}
