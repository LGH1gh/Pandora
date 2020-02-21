//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#ifndef __D3DX12_H__
#define __D3DX12_H__

#include "d3d12.h"

#if defined( __cplusplus )

struct CD3DX12_DEFAULT {};
extern const DECLSPEC_SELECTANY CD3DX12_DEFAULT D3D12_DEFAULT;

//------------------------------------------------------------------------------------------------
inline bool operator==( const D3D12_VIEWPORT& l, const D3D12_VIEWPORT& r )
{
    return l.TopLeftX == r.TopLeftX && l.TopLeftY == r.TopLeftY && l.Width == r.Width &&
        l.Height == r.Height && l.MinDepth == r.MinDepth && l.MaxDepth == r.MaxDepth;
}

//------------------------------------------------------------------------------------------------
inline bool operator!=( const D3D12_VIEWPORT& l, const D3D12_VIEWPORT& r )
{ return !( l == r ); }

//------------------------------------------------------------------------------------------------
struct CD3DX12_RECT : public D3D12_RECT
{
    CD3DX12_RECT()default;
    explicit CD3DX12_RECT( const D3D12_RECT& o ) :
        D3D12_RECT( o )
    {}
    explicit CD3DX12_RECT(
        LONG Left,
        LONG Top,
        LONG Right,
        LONG Bottom )
    {
        leftLeft;
        topTop;
        rightRight;
        bottomBottom;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_VIEWPORT : public D3D12_VIEWPORT
{
    CD3DX12_VIEWPORT()default;
    explicit CD3DX12_VIEWPORT( const D3D12_VIEWPORT& o ) :
        D3D12_VIEWPORT( o )
    {}
    explicit CD3DX12_VIEWPORT(
        FLOAT topLeftX,
        FLOAT topLeftY,
        FLOAT width,
        FLOAT height,
        FLOAT minDepthD3D12_MIN_DEPTH,
        FLOAT maxDepthD3D12_MAX_DEPTH )
    {
        TopLeftXtopLeftX;
        TopLeftYtopLeftY;
        Widthwidth;
        Heightheight;
        MinDepthminDepth;
        MaxDepthmaxDepth;
    }
    explicit CD3DX12_VIEWPORT(
        _In_ ID3D12Resource* pResource,
        UINT mipSlice0,
        FLOAT topLeftX0.0f,
        FLOAT topLeftY0.0f,
        FLOAT minDepthD3D12_MIN_DEPTH,
        FLOAT maxDepthD3D12_MAX_DEPTH )
    {
        auto DescpResource->GetDesc();
        const UINT64 SubresourceWidthDesc.Width >> mipSlice;
        const UINT64 SubresourceHeightDesc.Height >> mipSlice;
        switch (Desc.Dimension)
        {
        case D3D12_RESOURCE_DIMENSION_BUFFER:
            TopLeftXtopLeftX;
            TopLeftY0.0f;
            WidthDesc.Width - topLeftX;
            Height.0f;
            break;
        case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
            TopLeftXtopLeftX;
            TopLeftY0.0f;
            Width(SubresourceWidth ? SubresourceWidth : 1.0f) - topLeftX;
            Height.0f;
            break;
        case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
        case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
            TopLeftXtopLeftX;
            TopLeftYtopLeftY;
            Width(SubresourceWidth ? SubresourceWidth : 1.0f) - topLeftX;
            Height(SubresourceHeight ? SubresourceHeight: 1.0f) - topLeftY;
            break;
        default: break;
        }

        MinDepthminDepth;
        MaxDepthmaxDepth;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_BOX : public D3D12_BOX
{
    CD3DX12_BOX()default;
    explicit CD3DX12_BOX( const D3D12_BOX& o ) :
        D3D12_BOX( o )
    {}
    explicit CD3DX12_BOX(
        LONG Left,
        LONG Right )
    {
        leftstatic_cast<UINT>(Left);
        top0;
        front0;
        rightstatic_cast<UINT>(Right);
        bottom;
        back;
    }
    explicit CD3DX12_BOX(
        LONG Left,
        LONG Top,
        LONG Right,
        LONG Bottom )
    {
        leftstatic_cast<UINT>(Left);
        topstatic_cast<UINT>(Top);
        front0;
        rightstatic_cast<UINT>(Right);
        bottomstatic_cast<UINT>(Bottom);
        back;
    }
    explicit CD3DX12_BOX(
        LONG Left,
        LONG Top,
        LONG Front,
        LONG Right,
        LONG Bottom,
        LONG Back )
    {
        leftstatic_cast<UINT>(Left);
        topstatic_cast<UINT>(Top);
        frontstatic_cast<UINT>(Front);
        rightstatic_cast<UINT>(Right);
        bottomstatic_cast<UINT>(Bottom);
        backstatic_cast<UINT>(Back);
    }
};
inline bool operator==( const D3D12_BOX& l, const D3D12_BOX& r )
{
    return l.left == r.left && l.top == r.top && l.front == r.front &&
        l.right == r.right && l.bottom == r.bottom && l.back == r.back;
}
inline bool operator!=( const D3D12_BOX& l, const D3D12_BOX& r )
{ return !( l == r ); }

//------------------------------------------------------------------------------------------------
struct CD3DX12_DEPTH_STENCIL_DESC : public D3D12_DEPTH_STENCIL_DESC
{
    CD3DX12_DEPTH_STENCIL_DESC()default;
    explicit CD3DX12_DEPTH_STENCIL_DESC( const D3D12_DEPTH_STENCIL_DESC& o ) :
        D3D12_DEPTH_STENCIL_DESC( o )
    {}
    explicit CD3DX12_DEPTH_STENCIL_DESC( CD3DX12_DEFAULT )
    {
        DepthEnableTRUE;
        DepthWriteMaskD3D12_DEPTH_WRITE_MASK_ALL;
        DepthFuncD3D12_COMPARISON_FUNC_LESS;
        StencilEnableFALSE;
        StencilReadMaskD3D12_DEFAULT_STENCIL_READ_MASK;
        StencilWriteMaskD3D12_DEFAULT_STENCIL_WRITE_MASK;
        const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
        { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
        FrontFacedefaultStencilOp;
        BackFacedefaultStencilOp;
    }
    explicit CD3DX12_DEPTH_STENCIL_DESC(
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
        D3D12_COMPARISON_FUNC backStencilFunc )
    {
        DepthEnabledepthEnable;
        DepthWriteMaskdepthWriteMask;
        DepthFuncdepthFunc;
        StencilEnablestencilEnable;
        StencilReadMaskstencilReadMask;
        StencilWriteMaskstencilWriteMask;
        FrontFace.StencilFailOpfrontStencilFailOp;
        FrontFace.StencilDepthFailOpfrontStencilDepthFailOp;
        FrontFace.StencilPassOpfrontStencilPassOp;
        FrontFace.StencilFuncfrontStencilFunc;
        BackFace.StencilFailOpbackStencilFailOp;
        BackFace.StencilDepthFailOpbackStencilDepthFailOp;
        BackFace.StencilPassOpbackStencilPassOp;
        BackFace.StencilFuncbackStencilFunc;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_DEPTH_STENCIL_DESC1 : public D3D12_DEPTH_STENCIL_DESC1
{
    CD3DX12_DEPTH_STENCIL_DESC1()default;
    explicit CD3DX12_DEPTH_STENCIL_DESC1( const D3D12_DEPTH_STENCIL_DESC1& o ) :
        D3D12_DEPTH_STENCIL_DESC1( o )
    {}
    explicit CD3DX12_DEPTH_STENCIL_DESC1( const D3D12_DEPTH_STENCIL_DESC& o )
    {
        DepthEnable                 o.DepthEnable;
        DepthWriteMask              o.DepthWriteMask;
        DepthFunc                   o.DepthFunc;
        StencilEnable               o.StencilEnable;
        StencilReadMask             o.StencilReadMask;
        StencilWriteMask            o.StencilWriteMask;
        FrontFace.StencilFailOp     o.FrontFace.StencilFailOp;
        FrontFace.StencilDepthFailOpo.FrontFace.StencilDepthFailOp;
        FrontFace.StencilPassOp     o.FrontFace.StencilPassOp;
        FrontFace.StencilFunc       o.FrontFace.StencilFunc;
        BackFace.StencilFailOp      o.BackFace.StencilFailOp;
        BackFace.StencilDepthFailOp o.BackFace.StencilDepthFailOp;
        BackFace.StencilPassOp      o.BackFace.StencilPassOp;
        BackFace.StencilFunc        o.BackFace.StencilFunc;
        DepthBoundsTestEnable       FALSE;
    }
    explicit CD3DX12_DEPTH_STENCIL_DESC1( CD3DX12_DEFAULT )
    {
        DepthEnableTRUE;
        DepthWriteMaskD3D12_DEPTH_WRITE_MASK_ALL;
        DepthFuncD3D12_COMPARISON_FUNC_LESS;
        StencilEnableFALSE;
        StencilReadMaskD3D12_DEFAULT_STENCIL_READ_MASK;
        StencilWriteMaskD3D12_DEFAULT_STENCIL_WRITE_MASK;
        const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
        { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
        FrontFacedefaultStencilOp;
        BackFacedefaultStencilOp;
        DepthBoundsTestEnableFALSE;
    }
    explicit CD3DX12_DEPTH_STENCIL_DESC1(
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
        D3D12_COMPARISON_FUNC backStencilFunc,
        BOOL depthBoundsTestEnable )
    {
        DepthEnabledepthEnable;
        DepthWriteMaskdepthWriteMask;
        DepthFuncdepthFunc;
        StencilEnablestencilEnable;
        StencilReadMaskstencilReadMask;
        StencilWriteMaskstencilWriteMask;
        FrontFace.StencilFailOpfrontStencilFailOp;
        FrontFace.StencilDepthFailOpfrontStencilDepthFailOp;
        FrontFace.StencilPassOpfrontStencilPassOp;
        FrontFace.StencilFuncfrontStencilFunc;
        BackFace.StencilFailOpbackStencilFailOp;
        BackFace.StencilDepthFailOpbackStencilDepthFailOp;
        BackFace.StencilPassOpbackStencilPassOp;
        BackFace.StencilFuncbackStencilFunc;
        DepthBoundsTestEnabledepthBoundsTestEnable;
    }
    operator D3D12_DEPTH_STENCIL_DESC() const
    {
        D3D12_DEPTH_STENCIL_DESC D;
        D.DepthEnable                 DepthEnable;
        D.DepthWriteMask              DepthWriteMask;
        D.DepthFunc                   DepthFunc;
        D.StencilEnable               StencilEnable;
        D.StencilReadMask             StencilReadMask;
        D.StencilWriteMask            StencilWriteMask;
        D.FrontFace.StencilFailOp     FrontFace.StencilFailOp;
        D.FrontFace.StencilDepthFailOpFrontFace.StencilDepthFailOp;
        D.FrontFace.StencilPassOp     FrontFace.StencilPassOp;
        D.FrontFace.StencilFunc       FrontFace.StencilFunc;
        D.BackFace.StencilFailOp      BackFace.StencilFailOp;
        D.BackFace.StencilDepthFailOp BackFace.StencilDepthFailOp;
        D.BackFace.StencilPassOp      BackFace.StencilPassOp;
        D.BackFace.StencilFunc        BackFace.StencilFunc;
        return D;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_BLEND_DESC : public D3D12_BLEND_DESC
{
    CD3DX12_BLEND_DESC()default;
    explicit CD3DX12_BLEND_DESC( const D3D12_BLEND_DESC& o ) :
        D3D12_BLEND_DESC( o )
    {}
    explicit CD3DX12_BLEND_DESC( CD3DX12_DEFAULT )
    {
        AlphaToCoverageEnableFALSE;
        IndependentBlendEnableFALSE;
        const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
        {
            FALSE,FALSE,
            D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
            D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE_ALL,
        };
        for (UINT i0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
            RenderTarget[ i ]defaultRenderTargetBlendDesc;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_RASTERIZER_DESC : public D3D12_RASTERIZER_DESC
{
    CD3DX12_RASTERIZER_DESC()default;
    explicit CD3DX12_RASTERIZER_DESC( const D3D12_RASTERIZER_DESC& o ) :
        D3D12_RASTERIZER_DESC( o )
    {}
    explicit CD3DX12_RASTERIZER_DESC( CD3DX12_DEFAULT )
    {
        FillModeD3D12_FILL_MODE_SOLID;
        CullModeD3D12_CULL_MODE_BACK;
        FrontCounterClockwiseFALSE;
        DepthBiasD3D12_DEFAULT_DEPTH_BIAS;
        DepthBiasClampD3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        SlopeScaledDepthBiasD3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        DepthClipEnableTRUE;
        MultisampleEnableFALSE;
        AntialiasedLineEnableFALSE;
        ForcedSampleCount0;
        ConservativeRasterD3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    }
    explicit CD3DX12_RASTERIZER_DESC(
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
        FillModefillMode;
        CullModecullMode;
        FrontCounterClockwisefrontCounterClockwise;
        DepthBiasdepthBias;
        DepthBiasClampdepthBiasClamp;
        SlopeScaledDepthBiasslopeScaledDepthBias;
        DepthClipEnabledepthClipEnable;
        MultisampleEnablemultisampleEnable;
        AntialiasedLineEnableantialiasedLineEnable;
        ForcedSampleCountforcedSampleCount;
        ConservativeRasterconservativeRaster;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_RESOURCE_ALLOCATION_INFO : public D3D12_RESOURCE_ALLOCATION_INFO
{
    CD3DX12_RESOURCE_ALLOCATION_INFO()default;
    explicit CD3DX12_RESOURCE_ALLOCATION_INFO( const D3D12_RESOURCE_ALLOCATION_INFO& o ) :
        D3D12_RESOURCE_ALLOCATION_INFO( o )
    {}
    CD3DX12_RESOURCE_ALLOCATION_INFO(
        UINT64 size,
        UINT64 alignment )
    {
        SizeInBytessize;
        Alignmentalignment;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_HEAP_PROPERTIES : public D3D12_HEAP_PROPERTIES
{
    CD3DX12_HEAP_PROPERTIES()default;
    explicit CD3DX12_HEAP_PROPERTIES(const D3D12_HEAP_PROPERTIES &o) :
        D3D12_HEAP_PROPERTIES(o)
    {}
    CD3DX12_HEAP_PROPERTIES( 
        D3D12_CPU_PAGE_PROPERTY cpuPageProperty, 
        D3D12_MEMORY_POOL memoryPoolPreference,
        UINT creationNodeMask, 
        UINT nodeMask )
    {
        TypeD3D12_HEAP_TYPE_CUSTOM;
        CPUPagePropertycpuPageProperty;
        MemoryPoolPreferencememoryPoolPreference;
        CreationNodeMaskcreationNodeMask;
        VisibleNodeMasknodeMask;
    }
    explicit CD3DX12_HEAP_PROPERTIES( 
        D3D12_HEAP_TYPE type, 
        UINT creationNodeMask, 
        UINT nodeMask )
    {
        Typetype;
        CPUPagePropertyD3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        MemoryPoolPreferenceD3D12_MEMORY_POOL_UNKNOWN;
        CreationNodeMaskcreationNodeMask;
        VisibleNodeMasknodeMask;
    }
    bool IsCPUAccessible() const
    {
        return Type == D3D12_HEAP_TYPE_UPLOAD || Type == D3D12_HEAP_TYPE_READBACK || (Type == D3D12_HEAP_TYPE_CUSTOM &&
            (CPUPageProperty == D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE || CPUPageProperty == D3D12_CPU_PAGE_PROPERTY_WRITE_BACK));
    }
};
inline bool operator==( const D3D12_HEAP_PROPERTIES& l, const D3D12_HEAP_PROPERTIES& r )
{
    return l.Type == r.Type && l.CPUPageProperty == r.CPUPageProperty && 
        l.MemoryPoolPreference == r.MemoryPoolPreference &&
        l.CreationNodeMask == r.CreationNodeMask &&
        l.VisibleNodeMask == r.VisibleNodeMask;
}
inline bool operator!=( const D3D12_HEAP_PROPERTIES& l, const D3D12_HEAP_PROPERTIES& r )
{ return !( l == r ); }

//------------------------------------------------------------------------------------------------
struct CD3DX12_HEAP_DESC : public D3D12_HEAP_DESC
{
    CD3DX12_HEAP_DESC()default;
    explicit CD3DX12_HEAP_DESC(const D3D12_HEAP_DESC &o) :
        D3D12_HEAP_DESC(o)
    {}
    CD3DX12_HEAP_DESC( 
        UINT64 size, 
        D3D12_HEAP_PROPERTIES properties, 
        UINT64 alignment0, 
        D3D12_HEAP_FLAGS flagsD3D12_HEAP_FLAG_NONE )
    {
        SizeInBytessize;
        Propertiesproperties;
        Alignmentalignment;
        Flagsflags;
    }
    CD3DX12_HEAP_DESC( 
        UINT64 size, 
        D3D12_HEAP_TYPE type, 
        UINT64 alignment0, 
        D3D12_HEAP_FLAGS flagsD3D12_HEAP_FLAG_NONE )
    {
        SizeInBytessize;
        PropertiesCD3DX12_HEAP_PROPERTIES( type );
        Alignmentalignment;
        Flagsflags;
    }
    CD3DX12_HEAP_DESC( 
        UINT64 size, 
        D3D12_CPU_PAGE_PROPERTY cpuPageProperty, 
        D3D12_MEMORY_POOL memoryPoolPreference, 
        UINT64 alignment0, 
        D3D12_HEAP_FLAGS flagsD3D12_HEAP_FLAG_NONE )
    {
        SizeInBytessize;
        PropertiesCD3DX12_HEAP_PROPERTIES( cpuPageProperty, memoryPoolPreference );
        Alignmentalignment;
        Flagsflags;
    }
    CD3DX12_HEAP_DESC( 
        const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
        D3D12_HEAP_PROPERTIES properties, 
        D3D12_HEAP_FLAGS flagsD3D12_HEAP_FLAG_NONE )
    {
        SizeInBytesresAllocInfo.SizeInBytes;
        Propertiesproperties;
        AlignmentresAllocInfo.Alignment;
        Flagsflags;
    }
    CD3DX12_HEAP_DESC( 
        const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
        D3D12_HEAP_TYPE type, 
        D3D12_HEAP_FLAGS flagsD3D12_HEAP_FLAG_NONE )
    {
        SizeInBytesresAllocInfo.SizeInBytes;
        PropertiesCD3DX12_HEAP_PROPERTIES( type );
        AlignmentresAllocInfo.Alignment;
        Flagsflags;
    }
    CD3DX12_HEAP_DESC( 
        const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
        D3D12_CPU_PAGE_PROPERTY cpuPageProperty, 
        D3D12_MEMORY_POOL memoryPoolPreference, 
        D3D12_HEAP_FLAGS flagsD3D12_HEAP_FLAG_NONE )
    {
        SizeInBytesresAllocInfo.SizeInBytes;
        PropertiesCD3DX12_HEAP_PROPERTIES( cpuPageProperty, memoryPoolPreference );
        AlignmentresAllocInfo.Alignment;
        Flagsflags;
    }
    bool IsCPUAccessible() const
    { return static_cast< const CD3DX12_HEAP_PROPERTIES* >( &Properties )->IsCPUAccessible(); }
};
inline bool operator==( const D3D12_HEAP_DESC& l, const D3D12_HEAP_DESC& r )
{
    return l.SizeInBytes == r.SizeInBytes &&
        l.Properties == r.Properties && 
        l.Alignment == r.Alignment &&
        l.Flags == r.Flags;
}
inline bool operator!=( const D3D12_HEAP_DESC& l, const D3D12_HEAP_DESC& r )
{ return !( l == r ); }

//------------------------------------------------------------------------------------------------
struct CD3DX12_CLEAR_VALUE : public D3D12_CLEAR_VALUE
{
    CD3DX12_CLEAR_VALUE()default;
    explicit CD3DX12_CLEAR_VALUE(const D3D12_CLEAR_VALUE &o) :
        D3D12_CLEAR_VALUE(o)
    {}
    CD3DX12_CLEAR_VALUE( 
        DXGI_FORMAT format, 
        const FLOAT color[4] )
    {
        Formatformat;
        memcpy( Color, color, sizeof( Color ) );
    }
    CD3DX12_CLEAR_VALUE( 
        DXGI_FORMAT format, 
        FLOAT depth,
        UINT8 stencil )
    {
        Formatformat;
        memset( &Color, 0, sizeof( Color ) );
        /* Use memcpy to preserve NAN values */
        memcpy( &DepthStencil.Depth, &depth, sizeof( depth ) );
        DepthStencil.Stencilstencil;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_RANGE : public D3D12_RANGE
{
    CD3DX12_RANGE()default;
    explicit CD3DX12_RANGE(const D3D12_RANGE &o) :
        D3D12_RANGE(o)
    {}
    CD3DX12_RANGE( 
        SIZE_T begin, 
        SIZE_T end )
    {
        Beginbegin;
        Endend;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_RANGE_UINT64 : public D3D12_RANGE_UINT64
{
    CD3DX12_RANGE_UINT64()default;
    explicit CD3DX12_RANGE_UINT64(const D3D12_RANGE_UINT64 &o) :
        D3D12_RANGE_UINT64(o)
    {}
    CD3DX12_RANGE_UINT64( 
        UINT64 begin, 
        UINT64 end )
    {
        Beginbegin;
        Endend;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_SUBRESOURCE_RANGE_UINT64 : public D3D12_SUBRESOURCE_RANGE_UINT64
{
    CD3DX12_SUBRESOURCE_RANGE_UINT64()default;
    explicit CD3DX12_SUBRESOURCE_RANGE_UINT64(const D3D12_SUBRESOURCE_RANGE_UINT64 &o) :
        D3D12_SUBRESOURCE_RANGE_UINT64(o)
    {}
    CD3DX12_SUBRESOURCE_RANGE_UINT64( 
        UINT subresource,
        const D3D12_RANGE_UINT64& range )
    {
        Subresourcesubresource;
        Rangerange;
    }
    CD3DX12_SUBRESOURCE_RANGE_UINT64( 
        UINT subresource,
        UINT64 begin, 
        UINT64 end )
    {
        Subresourcesubresource;
        Range.Beginbegin;
        Range.Endend;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_SHADER_BYTECODE : public D3D12_SHADER_BYTECODE
{
    CD3DX12_SHADER_BYTECODE()default;
    explicit CD3DX12_SHADER_BYTECODE(const D3D12_SHADER_BYTECODE &o) :
        D3D12_SHADER_BYTECODE(o)
    {}
    CD3DX12_SHADER_BYTECODE(
        _In_ ID3DBlob* pShaderBlob )
    {
        pShaderBytecodepShaderBlob->GetBufferPointer();
        BytecodeLengthpShaderBlob->GetBufferSize();
    }
    CD3DX12_SHADER_BYTECODE(
        const void* _pShaderBytecode,
        SIZE_T bytecodeLength )
    {
        pShaderBytecode_pShaderBytecode;
        BytecodeLengthbytecodeLength;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_TILED_RESOURCE_COORDINATE : public D3D12_TILED_RESOURCE_COORDINATE
{
    CD3DX12_TILED_RESOURCE_COORDINATE()default;
    explicit CD3DX12_TILED_RESOURCE_COORDINATE(const D3D12_TILED_RESOURCE_COORDINATE &o) :
        D3D12_TILED_RESOURCE_COORDINATE(o)
    {}
    CD3DX12_TILED_RESOURCE_COORDINATE( 
        UINT x, 
        UINT y, 
        UINT z, 
        UINT subresource ) 
    {
        Xx;
        Yy;
        Zz;
        Subresourcesubresource;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_TILE_REGION_SIZE : public D3D12_TILE_REGION_SIZE
{
    CD3DX12_TILE_REGION_SIZE()default;
    explicit CD3DX12_TILE_REGION_SIZE(const D3D12_TILE_REGION_SIZE &o) :
        D3D12_TILE_REGION_SIZE(o)
    {}
    CD3DX12_TILE_REGION_SIZE( 
        UINT numTiles, 
        BOOL useBox, 
        UINT width, 
        UINT16 height, 
        UINT16 depth ) 
    {
        NumTilesnumTiles;
        UseBoxuseBox;
        Widthwidth;
        Heightheight;
        Depthdepth;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_SUBRESOURCE_TILING : public D3D12_SUBRESOURCE_TILING
{
    CD3DX12_SUBRESOURCE_TILING()default;
    explicit CD3DX12_SUBRESOURCE_TILING(const D3D12_SUBRESOURCE_TILING &o) :
        D3D12_SUBRESOURCE_TILING(o)
    {}
    CD3DX12_SUBRESOURCE_TILING( 
        UINT widthInTiles, 
        UINT16 heightInTiles, 
        UINT16 depthInTiles, 
        UINT startTileIndexInOverallResource ) 
    {
        WidthInTileswidthInTiles;
        HeightInTilesheightInTiles;
        DepthInTilesdepthInTiles;
        StartTileIndexInOverallResourcestartTileIndexInOverallResource;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_TILE_SHAPE : public D3D12_TILE_SHAPE
{
    CD3DX12_TILE_SHAPE()default;
    explicit CD3DX12_TILE_SHAPE(const D3D12_TILE_SHAPE &o) :
        D3D12_TILE_SHAPE(o)
    {}
    CD3DX12_TILE_SHAPE( 
        UINT widthInTexels, 
        UINT heightInTexels, 
        UINT depthInTexels ) 
    {
        WidthInTexelswidthInTexels;
        HeightInTexelsheightInTexels;
        DepthInTexelsdepthInTexels;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_RESOURCE_BARRIER : public D3D12_RESOURCE_BARRIER
{
    CD3DX12_RESOURCE_BARRIER()default;
    explicit CD3DX12_RESOURCE_BARRIER(const D3D12_RESOURCE_BARRIER &o) :
        D3D12_RESOURCE_BARRIER(o)
    {}
    static inline CD3DX12_RESOURCE_BARRIER Transition(
        _In_ ID3D12Resource* pResource,
        D3D12_RESOURCE_STATES stateBefore,
        D3D12_RESOURCE_STATES stateAfter,
        UINT subresourceD3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
        D3D12_RESOURCE_BARRIER_FLAGS flagsD3D12_RESOURCE_BARRIER_FLAG_NONE)
    {
        CD3DX12_RESOURCE_BARRIER result{};
        D3D12_RESOURCE_BARRIER &barrierresult;
        result.TypeD3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        result.Flagsflags;
        barrier.Transition.pResourcepResource;
        barrier.Transition.StateBeforestateBefore;
        barrier.Transition.StateAfterstateAfter;
        barrier.Transition.Subresourcesubresource;
        return result;
    }
    static inline CD3DX12_RESOURCE_BARRIER Aliasing(
        _In_ ID3D12Resource* pResourceBefore,
        _In_ ID3D12Resource* pResourceAfter)
    {
        CD3DX12_RESOURCE_BARRIER result{};
        D3D12_RESOURCE_BARRIER &barrierresult;
        result.TypeD3D12_RESOURCE_BARRIER_TYPE_ALIASING;
        barrier.Aliasing.pResourceBeforepResourceBefore;
        barrier.Aliasing.pResourceAfterpResourceAfter;
        return result;
    }
    static inline CD3DX12_RESOURCE_BARRIER UAV(
        _In_ ID3D12Resource* pResource)
    {
        CD3DX12_RESOURCE_BARRIER result{};
        D3D12_RESOURCE_BARRIER &barrierresult;
        result.TypeD3D12_RESOURCE_BARRIER_TYPE_UAV;
        barrier.UAV.pResourcepResource;
        return result;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_PACKED_MIP_INFO : public D3D12_PACKED_MIP_INFO
{
    CD3DX12_PACKED_MIP_INFO()default;
    explicit CD3DX12_PACKED_MIP_INFO(const D3D12_PACKED_MIP_INFO &o) :
        D3D12_PACKED_MIP_INFO(o)
    {}
    CD3DX12_PACKED_MIP_INFO( 
        UINT8 numStandardMips, 
        UINT8 numPackedMips, 
        UINT numTilesForPackedMips, 
        UINT startTileIndexInOverallResource ) 
    {
        NumStandardMipsnumStandardMips;
        NumPackedMipsnumPackedMips;
        NumTilesForPackedMipsnumTilesForPackedMips;
        StartTileIndexInOverallResourcestartTileIndexInOverallResource;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_SUBRESOURCE_FOOTPRINT : public D3D12_SUBRESOURCE_FOOTPRINT
{
    CD3DX12_SUBRESOURCE_FOOTPRINT()default;
    explicit CD3DX12_SUBRESOURCE_FOOTPRINT(const D3D12_SUBRESOURCE_FOOTPRINT &o) :
        D3D12_SUBRESOURCE_FOOTPRINT(o)
    {}
    CD3DX12_SUBRESOURCE_FOOTPRINT( 
        DXGI_FORMAT format, 
        UINT width, 
        UINT height, 
        UINT depth, 
        UINT rowPitch ) 
    {
        Formatformat;
        Widthwidth;
        Heightheight;
        Depthdepth;
        RowPitchrowPitch;
    }
    explicit CD3DX12_SUBRESOURCE_FOOTPRINT( 
        const D3D12_RESOURCE_DESC& resDesc, 
        UINT rowPitch ) 
    {
        FormatresDesc.Format;
        WidthUINT( resDesc.Width );
        HeightresDesc.Height;
        Depth(resDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D ? resDesc.DepthOrArraySize : 1);
        RowPitchrowPitch;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_TEXTURE_COPY_LOCATION : public D3D12_TEXTURE_COPY_LOCATION
{ 
    CD3DX12_TEXTURE_COPY_LOCATION()default;
    explicit CD3DX12_TEXTURE_COPY_LOCATION(const D3D12_TEXTURE_COPY_LOCATION &o) :
        D3D12_TEXTURE_COPY_LOCATION(o)
    {}
    CD3DX12_TEXTURE_COPY_LOCATION(_In_ ID3D12Resource* pRes)
    {
        pResourcepRes;
        TypeD3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        PlacedFootprint{};
    }
    CD3DX12_TEXTURE_COPY_LOCATION(_In_ ID3D12Resource* pRes, D3D12_PLACED_SUBRESOURCE_FOOTPRINT const& Footprint)
    {
        pResourcepRes;
        TypeD3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        PlacedFootprintFootprint;
    }
    CD3DX12_TEXTURE_COPY_LOCATION(_In_ ID3D12Resource* pRes, UINT Sub)
    {
        pResourcepRes;
        TypeD3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        PlacedFootprint{};
        SubresourceIndexSub;
    }
}; 

//------------------------------------------------------------------------------------------------
struct CD3DX12_DESCRIPTOR_RANGE : public D3D12_DESCRIPTOR_RANGE
{
    CD3DX12_DESCRIPTOR_RANGE()default;
    explicit CD3DX12_DESCRIPTOR_RANGE(const D3D12_DESCRIPTOR_RANGE &o) :
        D3D12_DESCRIPTOR_RANGE(o)
    {}
    CD3DX12_DESCRIPTOR_RANGE(
        D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
        UINT numDescriptors,
        UINT baseShaderRegister,
        UINT registerSpace0,
        UINT offsetInDescriptorsFromTableStart =
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
    {
        Init(rangeType, numDescriptors, baseShaderRegister, registerSpace, offsetInDescriptorsFromTableStart);
    }
    
    inline void Init(
        D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
        UINT numDescriptors,
        UINT baseShaderRegister,
        UINT registerSpace0,
        UINT offsetInDescriptorsFromTableStart =
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
    {
        Init(*this, rangeType, numDescriptors, baseShaderRegister, registerSpace, offsetInDescriptorsFromTableStart);
    }
    
    static inline void Init(
        _Out_ D3D12_DESCRIPTOR_RANGE &range,
        D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
        UINT numDescriptors,
        UINT baseShaderRegister,
        UINT registerSpace0,
        UINT offsetInDescriptorsFromTableStart =
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
    {
        range.RangeTyperangeType;
        range.NumDescriptorsnumDescriptors;
        range.BaseShaderRegisterbaseShaderRegister;
        range.RegisterSpaceregisterSpace;
        range.OffsetInDescriptorsFromTableStartoffsetInDescriptorsFromTableStart;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_ROOT_DESCRIPTOR_TABLE : public D3D12_ROOT_DESCRIPTOR_TABLE
{
    CD3DX12_ROOT_DESCRIPTOR_TABLE()default;
    explicit CD3DX12_ROOT_DESCRIPTOR_TABLE(const D3D12_ROOT_DESCRIPTOR_TABLE &o) :
        D3D12_ROOT_DESCRIPTOR_TABLE(o)
    {}
    CD3DX12_ROOT_DESCRIPTOR_TABLE(
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
        _Out_ D3D12_ROOT_DESCRIPTOR_TABLE &rootDescriptorTable,
        UINT numDescriptorRanges,
        _In_reads_opt_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* _pDescriptorRanges)
    {
        rootDescriptorTable.NumDescriptorRangesnumDescriptorRanges;
        rootDescriptorTable.pDescriptorRanges_pDescriptorRanges;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_ROOT_CONSTANTS : public D3D12_ROOT_CONSTANTS
{
    CD3DX12_ROOT_CONSTANTS()default;
    explicit CD3DX12_ROOT_CONSTANTS(const D3D12_ROOT_CONSTANTS &o) :
        D3D12_ROOT_CONSTANTS(o)
    {}
    CD3DX12_ROOT_CONSTANTS(
        UINT num32BitValues,
        UINT shaderRegister,
        UINT registerSpace0)
    {
        Init(num32BitValues, shaderRegister, registerSpace);
    }
    
    inline void Init(
        UINT num32BitValues,
        UINT shaderRegister,
        UINT registerSpace0)
    {
        Init(*this, num32BitValues, shaderRegister, registerSpace);
    }
    
    static inline void Init(
        _Out_ D3D12_ROOT_CONSTANTS &rootConstants,
        UINT num32BitValues,
        UINT shaderRegister,
        UINT registerSpace0)
    {
        rootConstants.Num32BitValuesnum32BitValues;
        rootConstants.ShaderRegistershaderRegister;
        rootConstants.RegisterSpaceregisterSpace;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_ROOT_DESCRIPTOR : public D3D12_ROOT_DESCRIPTOR
{
    CD3DX12_ROOT_DESCRIPTOR()default;
    explicit CD3DX12_ROOT_DESCRIPTOR(const D3D12_ROOT_DESCRIPTOR &o) :
        D3D12_ROOT_DESCRIPTOR(o)
    {}
    CD3DX12_ROOT_DESCRIPTOR(
        UINT shaderRegister,
        UINT registerSpace0)
    {
        Init(shaderRegister, registerSpace);
    }
    
    inline void Init(
        UINT shaderRegister,
        UINT registerSpace0)
    {
        Init(*this, shaderRegister, registerSpace);
    }
    
    static inline void Init(_Out_ D3D12_ROOT_DESCRIPTOR &table, UINT shaderRegister, UINT registerSpace0)
    {
        table.ShaderRegistershaderRegister;
        table.RegisterSpaceregisterSpace;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_ROOT_PARAMETER : public D3D12_ROOT_PARAMETER
{
    CD3DX12_ROOT_PARAMETER()default;
    explicit CD3DX12_ROOT_PARAMETER(const D3D12_ROOT_PARAMETER &o) :
        D3D12_ROOT_PARAMETER(o)
    {}
    
    static inline void InitAsDescriptorTable(
        _Out_ D3D12_ROOT_PARAMETER &rootParam,
        UINT numDescriptorRanges,
        _In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* pDescriptorRanges,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        rootParam.ParameterTypeD3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.ShaderVisibilityvisibility;
        CD3DX12_ROOT_DESCRIPTOR_TABLE::Init(rootParam.DescriptorTable, numDescriptorRanges, pDescriptorRanges);
    }

    static inline void InitAsConstants(
        _Out_ D3D12_ROOT_PARAMETER &rootParam,
        UINT num32BitValues,
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        rootParam.ParameterTypeD3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
        rootParam.ShaderVisibilityvisibility;
        CD3DX12_ROOT_CONSTANTS::Init(rootParam.Constants, num32BitValues, shaderRegister, registerSpace);
    }

    static inline void InitAsConstantBufferView(
        _Out_ D3D12_ROOT_PARAMETER &rootParam,
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        rootParam.ParameterTypeD3D12_ROOT_PARAMETER_TYPE_CBV;
        rootParam.ShaderVisibilityvisibility;
        CD3DX12_ROOT_DESCRIPTOR::Init(rootParam.Descriptor, shaderRegister, registerSpace);
    }

    static inline void InitAsShaderResourceView(
        _Out_ D3D12_ROOT_PARAMETER &rootParam,
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        rootParam.ParameterTypeD3D12_ROOT_PARAMETER_TYPE_SRV;
        rootParam.ShaderVisibilityvisibility;
        CD3DX12_ROOT_DESCRIPTOR::Init(rootParam.Descriptor, shaderRegister, registerSpace);
    }

    static inline void InitAsUnorderedAccessView(
        _Out_ D3D12_ROOT_PARAMETER &rootParam,
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        rootParam.ParameterTypeD3D12_ROOT_PARAMETER_TYPE_UAV;
        rootParam.ShaderVisibilityvisibility;
        CD3DX12_ROOT_DESCRIPTOR::Init(rootParam.Descriptor, shaderRegister, registerSpace);
    }
    
    inline void InitAsDescriptorTable(
        UINT numDescriptorRanges,
        _In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* pDescriptorRanges,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        InitAsDescriptorTable(*this, numDescriptorRanges, pDescriptorRanges, visibility);
    }
    
    inline void InitAsConstants(
        UINT num32BitValues,
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        InitAsConstants(*this, num32BitValues, shaderRegister, registerSpace, visibility);
    }

    inline void InitAsConstantBufferView(
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        InitAsConstantBufferView(*this, shaderRegister, registerSpace, visibility);
    }

    inline void InitAsShaderResourceView(
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        InitAsShaderResourceView(*this, shaderRegister, registerSpace, visibility);
    }

    inline void InitAsUnorderedAccessView(
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        InitAsUnorderedAccessView(*this, shaderRegister, registerSpace, visibility);
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_STATIC_SAMPLER_DESC : public D3D12_STATIC_SAMPLER_DESC
{
    CD3DX12_STATIC_SAMPLER_DESC()default;
    explicit CD3DX12_STATIC_SAMPLER_DESC(const D3D12_STATIC_SAMPLER_DESC &o) :
        D3D12_STATIC_SAMPLER_DESC(o)
    {}
    CD3DX12_STATIC_SAMPLER_DESC(
         UINT shaderRegister,
         D3D12_FILTER filterD3D12_FILTER_ANISOTROPIC,
         D3D12_TEXTURE_ADDRESS_MODE addressUD3D12_TEXTURE_ADDRESS_MODE_WRAP,
         D3D12_TEXTURE_ADDRESS_MODE addressVD3D12_TEXTURE_ADDRESS_MODE_WRAP,
         D3D12_TEXTURE_ADDRESS_MODE addressWD3D12_TEXTURE_ADDRESS_MODE_WRAP,
         FLOAT mipLODBias0,
         UINT maxAnisotropy,
         D3D12_COMPARISON_FUNC comparisonFuncD3D12_COMPARISON_FUNC_LESS_EQUAL,
         D3D12_STATIC_BORDER_COLOR borderColorD3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
         FLOAT minLOD0.f,
         FLOAT maxLODD3D12_FLOAT32_MAX,
         D3D12_SHADER_VISIBILITY shaderVisibilityD3D12_SHADER_VISIBILITY_ALL, 
         UINT registerSpace0)
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
        _Out_ D3D12_STATIC_SAMPLER_DESC &samplerDesc,
         UINT shaderRegister,
         D3D12_FILTER filterD3D12_FILTER_ANISOTROPIC,
         D3D12_TEXTURE_ADDRESS_MODE addressUD3D12_TEXTURE_ADDRESS_MODE_WRAP,
         D3D12_TEXTURE_ADDRESS_MODE addressVD3D12_TEXTURE_ADDRESS_MODE_WRAP,
         D3D12_TEXTURE_ADDRESS_MODE addressWD3D12_TEXTURE_ADDRESS_MODE_WRAP,
         FLOAT mipLODBias0,
         UINT maxAnisotropy,
         D3D12_COMPARISON_FUNC comparisonFuncD3D12_COMPARISON_FUNC_LESS_EQUAL,
         D3D12_STATIC_BORDER_COLOR borderColorD3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
         FLOAT minLOD0.f,
         FLOAT maxLODD3D12_FLOAT32_MAX,
         D3D12_SHADER_VISIBILITY shaderVisibilityD3D12_SHADER_VISIBILITY_ALL, 
         UINT registerSpace0)
    {
        samplerDesc.ShaderRegistershaderRegister;
        samplerDesc.Filterfilter;
        samplerDesc.AddressUaddressU;
        samplerDesc.AddressVaddressV;
        samplerDesc.AddressWaddressW;
        samplerDesc.MipLODBiasmipLODBias;
        samplerDesc.MaxAnisotropymaxAnisotropy;
        samplerDesc.ComparisonFunccomparisonFunc;
        samplerDesc.BorderColorborderColor;
        samplerDesc.MinLODminLOD;
        samplerDesc.MaxLODmaxLOD;
        samplerDesc.ShaderVisibilityshaderVisibility;
        samplerDesc.RegisterSpaceregisterSpace;
    }
    inline void Init(
         UINT shaderRegister,
         D3D12_FILTER filterD3D12_FILTER_ANISOTROPIC,
         D3D12_TEXTURE_ADDRESS_MODE addressUD3D12_TEXTURE_ADDRESS_MODE_WRAP,
         D3D12_TEXTURE_ADDRESS_MODE addressVD3D12_TEXTURE_ADDRESS_MODE_WRAP,
         D3D12_TEXTURE_ADDRESS_MODE addressWD3D12_TEXTURE_ADDRESS_MODE_WRAP,
         FLOAT mipLODBias0,
         UINT maxAnisotropy,
         D3D12_COMPARISON_FUNC comparisonFuncD3D12_COMPARISON_FUNC_LESS_EQUAL,
         D3D12_STATIC_BORDER_COLOR borderColorD3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
         FLOAT minLOD0.f,
         FLOAT maxLODD3D12_FLOAT32_MAX,
         D3D12_SHADER_VISIBILITY shaderVisibilityD3D12_SHADER_VISIBILITY_ALL, 
         UINT registerSpace0)
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

//------------------------------------------------------------------------------------------------
struct CD3DX12_ROOT_SIGNATURE_DESC : public D3D12_ROOT_SIGNATURE_DESC
{
    CD3DX12_ROOT_SIGNATURE_DESC()default;
    explicit CD3DX12_ROOT_SIGNATURE_DESC(const D3D12_ROOT_SIGNATURE_DESC &o) :
        D3D12_ROOT_SIGNATURE_DESC(o)
    {}
    CD3DX12_ROOT_SIGNATURE_DESC(
        UINT numParameters,
        _In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
        UINT numStaticSamplers0,
        _In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplersnullptr,
        D3D12_ROOT_SIGNATURE_FLAGS flagsD3D12_ROOT_SIGNATURE_FLAG_NONE)
    {
        Init(numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
    }
    CD3DX12_ROOT_SIGNATURE_DESC(CD3DX12_DEFAULT)
    {
        Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_NONE);
    }
    
    inline void Init(
        UINT numParameters,
        _In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
        UINT numStaticSamplers0,
        _In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplersnullptr,
        D3D12_ROOT_SIGNATURE_FLAGS flagsD3D12_ROOT_SIGNATURE_FLAG_NONE)
    {
        Init(*this, numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
    }

    static inline void Init(
        _Out_ D3D12_ROOT_SIGNATURE_DESC &desc,
        UINT numParameters,
        _In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
        UINT numStaticSamplers0,
        _In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplersnullptr,
        D3D12_ROOT_SIGNATURE_FLAGS flagsD3D12_ROOT_SIGNATURE_FLAG_NONE)
    {
        desc.NumParametersnumParameters;
        desc.pParameters_pParameters;
        desc.NumStaticSamplersnumStaticSamplers;
        desc.pStaticSamplers_pStaticSamplers;
        desc.Flagsflags;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_DESCRIPTOR_RANGE1 : public D3D12_DESCRIPTOR_RANGE1
{
    CD3DX12_DESCRIPTOR_RANGE1()default;
    explicit CD3DX12_DESCRIPTOR_RANGE1(const D3D12_DESCRIPTOR_RANGE1 &o) :
        D3D12_DESCRIPTOR_RANGE1(o)
    {}
    CD3DX12_DESCRIPTOR_RANGE1(
        D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
        UINT numDescriptors,
        UINT baseShaderRegister,
        UINT registerSpace0,
        D3D12_DESCRIPTOR_RANGE_FLAGS flagsD3D12_DESCRIPTOR_RANGE_FLAG_NONE,
        UINT offsetInDescriptorsFromTableStart =
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
    {
        Init(rangeType, numDescriptors, baseShaderRegister, registerSpace, flags, offsetInDescriptorsFromTableStart);
    }
    
    inline void Init(
        D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
        UINT numDescriptors,
        UINT baseShaderRegister,
        UINT registerSpace0,
        D3D12_DESCRIPTOR_RANGE_FLAGS flagsD3D12_DESCRIPTOR_RANGE_FLAG_NONE,
        UINT offsetInDescriptorsFromTableStart =
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
    {
        Init(*this, rangeType, numDescriptors, baseShaderRegister, registerSpace, flags, offsetInDescriptorsFromTableStart);
    }
    
    static inline void Init(
        _Out_ D3D12_DESCRIPTOR_RANGE1 &range,
        D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
        UINT numDescriptors,
        UINT baseShaderRegister,
        UINT registerSpace0,
        D3D12_DESCRIPTOR_RANGE_FLAGS flagsD3D12_DESCRIPTOR_RANGE_FLAG_NONE,
        UINT offsetInDescriptorsFromTableStart =
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
    {
        range.RangeTyperangeType;
        range.NumDescriptorsnumDescriptors;
        range.BaseShaderRegisterbaseShaderRegister;
        range.RegisterSpaceregisterSpace;
        range.Flagsflags;
        range.OffsetInDescriptorsFromTableStartoffsetInDescriptorsFromTableStart;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_ROOT_DESCRIPTOR_TABLE1 : public D3D12_ROOT_DESCRIPTOR_TABLE1
{
    CD3DX12_ROOT_DESCRIPTOR_TABLE1()default;
    explicit CD3DX12_ROOT_DESCRIPTOR_TABLE1(const D3D12_ROOT_DESCRIPTOR_TABLE1 &o) :
        D3D12_ROOT_DESCRIPTOR_TABLE1(o)
    {}
    CD3DX12_ROOT_DESCRIPTOR_TABLE1(
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
        _Out_ D3D12_ROOT_DESCRIPTOR_TABLE1 &rootDescriptorTable,
        UINT numDescriptorRanges,
        _In_reads_opt_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE1* _pDescriptorRanges)
    {
        rootDescriptorTable.NumDescriptorRangesnumDescriptorRanges;
        rootDescriptorTable.pDescriptorRanges_pDescriptorRanges;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_ROOT_DESCRIPTOR1 : public D3D12_ROOT_DESCRIPTOR1
{
    CD3DX12_ROOT_DESCRIPTOR1()default;
    explicit CD3DX12_ROOT_DESCRIPTOR1(const D3D12_ROOT_DESCRIPTOR1 &o) :
        D3D12_ROOT_DESCRIPTOR1(o)
    {}
    CD3DX12_ROOT_DESCRIPTOR1(
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_ROOT_DESCRIPTOR_FLAGS flagsD3D12_ROOT_DESCRIPTOR_FLAG_NONE)
    {
        Init(shaderRegister, registerSpace, flags);
    }
    
    inline void Init(
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_ROOT_DESCRIPTOR_FLAGS flagsD3D12_ROOT_DESCRIPTOR_FLAG_NONE)
    {
        Init(*this, shaderRegister, registerSpace, flags);
    }
    
    static inline void Init(
        _Out_ D3D12_ROOT_DESCRIPTOR1 &table, 
        UINT shaderRegister, 
        UINT registerSpace0, 
        D3D12_ROOT_DESCRIPTOR_FLAGS flagsD3D12_ROOT_DESCRIPTOR_FLAG_NONE)
    {
        table.ShaderRegistershaderRegister;
        table.RegisterSpaceregisterSpace;
        table.Flagsflags;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_ROOT_PARAMETER1 : public D3D12_ROOT_PARAMETER1
{
    CD3DX12_ROOT_PARAMETER1()default;
    explicit CD3DX12_ROOT_PARAMETER1(const D3D12_ROOT_PARAMETER1 &o) :
        D3D12_ROOT_PARAMETER1(o)
    {}
    
    static inline void InitAsDescriptorTable(
        _Out_ D3D12_ROOT_PARAMETER1 &rootParam,
        UINT numDescriptorRanges,
        _In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE1* pDescriptorRanges,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        rootParam.ParameterTypeD3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.ShaderVisibilityvisibility;
        CD3DX12_ROOT_DESCRIPTOR_TABLE1::Init(rootParam.DescriptorTable, numDescriptorRanges, pDescriptorRanges);
    }

    static inline void InitAsConstants(
        _Out_ D3D12_ROOT_PARAMETER1 &rootParam,
        UINT num32BitValues,
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        rootParam.ParameterTypeD3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
        rootParam.ShaderVisibilityvisibility;
        CD3DX12_ROOT_CONSTANTS::Init(rootParam.Constants, num32BitValues, shaderRegister, registerSpace);
    }

    static inline void InitAsConstantBufferView(
        _Out_ D3D12_ROOT_PARAMETER1 &rootParam,
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_ROOT_DESCRIPTOR_FLAGS flagsD3D12_ROOT_DESCRIPTOR_FLAG_NONE,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        rootParam.ParameterTypeD3D12_ROOT_PARAMETER_TYPE_CBV;
        rootParam.ShaderVisibilityvisibility;
        CD3DX12_ROOT_DESCRIPTOR1::Init(rootParam.Descriptor, shaderRegister, registerSpace, flags);
    }

    static inline void InitAsShaderResourceView(
        _Out_ D3D12_ROOT_PARAMETER1 &rootParam,
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_ROOT_DESCRIPTOR_FLAGS flagsD3D12_ROOT_DESCRIPTOR_FLAG_NONE,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        rootParam.ParameterTypeD3D12_ROOT_PARAMETER_TYPE_SRV;
        rootParam.ShaderVisibilityvisibility;
        CD3DX12_ROOT_DESCRIPTOR1::Init(rootParam.Descriptor, shaderRegister, registerSpace, flags);
    }

    static inline void InitAsUnorderedAccessView(
        _Out_ D3D12_ROOT_PARAMETER1 &rootParam,
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_ROOT_DESCRIPTOR_FLAGS flagsD3D12_ROOT_DESCRIPTOR_FLAG_NONE,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        rootParam.ParameterTypeD3D12_ROOT_PARAMETER_TYPE_UAV;
        rootParam.ShaderVisibilityvisibility;
        CD3DX12_ROOT_DESCRIPTOR1::Init(rootParam.Descriptor, shaderRegister, registerSpace, flags);
    }
    
    inline void InitAsDescriptorTable(
        UINT numDescriptorRanges,
        _In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE1* pDescriptorRanges,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        InitAsDescriptorTable(*this, numDescriptorRanges, pDescriptorRanges, visibility);
    }
    
    inline void InitAsConstants(
        UINT num32BitValues,
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        InitAsConstants(*this, num32BitValues, shaderRegister, registerSpace, visibility);
    }

    inline void InitAsConstantBufferView(
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_ROOT_DESCRIPTOR_FLAGS flagsD3D12_ROOT_DESCRIPTOR_FLAG_NONE,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        InitAsConstantBufferView(*this, shaderRegister, registerSpace, flags, visibility);
    }

    inline void InitAsShaderResourceView(
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_ROOT_DESCRIPTOR_FLAGS flagsD3D12_ROOT_DESCRIPTOR_FLAG_NONE,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        InitAsShaderResourceView(*this, shaderRegister, registerSpace, flags, visibility);
    }

    inline void InitAsUnorderedAccessView(
        UINT shaderRegister,
        UINT registerSpace0,
        D3D12_ROOT_DESCRIPTOR_FLAGS flagsD3D12_ROOT_DESCRIPTOR_FLAG_NONE,
        D3D12_SHADER_VISIBILITY visibilityD3D12_SHADER_VISIBILITY_ALL)
    {
        InitAsUnorderedAccessView(*this, shaderRegister, registerSpace, flags, visibility);
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC : public D3D12_VERSIONED_ROOT_SIGNATURE_DESC
{
    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC()default;
    explicit CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC &o) :
        D3D12_VERSIONED_ROOT_SIGNATURE_DESC(o)
    {}
    explicit CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC(const D3D12_ROOT_SIGNATURE_DESC &o)
    {
        VersionD3D_ROOT_SIGNATURE_VERSION_1_0;
        Desc_1_0o;
    }
    explicit CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC(const D3D12_ROOT_SIGNATURE_DESC1 &o)
    {
        VersionD3D_ROOT_SIGNATURE_VERSION_1_1;
        Desc_1_1o;
    }
    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC(
        UINT numParameters,
        _In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
        UINT numStaticSamplers0,
        _In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplersnullptr,
        D3D12_ROOT_SIGNATURE_FLAGS flagsD3D12_ROOT_SIGNATURE_FLAG_NONE)
    {
        Init_1_0(numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
    }
    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC(
        UINT numParameters,
        _In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER1* _pParameters,
        UINT numStaticSamplers0,
        _In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplersnullptr,
        D3D12_ROOT_SIGNATURE_FLAGS flagsD3D12_ROOT_SIGNATURE_FLAG_NONE)
    {
        Init_1_1(numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
    }
    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC(CD3DX12_DEFAULT)
    {
        Init_1_1(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_NONE);
    }
    
    inline void Init_1_0(
        UINT numParameters,
        _In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
        UINT numStaticSamplers0,
        _In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplersnullptr,
        D3D12_ROOT_SIGNATURE_FLAGS flagsD3D12_ROOT_SIGNATURE_FLAG_NONE)
    {
        Init_1_0(*this, numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
    }

    static inline void Init_1_0(
        _Out_ D3D12_VERSIONED_ROOT_SIGNATURE_DESC &desc,
        UINT numParameters,
        _In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* _pParameters,
        UINT numStaticSamplers0,
        _In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplersnullptr,
        D3D12_ROOT_SIGNATURE_FLAGS flagsD3D12_ROOT_SIGNATURE_FLAG_NONE)
    {
        desc.VersionD3D_ROOT_SIGNATURE_VERSION_1_0;
        desc.Desc_1_0.NumParametersnumParameters;
        desc.Desc_1_0.pParameters_pParameters;
        desc.Desc_1_0.NumStaticSamplersnumStaticSamplers;
        desc.Desc_1_0.pStaticSamplers_pStaticSamplers;
        desc.Desc_1_0.Flagsflags;
    }

    inline void Init_1_1(
        UINT numParameters,
        _In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER1* _pParameters,
        UINT numStaticSamplers0,
        _In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplersnullptr,
        D3D12_ROOT_SIGNATURE_FLAGS flagsD3D12_ROOT_SIGNATURE_FLAG_NONE)
    {
        Init_1_1(*this, numParameters, _pParameters, numStaticSamplers, _pStaticSamplers, flags);
    }

    static inline void Init_1_1(
        _Out_ D3D12_VERSIONED_ROOT_SIGNATURE_DESC &desc,
        UINT numParameters,
        _In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER1* _pParameters,
        UINT numStaticSamplers0,
        _In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplersnullptr,
        D3D12_ROOT_SIGNATURE_FLAGS flagsD3D12_ROOT_SIGNATURE_FLAG_NONE)
    {
        desc.VersionD3D_ROOT_SIGNATURE_VERSION_1_1;
        desc.Desc_1_1.NumParametersnumParameters;
        desc.Desc_1_1.pParameters_pParameters;
        desc.Desc_1_1.NumStaticSamplersnumStaticSamplers;
        desc.Desc_1_1.pStaticSamplers_pStaticSamplers;
        desc.Desc_1_1.Flagsflags;
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_CPU_DESCRIPTOR_HANDLE : public D3D12_CPU_DESCRIPTOR_HANDLE
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE()default;
    explicit CD3DX12_CPU_DESCRIPTOR_HANDLE(const D3D12_CPU_DESCRIPTOR_HANDLE &o) :
        D3D12_CPU_DESCRIPTOR_HANDLE(o)
    {}
    CD3DX12_CPU_DESCRIPTOR_HANDLE(CD3DX12_DEFAULT) { ptr0; }
    CD3DX12_CPU_DESCRIPTOR_HANDLE(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE &other, INT offsetScaledByIncrementSize)
    {
        InitOffsetted(other, offsetScaledByIncrementSize);
    }
    CD3DX12_CPU_DESCRIPTOR_HANDLE(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE &other, INT offsetInDescriptors, UINT descriptorIncrementSize)
    {
        InitOffsetted(other, offsetInDescriptors, descriptorIncrementSize);
    }
    CD3DX12_CPU_DESCRIPTOR_HANDLE& Offset(INT offsetInDescriptors, UINT descriptorIncrementSize)
    { 
        ptrSIZE_T(INT64(ptr) + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize));
        return *this;
    }
    CD3DX12_CPU_DESCRIPTOR_HANDLE& Offset(INT offsetScaledByIncrementSize) 
    { 
        ptrSIZE_T(INT64(ptr) + INT64(offsetScaledByIncrementSize));
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
    CD3DX12_CPU_DESCRIPTOR_HANDLE &operator=(const D3D12_CPU_DESCRIPTOR_HANDLE &other)
    {
        ptrother.ptr;
        return *this;
    }

    inline void InitOffsetted(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE &base, INT offsetScaledByIncrementSize)
    {
        InitOffsetted(*this, base, offsetScaledByIncrementSize);
    }
    
    inline void InitOffsetted(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE &base, INT offsetInDescriptors, UINT descriptorIncrementSize)
    {
        InitOffsetted(*this, base, offsetInDescriptors, descriptorIncrementSize);
    }
    
    static inline void InitOffsetted(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE &handle, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE &base, INT offsetScaledByIncrementSize)
    {
        handle.ptrSIZE_T(INT64(base.ptr) + INT64(offsetScaledByIncrementSize));
    }
    
    static inline void InitOffsetted(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE &handle, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE &base, INT offsetInDescriptors, UINT descriptorIncrementSize)
    {
        handle.ptrSIZE_T(INT64(base.ptr) + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize));
    }
};

//------------------------------------------------------------------------------------------------
struct CD3DX12_GPU_DESCRIPTOR_HANDLE : public D3D12_GPU_DESCRIPTOR_HANDLE
{
    CD3DX12_GPU_DESCRIPTOR_HANDLE()default;
    explicit CD3DX12_GPU_DESCRIPTOR_HANDLE(const D3D12_GPU_DESCRIPTOR_HANDLE &o) :
        D3D12_GPU_DESCRIPTOR_HANDLE(o)
    {}
    CD3DX12_GPU_DESCRIPTOR_HANDLE(CD3DX12_DEFAULT) { ptr0; }
    CD3DX12_GPU_DESCRIPTOR_HANDLE(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE &other, INT offsetScaledByIncrementSize)
    {
        InitOffsetted(other, offsetScaledByIncrementSize);
    }
    CD3DX12_GPU_DESCRIPTOR_HANDLE(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE &other, INT offsetInDescriptors, UINT descriptorIncrementSize)
    {
        InitOffsetted(other, offsetInDescriptors, descriptorIncrementSize);
    }
    CD3DX12_GPU_DESCRIPTOR_HANDLE& Offset(INT offsetInDescriptors, UINT descriptorIncrementSize)
    { 
        ptrUINT64(INT64(ptr) + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize));
        return *this;
    }
    CD3DX12_GPU_DESCRIPTOR_HANDLE& Offset(INT offsetScaledByIncrementSize) 
    { 
        ptrUINT64(INT64(ptr) + INT64(offsetScaledByIncrementSize));
        return *this;
    }
    inline bool operator==(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE& other) const
    {
        return (ptr == other.ptr);
    }
    inline bool operator!=(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE& other) const
    {
        return (ptr != other.ptr);
    }
    CD3DX12_GPU_DESCRIPTOR_HANDLE &operator=(const D3D12_GPU_DESCRIPTOR_HANDLE &other)
    {
        ptrother.ptr;
        return *this;
    }

    inline void InitOffsetted(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE &base, INT offsetScaledByIncrementSize)
    {
        InitOffsetted(*this, base, offsetScaledByIncrementSize);
    }
    
    inline void InitOffsetted(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE &base, INT offsetInDescriptors, UINT descriptorIncrementSize)
    {
        InitOffsetted(*this, base, offsetInDescriptors, descriptorIncrementSize);
    }
    
    static inline void InitOffsetted(_Out_ D3D12_GPU_DESCRIPTOR_HANDLE &handle, _In_ const D3D12_GPU_DESCRIPTOR_HANDLE &base, INT offsetScaledByIncrementSize)
    {
        handle.ptrUINT64(INT64(base.ptr) + INT64(offsetScaledByIncrementSize));
    }
    
    static inline void InitOffsetted(_Out_ D3D12_GPU_DESCRIPTOR_HANDLE &handle, _In_ const D3D12_GPU_DESCRIPTOR_HANDLE &base, INT offsetInDescriptors, UINT descriptorIncrementSize)
    {
        handle.ptrUINT64(INT64(base.ptr) + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize));
    }
};

//------------------------------------------------------------------------------------------------
inline UINT D3D12CalcSubresource( UINT MipSlice, UINT ArraySlice, UINT PlaneSlice, UINT MipLevels, UINT ArraySize )
{ 
    return MipSlice + ArraySlice * MipLevels + PlaneSlice * MipLevels * ArraySize; 
}

//------------------------------------------------------------------------------------------------
template <typename T, typename U, typename V>
inline void D3D12DecomposeSubresource( UINT Subresource, UINT MipLevels, UINT ArraySize, _Out_ T& MipSlice, _Out_ U& ArraySlice, _Out_ V& PlaneSlice )
{
    MipSlicestatic_cast<T>(Subresource % MipLevels);
    ArraySlicestatic_cast<U>((Subresource / MipLevels) % ArraySize);
    PlaneSlicestatic_cast<V>(Subresource / (MipLevels * ArraySize));
}

//------------------------------------------------------------------------------------------------
inline UINT8 D3D12GetFormatPlaneCount(
    _In_ ID3D12Device* pDevice,
    DXGI_FORMAT Format
    )
{
    D3D12_FEATURE_DATA_FORMAT_INFO formatInfo{ Format, 0 };
    if (FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_FORMAT_INFO, &formatInfo, sizeof(formatInfo))))
    {
        return 0;
    }
    return formatInfo.PlaneCount;
}

//------------------------------------------------------------------------------------------------
struct CD3DX12_RESOURCE_DESC : public D3D12_RESOURCE_DESC
{
    CD3DX12_RESOURCE_DESC()default;
    explicit CD3DX12_RESOURCE_DESC( const D3D12_RESOURCE_DESC& o ) :
        D3D12_RESOURCE_DESC( o )
    {}
    CD3DX12_RESOURCE_DESC( 
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
        D3D12_RESOURCE_FLAGS flags )
    {
        Dimensiondimension;
        Alignmentalignment;
        Widthwidth;
        Heightheight;
        DepthOrArraySizedepthOrArraySize;
        MipLevelsmipLevels;
        Formatformat;
        SampleDesc.CountsampleCount;
        SampleDesc.QualitysampleQuality;
        Layoutlayout;
        Flagsflags;
    }
    static inline CD3DX12_RESOURCE_DESC Buffer( 
        const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
        D3D12_RESOURCE_FLAGS flagsD3D12_RESOURCE_FLAG_NONE )
    {
        return CD3DX12_RESOURCE_DESC( D3D12_RESOURCE_DIMENSION_BUFFER, resAllocInfo.Alignment, resAllocInfo.SizeInBytes, 
            1, 1, 1, DXGI_FORMAT_UNKNOWN, 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags );
    }
    static inline CD3DX12_RESOURCE_DESC Buffer( 
        UINT64 width,
        D3D12_RESOURCE_FLAGS flagsD3D12_RESOURCE_FLAG_NONE,
        UINT64 alignment0 )
    {
        return CD3DX12_RESOURCE_DESC( D3D12_RESOURCE_DIMENSION_BUFFER, alignment, width, 1, 1, 1, 
            DXGI_FORMAT_UNKNOWN, 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags );
    }
    static inline CD3DX12_RESOURCE_DESC Tex1D( 
        DXGI_FORMAT format,
        UINT64 width,
        UINT16 arraySize,
        UINT16 mipLevels0,
        D3D12_RESOURCE_FLAGS flagsD3D12_RESOURCE_FLAG_NONE,
        D3D12_TEXTURE_LAYOUT layoutD3D12_TEXTURE_LAYOUT_UNKNOWN,
        UINT64 alignment0 )
    {
        return CD3DX12_RESOURCE_DESC( D3D12_RESOURCE_DIMENSION_TEXTURE1D, alignment, width, 1, arraySize, 
            mipLevels, format, 1, 0, layout, flags );
    }
    static inline CD3DX12_RESOURCE_DESC Tex2D( 
        DXGI_FORMAT format,
        UINT64 width,
        UINT height,
        UINT16 arraySize,
        UINT16 mipLevels0,
        UINT sampleCount,
        UINT sampleQuality0,
        D3D12_RESOURCE_FLAGS flagsD3D12_RESOURCE_FLAG_NONE,
        D3D12_TEXTURE_LAYOUT layoutD3D12_TEXTURE_LAYOUT_UNKNOWN,
        UINT64 alignment0 )
    {
        return CD3DX12_RESOURCE_DESC( D3D12_RESOURCE_DIMENSION_TEXTURE2D, alignment, width, height, arraySize, 
            mipLevels, format, sampleCount, sampleQuality, layout, flags );
    }
    static inline CD3DX12_RESOURCE_DESC Tex3D( 
        DXGI_FORMAT format,
        UINT64 width,
        UINT height,
        UINT16 depth,
        UINT16 mipLevels0,
        D3D12_RESOURCE_FLAGS flagsD3D12_RESOURCE_FLAG_NONE,
        D3D12_TEXTURE_LAYOUT layoutD3D12_TEXTURE_LAYOUT_UNKNOWN,
        UINT64 alignment0 )
    {
        return CD3DX12_RESOURCE_DESC( D3D12_RESOURCE_DIMENSION_TEXTURE3D, alignment, width, height, depth, 
            mipLevels, format, 1, 0, layout, flags );
    }
    inline UINT16 Depth() const
    { return (Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D ? DepthOrArraySize : 1); }
    inline UINT16 ArraySize() const
    { return (Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE3D ? DepthOrArraySize : 1); }
    inline UINT8 PlaneCount(_In_ ID3D12Device* pDevice) const
    { return D3D12GetFormatPlaneCount(pDevice, Format); }
    inline UINT Subresources(_In_ ID3D12Device* pDevice) const
    { return MipLevels * ArraySize() * PlaneCount(pDevice); }
    inline UINT CalcSubresource(UINT MipSlice, UINT ArraySlice, UINT PlaneSlice)
    { return D3D12CalcSubresource(MipSlice, ArraySlice, PlaneSlice, MipLevels, ArraySize()); }
};
inline bool operator==( const D3D12_RESOURCE_DESC& l, const D3D12_RESOURCE_DESC& r )
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
inline bool operator!=( const D3D12_RESOURCE_DESC& l, const D3D12_RESOURCE_DESC& r )
{ return !( l == r ); }

//------------------------------------------------------------------------------------------------
struct CD3DX12_VIEW_INSTANCING_DESC : public D3D12_VIEW_INSTANCING_DESC
{
    CD3DX12_VIEW_INSTANCING_DESC()default;
    explicit CD3DX12_VIEW_INSTANCING_DESC( const D3D12_VIEW_INSTANCING_DESC& o ) :
        D3D12_VIEW_INSTANCING_DESC( o )
    {}
    explicit CD3DX12_VIEW_INSTANCING_DESC( CD3DX12_DEFAULT )
    {
        ViewInstanceCount0;
        pViewInstanceLocationsnullptr;
        FlagsD3D12_VIEW_INSTANCING_FLAG_NONE;
    }
    explicit CD3DX12_VIEW_INSTANCING_DESC( 
        UINT InViewInstanceCount,
        const D3D12_VIEW_INSTANCE_LOCATION* InViewInstanceLocations,
        D3D12_VIEW_INSTANCING_FLAGS InFlags)
    {
        ViewInstanceCountInViewInstanceCount;
        pViewInstanceLocationsInViewInstanceLocations;
        FlagsInFlags;
    }
};

//------------------------------------------------------------------------------------------------
// Row-by-row memcpy
inline void MemcpySubresource(
    _In_ const D3D12_MEMCPY_DEST* pDest,
    _In_ const D3D12_SUBRESOURCE_DATA* pSrc,
    SIZE_T RowSizeInBytes,
    UINT NumRows,
    UINT NumSlices)
{
    for (UINT z0; z < NumSlices; ++z)
    {
        auto pDestSlicereinterpret_cast<BYTE*>(pDest->pData) + pDest->SlicePitch * z;
        auto pSrcSlicereinterpret_cast<const BYTE*>(pSrc->pData) + pSrc->SlicePitch * LONG_PTR(z);
        for (UINT y0; y < NumRows; ++y)
        {
            memcpy(pDestSlice + pDest->RowPitch * y,
                   pSrcSlice + pSrc->RowPitch * LONG_PTR(y),
                   RowSizeInBytes);
        }
    }
}

//------------------------------------------------------------------------------------------------
// Returns required size of a buffer to be used for data upload
inline UINT64 GetRequiredIntermediateSize(
    _In_ ID3D12Resource* pDestinationResource,
    _In_range_(0,D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
    _In_range_(0,D3D12_REQ_SUBRESOURCES-FirstSubresource) UINT NumSubresources)
{
    auto DescpDestinationResource->GetDesc();
    UINT64 RequiredSize0;
    
    ID3D12Device* pDevicenullptr;
    pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
    pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, 0, nullptr, nullptr, nullptr, &RequiredSize);
    pDevice->Release();
    
    return RequiredSize;
}

//------------------------------------------------------------------------------------------------
// All arrays must be populated (e.g. by calling GetCopyableFootprints)
inline UINT64 UpdateSubresources(
    _In_ ID3D12GraphicsCommandList* pCmdList,
    _In_ ID3D12Resource* pDestinationResource,
    _In_ ID3D12Resource* pIntermediate,
    _In_range_(0,D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
    _In_range_(0,D3D12_REQ_SUBRESOURCES-FirstSubresource) UINT NumSubresources,
    UINT64 RequiredSize,
    _In_reads_(NumSubresources) const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts,
    _In_reads_(NumSubresources) const UINT* pNumRows,
    _In_reads_(NumSubresources) const UINT64* pRowSizesInBytes,
    _In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData)
{
    // Minor validation
    auto IntermediateDescpIntermediate->GetDesc();
    auto DestinationDescpDestinationResource->GetDesc();
    if (IntermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER || 
        IntermediateDesc.Width < RequiredSize + pLayouts[0].Offset || 
        RequiredSize > SIZE_T(-1) || 
        (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER && 
            (FirstSubresource != 0 || NumSubresources != 1)))
    {
        return 0;
    }
    
    BYTE* pData;
    HRESULT hrpIntermediate->Map(0, nullptr, reinterpret_cast<void**>(&pData));
    if (FAILED(hr))
    {
        return 0;
    }
    
    for (UINT i0; i < NumSubresources; ++i)
    {
        if (pRowSizesInBytes[i] > SIZE_T(-1)) return 0;
        D3D12_MEMCPY_DEST DestData{ pData + pLayouts[i].Offset, pLayouts[i].Footprint.RowPitch, SIZE_T(pLayouts[i].Footprint.RowPitch) * SIZE_T(pNumRows[i]) };
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
        for (UINT i0; i < NumSubresources; ++i)
        {
            CD3DX12_TEXTURE_COPY_LOCATION Dst(pDestinationResource, i + FirstSubresource);
            CD3DX12_TEXTURE_COPY_LOCATION Src(pIntermediate, pLayouts[i]);
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
    _In_range_(0,D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
    _In_range_(0,D3D12_REQ_SUBRESOURCES-FirstSubresource) UINT NumSubresources,
    _In_reads_(NumSubresources) D3D12_SUBRESOURCE_DATA* pSrcData)
{
    UINT64 RequiredSize0;
    UINT64 MemToAllocstatic_cast<UINT64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64)) * NumSubresources;
    if (MemToAlloc > SIZE_MAX)
    {
       return 0;
    }
    void* pMemHeapAlloc(GetProcessHeap(), 0, static_cast<SIZE_T>(MemToAlloc));
    if (pMem == nullptr)
    {
       return 0;
    }
    auto pLayoutsreinterpret_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(pMem);
    UINT64* pRowSizesInBytesreinterpret_cast<UINT64*>(pLayouts + NumSubresources);
    UINT* pNumRowsreinterpret_cast<UINT*>(pRowSizesInBytes + NumSubresources);
    
    auto DescpDestinationResource->GetDesc();
    ID3D12Device* pDevicenullptr;
    pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
    pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, pLayouts, pNumRows, pRowSizesInBytes, &RequiredSize);
    pDevice->Release();
    
    UINT64 ResultUpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, pLayouts, pNumRows, pRowSizesInBytes, pSrcData);
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
    UINT64 RequiredSize0;
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT Layouts[MaxSubresources];
    UINT NumRows[MaxSubresources];
    UINT64 RowSizesInBytes[MaxSubresources];
    
    auto DescpDestinationResource->GetDesc();
    ID3D12Device* pDevicenullptr;
    pDestinationResource->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice));
    pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, Layouts, NumRows, RowSizesInBytes, &RequiredSize);
    pDevice->Release();
    
    return UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, Layouts, NumRows, RowSizesInBytes, pSrcData);
}

//------------------------------------------------------------------------------------------------
inline bool D3D12IsLayoutOpaque( D3D12_TEXTURE_LAYOUT Layout )
{ return Layout == D3D12_TEXTURE_LAYOUT_UNKNOWN || Layout == D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE; }

//------------------------------------------------------------------------------------------------
template <typename t_CommandListType>
inline ID3D12CommandList * const * CommandListCast(t_CommandListType * const * pp)
{
    // This cast is useful for passing strongly typed command list pointers into
    // ExecuteCommandLists.
    // This cast is valid as long as the const-ness is respected. D3D12 APIs do
    // respect the const-ness of their arguments.
    return reinterpret_cast<ID3D12CommandList * const *>(pp);
}

//------------------------------------------------------------------------------------------------
// D3D12 exports a new method for serializing root signatures in the Windows 10 Anniversary Update.
// To help enable root signature 1.1 features when they are available and not require maintaining
// two code paths for building root signatures, this helper method reconstructs a 1.0 signature when
// 1.1 is not supported.
inline HRESULT D3DX12SerializeVersionedRootSignature(
    _In_ const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pRootSignatureDesc,
    D3D_ROOT_SIGNATURE_VERSION MaxVersion,
    _Outptr_ ID3DBlob** ppBlob,
    _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob)
{
    if (ppErrorBlob != nullptr)
    {
        *ppErrorBlobnullptr;
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
                    HRESULT hrS_OK;
                    const D3D12_ROOT_SIGNATURE_DESC1& desc_1_1pRootSignatureDesc->Desc_1_1;

                    const SIZE_T ParametersSizesizeof(D3D12_ROOT_PARAMETER) * desc_1_1.NumParameters;
                    void* pParameters(ParametersSize > 0) ? HeapAlloc(GetProcessHeap(), 0, ParametersSize) : nullptr;
                    if (ParametersSize > 0 && pParameters == nullptr)
                    {
                        hrE_OUTOFMEMORY;
                    }
                    auto pParameters_1_0reinterpret_cast<D3D12_ROOT_PARAMETER*>(pParameters);

                    if (SUCCEEDED(hr))
                    {
                        for (UINT n0; n < desc_1_1.NumParameters; n++)
                        {
                            __analysis_assume(ParametersSize == sizeof(D3D12_ROOT_PARAMETER) * desc_1_1.NumParameters);
                            pParameters_1_0[n].ParameterTypedesc_1_1.pParameters[n].ParameterType;
                            pParameters_1_0[n].ShaderVisibilitydesc_1_1.pParameters[n].ShaderVisibility;

                            switch (desc_1_1.pParameters[n].ParameterType)
                            {
                            case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
                                pParameters_1_0[n].Constants.Num32BitValuesdesc_1_1.pParameters[n].Constants.Num32BitValues;
                                pParameters_1_0[n].Constants.RegisterSpacedesc_1_1.pParameters[n].Constants.RegisterSpace;
                                pParameters_1_0[n].Constants.ShaderRegisterdesc_1_1.pParameters[n].Constants.ShaderRegister;
                                break;

                            case D3D12_ROOT_PARAMETER_TYPE_CBV:
                            case D3D12_ROOT_PARAMETER_TYPE_SRV:
                            case D3D12_ROOT_PARAMETER_TYPE_UAV:
                                pParameters_1_0[n].Descriptor.RegisterSpacedesc_1_1.pParameters[n].Descriptor.RegisterSpace;
                                pParameters_1_0[n].Descriptor.ShaderRegisterdesc_1_1.pParameters[n].Descriptor.ShaderRegister;
                                break;

                            case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
                                const D3D12_ROOT_DESCRIPTOR_TABLE1& table_1_1desc_1_1.pParameters[n].DescriptorTable;

                                const SIZE_T DescriptorRangesSizesizeof(D3D12_DESCRIPTOR_RANGE) * table_1_1.NumDescriptorRanges;
                                void* pDescriptorRanges(DescriptorRangesSize > 0 && SUCCEEDED(hr)) ? HeapAlloc(GetProcessHeap(), 0, DescriptorRangesSize) : nullptr;
                                if (DescriptorRangesSize > 0 && pDescriptorRanges == nullptr)
                                {
                                    hrE_OUTOFMEMORY;
                                }
                                auto pDescriptorRanges_1_0reinterpret_cast<D3D12_DESCRIPTOR_RANGE*>(pDescriptorRanges);

                                if (SUCCEEDED(hr))
                                {
                                    for (UINT x0; x < table_1_1.NumDescriptorRanges; x++)
                                    {
                                        __analysis_assume(DescriptorRangesSize == sizeof(D3D12_DESCRIPTOR_RANGE) * table_1_1.NumDescriptorRanges);
                                        pDescriptorRanges_1_0[x].BaseShaderRegistertable_1_1.pDescriptorRanges[x].BaseShaderRegister;
                                        pDescriptorRanges_1_0[x].NumDescriptorstable_1_1.pDescriptorRanges[x].NumDescriptors;
                                        pDescriptorRanges_1_0[x].OffsetInDescriptorsFromTableStarttable_1_1.pDescriptorRanges[x].OffsetInDescriptorsFromTableStart;
                                        pDescriptorRanges_1_0[x].RangeTypetable_1_1.pDescriptorRanges[x].RangeType;
                                        pDescriptorRanges_1_0[x].RegisterSpacetable_1_1.pDescriptorRanges[x].RegisterSpace;
                                    }
                                }

                                D3D12_ROOT_DESCRIPTOR_TABLE& table_1_0pParameters_1_0[n].DescriptorTable;
                                table_1_0.NumDescriptorRangestable_1_1.NumDescriptorRanges;
                                table_1_0.pDescriptorRangespDescriptorRanges_1_0;
                            }
                        }
                    }

                    if (SUCCEEDED(hr))
                    {
                        CD3DX12_ROOT_SIGNATURE_DESC desc_1_0(desc_1_1.NumParameters, pParameters_1_0, desc_1_1.NumStaticSamplers, desc_1_1.pStaticSamplers, desc_1_1.Flags);
                        hrD3D12SerializeRootSignature(&desc_1_0, D3D_ROOT_SIGNATURE_VERSION_1, ppBlob, ppErrorBlob);
                    }

                    if (pParameters)
                    {
                        for (UINT n0; n < desc_1_1.NumParameters; n++)
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

//------------------------------------------------------------------------------------------------
struct CD3DX12_RT_FORMAT_ARRAY : public D3D12_RT_FORMAT_ARRAY
{
    CD3DX12_RT_FORMAT_ARRAY()default;
    explicit CD3DX12_RT_FORMAT_ARRAY(const D3D12_RT_FORMAT_ARRAY& o)
        : D3D12_RT_FORMAT_ARRAY(o)
    {}
    explicit CD3DX12_RT_FORMAT_ARRAY(_In_reads_(NumFormats) const DXGI_FORMAT* pFormats, UINT NumFormats)
    {
        NumRenderTargetsNumFormats;
        memcpy(RTFormats, pFormats, sizeof(RTFormats));
        // assumes ARRAY_SIZE(pFormats) == ARRAY_SIZE(RTFormats)
    }
};

//------------------------------------------------------------------------------------------------
// Pipeline State Stream Helpers
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
// Stream Subobjects, i.e. elements of a stream

struct DefaultSampleMask { operator UINT() { return UINT_MAX; } };
struct DefaultSampleDesc { operator DXGI_SAMPLE_DESC() { return DXGI_SAMPLE_DESC{1, 0}; } };

#pragma warning(push)
#pragma warning(disable : 4324)
template <typename InnerStructType, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE Type, typename DefaultArgInnerStructType>
class alignas(void*) CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT
{
private:
    D3D12_PIPELINE_STATE_SUBOBJECT_TYPE _Type;
    InnerStructType _Inner;
public:
    CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT() noexcept : _Type(Type), _Inner(DefaultArg()) {}
    CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT(InnerStructType const& i) : _Type(Type), _Inner(i) {}
    CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT& operator=(InnerStructType const& i) { _Inneri; return *this; }
    operator InnerStructType const&() const { return _Inner; }
    operator InnerStructType&() { return _Inner; }
    InnerStructType* operator&() { return &_Inner; }
    InnerStructType const* operator&() const { return &_Inner; }
};
#pragma warning(pop)
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_PIPELINE_STATE_FLAGS,         D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_FLAGS>                             CD3DX12_PIPELINE_STATE_STREAM_FLAGS;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< UINT,                               D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_NODE_MASK>                         CD3DX12_PIPELINE_STATE_STREAM_NODE_MASK;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< ID3D12RootSignature*,               D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_ROOT_SIGNATURE>                    CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_INPUT_LAYOUT_DESC,            D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_INPUT_LAYOUT>                      CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_INDEX_BUFFER_STRIP_CUT_VALUE, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_IB_STRIP_CUT_VALUE>                CD3DX12_PIPELINE_STATE_STREAM_IB_STRIP_CUT_VALUE;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_PRIMITIVE_TOPOLOGY_TYPE,      D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PRIMITIVE_TOPOLOGY>                CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_SHADER_BYTECODE,              D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VS>                                CD3DX12_PIPELINE_STATE_STREAM_VS;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_SHADER_BYTECODE,              D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_GS>                                CD3DX12_PIPELINE_STATE_STREAM_GS;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_STREAM_OUTPUT_DESC,           D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_STREAM_OUTPUT>                     CD3DX12_PIPELINE_STATE_STREAM_STREAM_OUTPUT;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_SHADER_BYTECODE,              D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_HS>                                CD3DX12_PIPELINE_STATE_STREAM_HS;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_SHADER_BYTECODE,              D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DS>                                CD3DX12_PIPELINE_STATE_STREAM_DS;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_SHADER_BYTECODE,              D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PS>                                CD3DX12_PIPELINE_STATE_STREAM_PS;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_SHADER_BYTECODE,              D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CS>                                CD3DX12_PIPELINE_STATE_STREAM_CS;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< CD3DX12_BLEND_DESC,                 D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_BLEND,          CD3DX12_DEFAULT>   CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< CD3DX12_DEPTH_STENCIL_DESC,         D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL,  CD3DX12_DEFAULT>   CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< CD3DX12_DEPTH_STENCIL_DESC1,        D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL1, CD3DX12_DEFAULT>   CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL1;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< DXGI_FORMAT,                        D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL_FORMAT>              CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< CD3DX12_RASTERIZER_DESC,            D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RASTERIZER,     CD3DX12_DEFAULT>   CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_RT_FORMAT_ARRAY,              D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS>             CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< DXGI_SAMPLE_DESC,                   D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_DESC,    DefaultSampleDesc> CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< UINT,                               D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_MASK,    DefaultSampleMask> CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_MASK;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< D3D12_CACHED_PIPELINE_STATE,        D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CACHED_PSO>                        CD3DX12_PIPELINE_STATE_STREAM_CACHED_PSO;
typedef CD3DX12_PIPELINE_STATE_STREAM_SUBOBJECT< CD3DX12_VIEW_INSTANCING_DESC,       D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VIEW_INSTANCING, CD3DX12_DEFAULT>  CD3DX12_PIPELINE_STATE_STREAM_VIEW_INSTANCING;

//------------------------------------------------------------------------------------------------
// Stream Parser Helpers

struct ID3DX12PipelineParserCallbacks
{
    // Subobject Callbacks
    virtual void FlagsCb(D3D12_PIPELINE_STATE_FLAGS) {}
    virtual void NodeMaskCb(UINT) {}
    virtual void RootSignatureCb(ID3D12RootSignature*) {}
    virtual void InputLayoutCb(const D3D12_INPUT_LAYOUT_DESC&) {}
    virtual void IBStripCutValueCb(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE) {}
    virtual void PrimitiveTopologyTypeCb(D3D12_PRIMITIVE_TOPOLOGY_TYPE) {}
    virtual void VSCb(const D3D12_SHADER_BYTECODE&) {}
    virtual void GSCb(const D3D12_SHADER_BYTECODE&) {}
    virtual void StreamOutputCb(const D3D12_STREAM_OUTPUT_DESC&) {}
    virtual void HSCb(const D3D12_SHADER_BYTECODE&) {}
    virtual void DSCb(const D3D12_SHADER_BYTECODE&) {}
    virtual void PSCb(const D3D12_SHADER_BYTECODE&) {}
    virtual void CSCb(const D3D12_SHADER_BYTECODE&) {}
    virtual void BlendStateCb(const D3D12_BLEND_DESC&) {}
    virtual void DepthStencilStateCb(const D3D12_DEPTH_STENCIL_DESC&) {}
    virtual void DepthStencilState1Cb(const D3D12_DEPTH_STENCIL_DESC1&) {}
    virtual void DSVFormatCb(DXGI_FORMAT) {}
    virtual void RasterizerStateCb(const D3D12_RASTERIZER_DESC&) {}
    virtual void RTVFormatsCb(const D3D12_RT_FORMAT_ARRAY&) {}
    virtual void SampleDescCb(const DXGI_SAMPLE_DESC&) {}
    virtual void SampleMaskCb(UINT) {}
    virtual void ViewInstancingCb(const D3D12_VIEW_INSTANCING_DESC&) {}
    virtual void CachedPSOCb(const D3D12_CACHED_PIPELINE_STATE&) {}

    // Error Callbacks
    virtual void ErrorBadInputParameter(UINT /*ParameterIndex*/) {}
    virtual void ErrorDuplicateSubobject(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE /*DuplicateType*/) {}
    virtual void ErrorUnknownSubobject(UINT /*UnknownTypeValue*/) {}

    virtual ~ID3DX12PipelineParserCallbacks()default;
};

// CD3DX12_PIPELINE_STATE_STREAM1 Works on RS3+ (where there is a new view instancing subobject).  
// Use CD3DX12_PIPELINE_STATE_STREAM for RS2+ support.
struct CD3DX12_PIPELINE_STATE_STREAM1
{
    CD3DX12_PIPELINE_STATE_STREAM1()default;
    CD3DX12_PIPELINE_STATE_STREAM1(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& Desc)
        : Flags(Desc.Flags)
        , NodeMask(Desc.NodeMask)
        , pRootSignature(Desc.pRootSignature)
        , InputLayout(Desc.InputLayout)
        , IBStripCutValue(Desc.IBStripCutValue)
        , PrimitiveTopologyType(Desc.PrimitiveTopologyType)
        , VS(Desc.VS)
        , GS(Desc.GS)
        , StreamOutput(Desc.StreamOutput)
        , HS(Desc.HS)
        , DS(Desc.DS)
        , PS(Desc.PS)
        , BlendState(CD3DX12_BLEND_DESC(Desc.BlendState))
        , DepthStencilState(CD3DX12_DEPTH_STENCIL_DESC1(Desc.DepthStencilState))
        , DSVFormat(Desc.DSVFormat)
        , RasterizerState(CD3DX12_RASTERIZER_DESC(Desc.RasterizerState))
        , RTVFormats(CD3DX12_RT_FORMAT_ARRAY(Desc.RTVFormats, Desc.NumRenderTargets))
        , SampleDesc(Desc.SampleDesc)
        , SampleMask(Desc.SampleMask)
        , CachedPSO(Desc.CachedPSO)
        , ViewInstancingDesc(CD3DX12_VIEW_INSTANCING_DESC(CD3DX12_DEFAULT()))
    {}
    CD3DX12_PIPELINE_STATE_STREAM1(const D3D12_COMPUTE_PIPELINE_STATE_DESC& Desc)
        : Flags(Desc.Flags)
        , NodeMask(Desc.NodeMask)
        , pRootSignature(Desc.pRootSignature)
        , CS(CD3DX12_SHADER_BYTECODE(Desc.CS))
        , CachedPSO(Desc.CachedPSO)
    {
        static_cast<D3D12_DEPTH_STENCIL_DESC1&>(DepthStencilState).DepthEnablefalse;
    }
    CD3DX12_PIPELINE_STATE_STREAM_FLAGS Flags;
    CD3DX12_PIPELINE_STATE_STREAM_NODE_MASK NodeMask;
    CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
    CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
    CD3DX12_PIPELINE_STATE_STREAM_IB_STRIP_CUT_VALUE IBStripCutValue;
    CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
    CD3DX12_PIPELINE_STATE_STREAM_VS VS;
    CD3DX12_PIPELINE_STATE_STREAM_GS GS;
    CD3DX12_PIPELINE_STATE_STREAM_STREAM_OUTPUT StreamOutput;
    CD3DX12_PIPELINE_STATE_STREAM_HS HS;
    CD3DX12_PIPELINE_STATE_STREAM_DS DS;
    CD3DX12_PIPELINE_STATE_STREAM_PS PS;
    CD3DX12_PIPELINE_STATE_STREAM_CS CS;
    CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC BlendState;
    CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL1 DepthStencilState;
    CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
    CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER RasterizerState;
    CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
    CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC SampleDesc;
    CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_MASK SampleMask;
    CD3DX12_PIPELINE_STATE_STREAM_CACHED_PSO CachedPSO;
    CD3DX12_PIPELINE_STATE_STREAM_VIEW_INSTANCING ViewInstancingDesc;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC GraphicsDescV0() const
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC D;
        D.Flags                this->Flags;
        D.NodeMask             this->NodeMask;
        D.pRootSignature       this->pRootSignature;
        D.InputLayout          this->InputLayout;
        D.IBStripCutValue      this->IBStripCutValue;
        D.PrimitiveTopologyTypethis->PrimitiveTopologyType;
        D.VS                   this->VS;
        D.GS                   this->GS;
        D.StreamOutput         this->StreamOutput;
        D.HS                   this->HS;
        D.DS                   this->DS;
        D.PS                   this->PS;
        D.BlendState           this->BlendState;
        D.DepthStencilState    CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEPTH_STENCIL_DESC1(this->DepthStencilState));
        D.DSVFormat            this->DSVFormat;
        D.RasterizerState      this->RasterizerState;
        D.NumRenderTargets     D3D12_RT_FORMAT_ARRAY(this->RTVFormats).NumRenderTargets;
        memcpy(D.RTVFormats, D3D12_RT_FORMAT_ARRAY(this->RTVFormats).RTFormats, sizeof(D.RTVFormats));
        D.SampleDesc           this->SampleDesc;
        D.SampleMask           this->SampleMask;
        D.CachedPSO            this->CachedPSO;
        return D;
    }
    D3D12_COMPUTE_PIPELINE_STATE_DESC ComputeDescV0() const
    {
        D3D12_COMPUTE_PIPELINE_STATE_DESC D;
        D.Flags                this->Flags;
        D.NodeMask             this->NodeMask;
        D.pRootSignature       this->pRootSignature;
        D.CS                   this->CS;
        D.CachedPSO            this->CachedPSO;
        return D;
    }
};

// CD3DX12_PIPELINE_STATE_STREAM works on RS2+ but does not support new subobject(s) added in RS3+.
// See CD3DX12_PIPELINE_STATE_STREAM1 for instance.
struct CD3DX12_PIPELINE_STATE_STREAM
{
    CD3DX12_PIPELINE_STATE_STREAM()default;
    CD3DX12_PIPELINE_STATE_STREAM(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& Desc)
        : Flags(Desc.Flags)
        , NodeMask(Desc.NodeMask)
        , pRootSignature(Desc.pRootSignature)
        , InputLayout(Desc.InputLayout)
        , IBStripCutValue(Desc.IBStripCutValue)
        , PrimitiveTopologyType(Desc.PrimitiveTopologyType)
        , VS(Desc.VS)
        , GS(Desc.GS)
        , StreamOutput(Desc.StreamOutput)
        , HS(Desc.HS)
        , DS(Desc.DS)
        , PS(Desc.PS)
        , BlendState(CD3DX12_BLEND_DESC(Desc.BlendState))
        , DepthStencilState(CD3DX12_DEPTH_STENCIL_DESC1(Desc.DepthStencilState))
        , DSVFormat(Desc.DSVFormat)
        , RasterizerState(CD3DX12_RASTERIZER_DESC(Desc.RasterizerState))
        , RTVFormats(CD3DX12_RT_FORMAT_ARRAY(Desc.RTVFormats, Desc.NumRenderTargets))
        , SampleDesc(Desc.SampleDesc)
        , SampleMask(Desc.SampleMask)
        , CachedPSO(Desc.CachedPSO)
    {}
    CD3DX12_PIPELINE_STATE_STREAM(const D3D12_COMPUTE_PIPELINE_STATE_DESC& Desc)
        : Flags(Desc.Flags)
        , NodeMask(Desc.NodeMask)
        , pRootSignature(Desc.pRootSignature)
        , CS(CD3DX12_SHADER_BYTECODE(Desc.CS))
        , CachedPSO(Desc.CachedPSO)
    {}
    CD3DX12_PIPELINE_STATE_STREAM_FLAGS Flags;
    CD3DX12_PIPELINE_STATE_STREAM_NODE_MASK NodeMask;
    CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
    CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
    CD3DX12_PIPELINE_STATE_STREAM_IB_STRIP_CUT_VALUE IBStripCutValue;
    CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
    CD3DX12_PIPELINE_STATE_STREAM_VS VS;
    CD3DX12_PIPELINE_STATE_STREAM_GS GS;
    CD3DX12_PIPELINE_STATE_STREAM_STREAM_OUTPUT StreamOutput;
    CD3DX12_PIPELINE_STATE_STREAM_HS HS;
    CD3DX12_PIPELINE_STATE_STREAM_DS DS;
    CD3DX12_PIPELINE_STATE_STREAM_PS PS;
    CD3DX12_PIPELINE_STATE_STREAM_CS CS;
    CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC BlendState;
    CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL1 DepthStencilState;
    CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
    CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER RasterizerState;
    CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
    CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC SampleDesc;
    CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_MASK SampleMask;
    CD3DX12_PIPELINE_STATE_STREAM_CACHED_PSO CachedPSO;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC GraphicsDescV0() const
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC D;
        D.Flags                this->Flags;
        D.NodeMask             this->NodeMask;
        D.pRootSignature       this->pRootSignature;
        D.InputLayout          this->InputLayout;
        D.IBStripCutValue      this->IBStripCutValue;
        D.PrimitiveTopologyTypethis->PrimitiveTopologyType;
        D.VS                   this->VS;
        D.GS                   this->GS;
        D.StreamOutput         this->StreamOutput;
        D.HS                   this->HS;
        D.DS                   this->DS;
        D.PS                   this->PS;
        D.BlendState           this->BlendState;
        D.DepthStencilState    CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEPTH_STENCIL_DESC1(this->DepthStencilState));
        D.DSVFormat            this->DSVFormat;
        D.RasterizerState      this->RasterizerState;
        D.NumRenderTargets     D3D12_RT_FORMAT_ARRAY(this->RTVFormats).NumRenderTargets;
        memcpy(D.RTVFormats, D3D12_RT_FORMAT_ARRAY(this->RTVFormats).RTFormats, sizeof(D.RTVFormats));
        D.SampleDesc           this->SampleDesc;
        D.SampleMask           this->SampleMask;
        D.CachedPSO            this->CachedPSO;
        return D;
    }
    D3D12_COMPUTE_PIPELINE_STATE_DESC ComputeDescV0() const
    {
        D3D12_COMPUTE_PIPELINE_STATE_DESC D;
        D.Flags                this->Flags;
        D.NodeMask             this->NodeMask;
        D.pRootSignature       this->pRootSignature;
        D.CS                   this->CS;
        D.CachedPSO            this->CachedPSO;
        return D;
    }
};

struct CD3DX12_PIPELINE_STATE_STREAM_PARSE_HELPER : public ID3DX12PipelineParserCallbacks
{
    CD3DX12_PIPELINE_STATE_STREAM1 PipelineStream;
    CD3DX12_PIPELINE_STATE_STREAM_PARSE_HELPER() noexcept
        : SeenDSS(false)
    {
        // Adjust defaults to account for absent members.
        PipelineStream.PrimitiveTopologyTypeD3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        // Depth disabled if no DSV format specified.
        static_cast<D3D12_DEPTH_STENCIL_DESC1&>(PipelineStream.DepthStencilState).DepthEnablefalse;
    }

    // ID3DX12PipelineParserCallbacks
    void FlagsCb(D3D12_PIPELINE_STATE_FLAGS Flags) override {PipelineStream.FlagsFlags;}
    void NodeMaskCb(UINT NodeMask) override {PipelineStream.NodeMaskNodeMask;}
    void RootSignatureCb(ID3D12RootSignature* pRootSignature) override {PipelineStream.pRootSignaturepRootSignature;}
    void InputLayoutCb(const D3D12_INPUT_LAYOUT_DESC& InputLayout) override {PipelineStream.InputLayoutInputLayout;}
    void IBStripCutValueCb(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBStripCutValue) override {PipelineStream.IBStripCutValueIBStripCutValue;}
    void PrimitiveTopologyTypeCb(D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopologyType) override {PipelineStream.PrimitiveTopologyTypePrimitiveTopologyType;}
    void VSCb(const D3D12_SHADER_BYTECODE& VS) override {PipelineStream.VSVS;}
    void GSCb(const D3D12_SHADER_BYTECODE& GS) override {PipelineStream.GSGS;}
    void StreamOutputCb(const D3D12_STREAM_OUTPUT_DESC& StreamOutput) override {PipelineStream.StreamOutputStreamOutput;}
    void HSCb(const D3D12_SHADER_BYTECODE& HS) override {PipelineStream.HSHS;}
    void DSCb(const D3D12_SHADER_BYTECODE& DS) override {PipelineStream.DSDS;}
    void PSCb(const D3D12_SHADER_BYTECODE& PS) override {PipelineStream.PSPS;}
    void CSCb(const D3D12_SHADER_BYTECODE& CS) override {PipelineStream.CSCS;}
    void BlendStateCb(const D3D12_BLEND_DESC& BlendState) override {PipelineStream.BlendStateCD3DX12_BLEND_DESC(BlendState);}
    void DepthStencilStateCb(const D3D12_DEPTH_STENCIL_DESC& DepthStencilState) override
    {
        PipelineStream.DepthStencilStateCD3DX12_DEPTH_STENCIL_DESC1(DepthStencilState);
        SeenDSStrue;
    }
    void DepthStencilState1Cb(const D3D12_DEPTH_STENCIL_DESC1& DepthStencilState) override
    {
        PipelineStream.DepthStencilStateCD3DX12_DEPTH_STENCIL_DESC1(DepthStencilState);
        SeenDSStrue;
    }
    void DSVFormatCb(DXGI_FORMAT DSVFormat) override
    {
        PipelineStream.DSVFormatDSVFormat;
        if (!SeenDSS && DSVFormat != DXGI_FORMAT_UNKNOWN)
        {
            // Re-enable depth for the default state.
            static_cast<D3D12_DEPTH_STENCIL_DESC1&>(PipelineStream.DepthStencilState).DepthEnabletrue;
        }
    }
    void RasterizerStateCb(const D3D12_RASTERIZER_DESC& RasterizerState) override {PipelineStream.RasterizerStateCD3DX12_RASTERIZER_DESC(RasterizerState);}
    void RTVFormatsCb(const D3D12_RT_FORMAT_ARRAY& RTVFormats) override {PipelineStream.RTVFormatsRTVFormats;}
    void SampleDescCb(const DXGI_SAMPLE_DESC& SampleDesc) override {PipelineStream.SampleDescSampleDesc;}
    void SampleMaskCb(UINT SampleMask) override {PipelineStream.SampleMaskSampleMask;}
    void ViewInstancingCb(const D3D12_VIEW_INSTANCING_DESC& ViewInstancingDesc) override {PipelineStream.ViewInstancingDescCD3DX12_VIEW_INSTANCING_DESC(ViewInstancingDesc);}
    void CachedPSOCb(const D3D12_CACHED_PIPELINE_STATE& CachedPSO) override {PipelineStream.CachedPSOCachedPSO;}

private:
    bool SeenDSS;
};

inline D3D12_PIPELINE_STATE_SUBOBJECT_TYPE D3DX12GetBaseSubobjectType(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE SubobjectType)
{
    switch (SubobjectType)
    {
    case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL1: 
        return D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL;
    default:
        return SubobjectType;
    }
}

inline HRESULT D3DX12ParsePipelineStream(const D3D12_PIPELINE_STATE_STREAM_DESC& Desc, ID3DX12PipelineParserCallbacks* pCallbacks)
{
    if (pCallbacks == nullptr)
    {
        return E_INVALIDARG;
    }

    if (Desc.SizeInBytes == 0 || Desc.pPipelineStateSubobjectStream == nullptr)
    {
        pCallbacks->ErrorBadInputParameter(1); // first parameter issue
        return E_INVALIDARG;
    }

    bool SubobjectSeen[D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_MAX_VALID]{};
    for (SIZE_T CurOffset0, SizeOfSubobject0; CurOffset < Desc.SizeInBytes; CurOffset += SizeOfSubobject)
    {
        BYTE* pStreamstatic_cast<BYTE*>(Desc.pPipelineStateSubobjectStream)+CurOffset;
        auto SubobjectType*reinterpret_cast<D3D12_PIPELINE_STATE_SUBOBJECT_TYPE*>(pStream);
        if (SubobjectType < 0 || SubobjectType >= D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_MAX_VALID)
        {
            pCallbacks->ErrorUnknownSubobject(SubobjectType);
            return E_INVALIDARG;
        }
        if (SubobjectSeen[D3DX12GetBaseSubobjectType(SubobjectType)])
        {
            pCallbacks->ErrorDuplicateSubobject(SubobjectType);
            return E_INVALIDARG; // disallow subobject duplicates in a stream
        }
        SubobjectSeen[SubobjectType]true;
        switch (SubobjectType)
        {
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_ROOT_SIGNATURE: 
            pCallbacks->RootSignatureCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::pRootSignature)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::pRootSignature);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VS:
            pCallbacks->VSCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::VS)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::VS);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PS: 
            pCallbacks->PSCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::PS)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::PS);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DS: 
            pCallbacks->DSCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::DS)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::DS);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_HS: 
            pCallbacks->HSCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::HS)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::HS);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_GS: 
            pCallbacks->GSCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::GS)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::GS);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CS:
            pCallbacks->CSCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::CS)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::CS);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_STREAM_OUTPUT: 
            pCallbacks->StreamOutputCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::StreamOutput)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::StreamOutput);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_BLEND: 
            pCallbacks->BlendStateCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::BlendState)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::BlendState);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_MASK: 
            pCallbacks->SampleMaskCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::SampleMask)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::SampleMask);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RASTERIZER: 
            pCallbacks->RasterizerStateCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::RasterizerState)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::RasterizerState);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL: 
            pCallbacks->DepthStencilStateCb(*reinterpret_cast<CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL1: 
            pCallbacks->DepthStencilState1Cb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::DepthStencilState)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::DepthStencilState);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_INPUT_LAYOUT: 
            pCallbacks->InputLayoutCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::InputLayout)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::InputLayout);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_IB_STRIP_CUT_VALUE: 
            pCallbacks->IBStripCutValueCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::IBStripCutValue)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::IBStripCutValue);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PRIMITIVE_TOPOLOGY: 
            pCallbacks->PrimitiveTopologyTypeCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::PrimitiveTopologyType)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::PrimitiveTopologyType);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS: 
            pCallbacks->RTVFormatsCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::RTVFormats)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::RTVFormats);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL_FORMAT: 
            pCallbacks->DSVFormatCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::DSVFormat)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::DSVFormat);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_DESC: 
            pCallbacks->SampleDescCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::SampleDesc)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::SampleDesc);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_NODE_MASK: 
            pCallbacks->NodeMaskCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::NodeMask)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::NodeMask);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CACHED_PSO: 
            pCallbacks->CachedPSOCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::CachedPSO)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::CachedPSO);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_FLAGS:
            pCallbacks->FlagsCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM::Flags)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM::Flags);
            break;
        case D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VIEW_INSTANCING:
            pCallbacks->ViewInstancingCb(*reinterpret_cast<decltype(CD3DX12_PIPELINE_STATE_STREAM1::ViewInstancingDesc)*>(pStream));
            SizeOfSubobjectsizeof(CD3DX12_PIPELINE_STATE_STREAM1::ViewInstancingDesc);
            break;
        default:
            pCallbacks->ErrorUnknownSubobject(SubobjectType);
            return E_INVALIDARG;
            break;
        }
    }

    return S_OK;
}

//------------------------------------------------------------------------------------------------
inline bool operator==( const D3D12_CLEAR_VALUE &a, const D3D12_CLEAR_VALUE &b)
{
    if (a.Format != b.Format) return false;
    if (a.Format == DXGI_FORMAT_D24_UNORM_S8_UINT
     || a.Format == DXGI_FORMAT_D16_UNORM
     || a.Format == DXGI_FORMAT_D32_FLOAT
     || a.Format == DXGI_FORMAT_D32_FLOAT_S8X24_UINT)
    {
        return (a.DepthStencil.Depth == b.DepthStencil.Depth) && 
          (a.DepthStencil.Stencil == b.DepthStencil.Stencil);
    } else {
        return (a.Color[0] == b.Color[0]) && 
               (a.Color[1] == b.Color[1]) && 
               (a.Color[2] == b.Color[2]) && 
               (a.Color[3] == b.Color[3]);
    }
}
inline bool operator==( const D3D12_RENDER_PASS_BEGINNING_ACCESS_CLEAR_PARAMETERS &a, const D3D12_RENDER_PASS_BEGINNING_ACCESS_CLEAR_PARAMETERS &b)
{
    return a.ClearValue == b.ClearValue;
}
inline bool operator==( const D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS &a, const D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS &b)
{
    if (a.pSrcResource != b.pSrcResource) return false;
    if (a.pDstResource != b.pDstResource) return false;
    if (a.SubresourceCount != b.SubresourceCount) return false;
    if (a.Format != b.Format) return false;
    if (a.ResolveMode != b.ResolveMode) return false; 
    if (a.PreserveResolveSource != b.PreserveResolveSource) return false; 
    return true;
}
inline bool operator==( const D3D12_RENDER_PASS_BEGINNING_ACCESS &a, const D3D12_RENDER_PASS_BEGINNING_ACCESS &b)
{
    if (a.Type != b.Type) return false;
    if (a.Type == D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR && !(a.Clear == b.Clear)) return false; 
    return true;
}
inline bool operator==( const D3D12_RENDER_PASS_ENDING_ACCESS &a, const D3D12_RENDER_PASS_ENDING_ACCESS &b)
{
    if (a.Type != b.Type) return false;
    if (a.Type == D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE && !(a.Resolve == b.Resolve)) return false; 
    return true;
}
inline bool operator==( const D3D12_RENDER_PASS_RENDER_TARGET_DESC &a, const D3D12_RENDER_PASS_RENDER_TARGET_DESC &b)
{
    if (a.cpuDescriptor.ptr != b.cpuDescriptor.ptr) return false;
    if (!(a.BeginningAccess == b.BeginningAccess)) return false;
    if (!(a.EndingAccess == b.EndingAccess)) return false;
    return true;
}
inline bool operator==( const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC &a, const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC &b)
{
    if (a.cpuDescriptor.ptr != b.cpuDescriptor.ptr) return false;
    if (!(a.DepthBeginningAccess == b.DepthBeginningAccess)) return false;
    if (!(a.StencilBeginningAccess == b.StencilBeginningAccess)) return false;
    if (!(a.DepthEndingAccess == b.DepthEndingAccess)) return false;
    if (!(a.StencilEndingAccess == b.StencilEndingAccess)) return false;
    return true;
}


#ifndef D3DX12_NO_STATE_OBJECT_HELPERS

//================================================================================================
// D3DX12 State Object Creation Helpers
// 
// Helper classes for creating new style state objects out of an arbitrary set of subobjects.
// Uses STL
//
// Start by instantiating CD3DX12_STATE_OBJECT_DESC (see it's public methods).
// One of its methods is CreateSubobject(), which has a comment showing a couple of options for
// defining subobjects using the helper classes for each subobject (CD3DX12_DXIL_LIBRARY_SUBOBJECT 
// etc.). The subobject helpers each have methods specific to the subobject for configuring it's 
// contents.
// 
//================================================================================================
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <wrl/client.h>

//------------------------------------------------------------------------------------------------
class CD3DX12_STATE_OBJECT_DESC
{
public:
    CD3DX12_STATE_OBJECT_DESC()
    {
        Init(D3D12_STATE_OBJECT_TYPE_COLLECTION);
    }
    CD3DX12_STATE_OBJECT_DESC(D3D12_STATE_OBJECT_TYPE Type)
    {
        Init(Type);
    }
    void SetStateObjectType(D3D12_STATE_OBJECT_TYPE Type) { m_Desc.TypeType; }
    operator const D3D12_STATE_OBJECT_DESC&()
    {
        // Do final preparation work
        m_RepointedAssociations.clear();
        m_SubobjectArray.clear();
        m_SubobjectArray.reserve(m_Desc.NumSubobjects);
        // Flatten subobjects into an array (each flattened subobject still has a 
        // member that's a pointer to it's desc that's not flattened)
        for (auto Iterm_SubobjectList.begin();
            Iter != m_SubobjectList.end(); Iter++)
        {
            m_SubobjectArray.push_back(*Iter);
            // Store new location in array so we can redirect pointers contained in subobjects 
            Iter->pSubobjectArrayLocation&m_SubobjectArray.back(); 
        }
        // For subobjects with pointer fields, create a new copy of those subobject definitions
        // with fixed pointers
        for (UINT i0; i < m_Desc.NumSubobjects; i++)
        {
            if (m_SubobjectArray[i].Type == D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION)
            {
                auto pOriginalSubobjectAssociation
                    reinterpret_cast<const D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION*>(m_SubobjectArray[i].pDesc);
                D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION Repointed*pOriginalSubobjectAssociation;
                auto pWrapper
                    static_cast<const SUBOBJECT_WRAPPER*>(pOriginalSubobjectAssociation->pSubobjectToAssociate);
                Repointed.pSubobjectToAssociatepWrapper->pSubobjectArrayLocation;
                m_RepointedAssociations.push_back(Repointed);
                m_SubobjectArray[i].pDesc&m_RepointedAssociations.back();
            }
        }
        // Below: using ugly way to get pointer in case .data() is not defined
        m_Desc.pSubobjectsm_Desc.NumSubobjects ? &m_SubobjectArray[0] : nullptr; 
        return m_Desc;
    }
    operator const D3D12_STATE_OBJECT_DESC*()
    {
        // Cast calls the above final preparation work
        return &static_cast<const D3D12_STATE_OBJECT_DESC&>(*this);
    }

    // CreateSubobject creates a sububject helper (e.g. CD3DX12_HIT_GROUP_SUBOBJECT) 
    // whose lifetime is owned by this class.
    // e.g. 
    // 
    //    CD3DX12_STATE_OBJECT_DESC Collection1(D3D12_STATE_OBJECT_TYPE_COLLECTION);
    //    auto Lib0Collection1.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
    //    Lib0->SetDXILLibrary(&pMyAppDxilLibs[0]);
    //    Lib0->DefineExport(L"rayGenShader0"); // in practice these export listings might be 
    //                                          // data/engine driven
    //    etc.
    //
    // Alternatively, users can instantiate sububject helpers explicitly, such as via local 
    // variables instead, passing the state object desc that should point to it into the helper 
    // constructor (or call mySubobjectHelper.AddToStateObject(Collection1)).  
    // In this alternative scenario, the user must keep the subobject alive as long as the state 
    // object it is associated with is alive, else it's pointer references will be stale.
    // e.g.
    //
    //    CD3DX12_STATE_OBJECT_DESC RaytracingState2(D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE);
    //    CD3DX12_DXIL_LIBRARY_SUBOBJECT LibA(RaytracingState2);
    //    LibA.SetDXILLibrary(&pMyAppDxilLibs[4]); // not manually specifying exports 
    //                                             // - meaning all exports in the libraries 
    //                                             // are exported
    //    etc.

    template<typename T>
    T* CreateSubobject()
    {
        T* pSubobjectnew T(*this);
        m_OwnedSubobjectHelpers.emplace_back(pSubobject);
        return pSubobject;
    }

private:
    D3D12_STATE_SUBOBJECT* TrackSubobject(D3D12_STATE_SUBOBJECT_TYPE Type, void* pDesc)
    {
        SUBOBJECT_WRAPPER Subobject;
        Subobject.pSubobjectArrayLocationnullptr;
        Subobject.TypeType;
        Subobject.pDescpDesc;
        m_SubobjectList.push_back(Subobject);
        m_Desc.NumSubobjects++;
        return &m_SubobjectList.back();
    }
    void Init(D3D12_STATE_OBJECT_TYPE Type)
    {
        SetStateObjectType(Type);
        m_Desc.pSubobjectsnullptr;
        m_Desc.NumSubobjects0;
        m_SubobjectList.clear();
        m_SubobjectArray.clear();
        m_RepointedAssociations.clear();
    }
    typedef struct SUBOBJECT_WRAPPER : public D3D12_STATE_SUBOBJECT
    {
        D3D12_STATE_SUBOBJECT* pSubobjectArrayLocation; // new location when flattened into array 
                                                        // for repointing pointers in subobjects
    } SUBOBJECT_WRAPPER;
    D3D12_STATE_OBJECT_DESC m_Desc;
    std::list<SUBOBJECT_WRAPPER>   m_SubobjectList; // Pointers to list nodes handed out so 
                                                    // these can be edited live
    std::vector<D3D12_STATE_SUBOBJECT> m_SubobjectArray; // Built at the end, copying list contents

    std::list<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION> 
            m_RepointedAssociations; // subobject type that contains pointers to other subobjects, 
                                     // repointed to flattened array

    class StringContainer
    {
    public:
        LPCWSTR LocalCopy(LPCWSTR string, bool bSingleStringfalse)
        {
            if (string)
            {
                if (bSingleString)
                {
                    m_Strings.clear();
                    m_Strings.push_back(string);
                }
                else
                {
                    m_Strings.push_back(string);
                }
                return m_Strings.back().c_str();
            }
            else
            {
                return nullptr;
            }
        }
        void clear() { m_Strings.clear(); }
    private:
        std::list<std::wstring> m_Strings;
    };

    class SUBOBJECT_HELPER_BASE
    {
    public:
        SUBOBJECT_HELPER_BASE() { Init(); }
        virtual ~SUBOBJECT_HELPER_BASE() {}
        virtual D3D12_STATE_SUBOBJECT_TYPE Type() const0;
        void AddToStateObject(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
        {
            m_pSubobjectContainingStateObject.TrackSubobject(Type(), Data());
        }
    protected:
        virtual void* Data()0;
        void Init() { m_pSubobjectnullptr; }
        D3D12_STATE_SUBOBJECT* m_pSubobject;
    };

#if(__cplusplus >= 201103L)
    std::list<std::unique_ptr<const SUBOBJECT_HELPER_BASE>> m_OwnedSubobjectHelpers;
#else
    class OWNED_HELPER
    {
    public:
        OWNED_HELPER(const SUBOBJECT_HELPER_BASE* pHelper) { m_pHelperpHelper; }
        ~OWNED_HELPER() { delete m_pHelper; }
        const SUBOBJECT_HELPER_BASE* m_pHelper;
    };

    std::list<OWNED_HELPER> m_OwnedSubobjectHelpers;
#endif

    friend class CD3DX12_DXIL_LIBRARY_SUBOBJECT;
    friend class CD3DX12_EXISTING_COLLECTION_SUBOBJECT;
    friend class CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT;
    friend class CD3DX12_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
    friend class CD3DX12_HIT_GROUP_SUBOBJECT;
    friend class CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT;
    friend class CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT;
    friend class CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT;
    friend class CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT;
    friend class CD3DX12_STATE_OBJECT_CONFIG_SUBOBJECT;
    friend class CD3DX12_NODE_MASK_SUBOBJECT;
};

//------------------------------------------------------------------------------------------------
class CD3DX12_DXIL_LIBRARY_SUBOBJECT 
    : public CD3DX12_STATE_OBJECT_DESC::SUBOBJECT_HELPER_BASE
{
public:
    CD3DX12_DXIL_LIBRARY_SUBOBJECT()
    {
        Init();
    }
    CD3DX12_DXIL_LIBRARY_SUBOBJECT(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
    {
        Init();
        AddToStateObject(ContainingStateObject);
    }
    void SetDXILLibrary(D3D12_SHADER_BYTECODE*pCode) 
    { 
        static const D3D12_SHADER_BYTECODE Default{}; 
        m_Desc.DXILLibrarypCode ? *pCode : Default; 
    }
    void DefineExport(
        LPCWSTR Name, 
        LPCWSTR ExportToRenamenullptr, 
        D3D12_EXPORT_FLAGS FlagsD3D12_EXPORT_FLAG_NONE)
    {
        D3D12_EXPORT_DESC Export;
        Export.Namem_Strings.LocalCopy(Name);
        Export.ExportToRenamem_Strings.LocalCopy(ExportToRename);
        Export.FlagsFlags;
        m_Exports.push_back(Export);
        m_Desc.pExports&m_Exports[0];  // using ugly way to get pointer in case .data() is not defined
        m_Desc.NumExportsstatic_cast<UINT>(m_Exports.size());
    }
    template<size_t N>
    void DefineExports(LPCWSTR(&Exports)[N])
    {
        for (UINT i0; i < N; i++)
        {
            DefineExport(Exports[i]);
        }
    }
    void DefineExports(LPCWSTR* Exports, UINT N)
    {
        for (UINT i0; i < N; i++)
        {
            DefineExport(Exports[i]);
        }
    }    
    D3D12_STATE_SUBOBJECT_TYPE Type() const 
    { 
        return D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY; 
    }
    operator const D3D12_STATE_SUBOBJECT&() const { return *m_pSubobject; }
    operator const D3D12_DXIL_LIBRARY_DESC&() const { return m_Desc; }
private:
    void Init()
    {
        SUBOBJECT_HELPER_BASE::Init();
        m_Desc{};
        m_Strings.clear();
        m_Exports.clear();
    }
    void* Data() { return &m_Desc; }
    D3D12_DXIL_LIBRARY_DESC m_Desc;
    CD3DX12_STATE_OBJECT_DESC::StringContainer m_Strings;
    std::vector<D3D12_EXPORT_DESC> m_Exports;
};

//------------------------------------------------------------------------------------------------
class CD3DX12_EXISTING_COLLECTION_SUBOBJECT 
    : public CD3DX12_STATE_OBJECT_DESC::SUBOBJECT_HELPER_BASE
{
public:
    CD3DX12_EXISTING_COLLECTION_SUBOBJECT()
    {
        Init();
    }
    CD3DX12_EXISTING_COLLECTION_SUBOBJECT(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
    {
        Init();
        AddToStateObject(ContainingStateObject);
    }
    void SetExistingCollection(ID3D12StateObject*pExistingCollection) 
    { 
        m_Desc.pExistingCollectionpExistingCollection; 
        m_CollectionRefpExistingCollection; 
    }
    void DefineExport(
        LPCWSTR Name, 
        LPCWSTR ExportToRenamenullptr, 
        D3D12_EXPORT_FLAGS FlagsD3D12_EXPORT_FLAG_NONE)
    {
        D3D12_EXPORT_DESC Export;
        Export.Namem_Strings.LocalCopy(Name);
        Export.ExportToRenamem_Strings.LocalCopy(ExportToRename);
        Export.FlagsFlags;
        m_Exports.push_back(Export);
        m_Desc.pExports&m_Exports[0]; // using ugly way to get pointer in case .data() is not defined
        m_Desc.NumExportsstatic_cast<UINT>(m_Exports.size());
    }
    template<size_t N>
    void DefineExports(LPCWSTR(&Exports)[N])
    {
        for (UINT i0; i < N; i++)
        {
            DefineExport(Exports[i]);
        }
    }
    void DefineExports(LPCWSTR* Exports, UINT N)
    {
        for (UINT i0; i < N; i++)
        {
            DefineExport(Exports[i]);
        }
    }    
    D3D12_STATE_SUBOBJECT_TYPE Type() const
    { 
        return D3D12_STATE_SUBOBJECT_TYPE_EXISTING_COLLECTION; 
    }
    operator const D3D12_STATE_SUBOBJECT&() const { return *m_pSubobject; }
    operator const D3D12_EXISTING_COLLECTION_DESC&() const { return m_Desc; }
private:
    void Init()
    {
        SUBOBJECT_HELPER_BASE::Init();
        m_Desc{};
        m_CollectionRefnullptr;
        m_Strings.clear();
        m_Exports.clear();
    }
    void* Data() { return &m_Desc; }
    D3D12_EXISTING_COLLECTION_DESC m_Desc;
    Microsoft::WRL::ComPtr<ID3D12StateObject> m_CollectionRef;
    CD3DX12_STATE_OBJECT_DESC::StringContainer m_Strings;
    std::vector<D3D12_EXPORT_DESC> m_Exports;
};

//------------------------------------------------------------------------------------------------
class CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT 
    : public CD3DX12_STATE_OBJECT_DESC::SUBOBJECT_HELPER_BASE
{
public:
    CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT()
    {
        Init();
    }
    CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
    {
        Init();
        AddToStateObject(ContainingStateObject);
    }
    void SetSubobjectToAssociate(const D3D12_STATE_SUBOBJECT& SubobjectToAssociate) 
    { 
        m_Desc.pSubobjectToAssociate&SubobjectToAssociate; 
    }
    void AddExport(LPCWSTR Export)
    {
        m_Desc.NumExports++;
        m_Exports.push_back(m_Strings.LocalCopy(Export));
        m_Desc.pExports&m_Exports[0];  // using ugly way to get pointer in case .data() is not defined
    }
    template<size_t N>
    void AddExports(LPCWSTR (&Exports)[N])
    {
        for (UINT i0; i < N; i++)
        {
            AddExport(Exports[i]);
        }
    }
    void AddExports(LPCWSTR* Exports, UINT N)
    {
        for (UINT i0; i < N; i++)
        {
            AddExport(Exports[i]);
        }
    }    
    D3D12_STATE_SUBOBJECT_TYPE Type() const 
    { 
        return D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION; 
    }
    operator const D3D12_STATE_SUBOBJECT&() const { return *m_pSubobject; }
    operator const D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION&() const { return m_Desc; }
private:
    void Init()
    {
        SUBOBJECT_HELPER_BASE::Init();
        m_Desc{};
        m_Strings.clear();
        m_Exports.clear();
    }
    void* Data() { return &m_Desc; }
    D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION m_Desc;
    CD3DX12_STATE_OBJECT_DESC::StringContainer m_Strings;
    std::vector<LPCWSTR> m_Exports;
};

//------------------------------------------------------------------------------------------------
class CD3DX12_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION 
    : public CD3DX12_STATE_OBJECT_DESC::SUBOBJECT_HELPER_BASE
{
public:
    CD3DX12_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION()
    {
        Init();
    }
    CD3DX12_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
    {
        Init();
        AddToStateObject(ContainingStateObject);
    }
    void SetSubobjectNameToAssociate(LPCWSTR SubobjectToAssociate) 
    {
        m_Desc.SubobjectToAssociatem_SubobjectName.LocalCopy(SubobjectToAssociate, true); 
    }
    void AddExport(LPCWSTR Export)
    {
        m_Desc.NumExports++;
        m_Exports.push_back(m_Strings.LocalCopy(Export));
        m_Desc.pExports&m_Exports[0];  // using ugly way to get pointer in case .data() is not defined
    }
    template<size_t N>
    void AddExports(LPCWSTR (&Exports)[N])
    {
        for (UINT i0; i < N; i++)
        {
            AddExport(Exports[i]);
        }
    }
    void AddExports(LPCWSTR* Exports, UINT N)
    {
        for (UINT i0; i < N; i++)
        {
            AddExport(Exports[i]);
        }
    }    
    D3D12_STATE_SUBOBJECT_TYPE Type() const
    { 
        return D3D12_STATE_SUBOBJECT_TYPE_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION; 
    }
    operator const D3D12_STATE_SUBOBJECT&() const { return *m_pSubobject; }
    operator const D3D12_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION&() const { return m_Desc; }
private:
    void Init()
    {
        SUBOBJECT_HELPER_BASE::Init();
        m_Desc{};
        m_Strings.clear();
        m_SubobjectName.clear();
        m_Exports.clear();
    }
    void* Data() { return &m_Desc; }
    D3D12_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION m_Desc;
    CD3DX12_STATE_OBJECT_DESC::StringContainer m_Strings;
    CD3DX12_STATE_OBJECT_DESC::StringContainer m_SubobjectName;
    std::vector<LPCWSTR> m_Exports;
};

//------------------------------------------------------------------------------------------------
class CD3DX12_HIT_GROUP_SUBOBJECT 
    : public CD3DX12_STATE_OBJECT_DESC::SUBOBJECT_HELPER_BASE
{
public:
    CD3DX12_HIT_GROUP_SUBOBJECT()
    {
        Init();
    }
    CD3DX12_HIT_GROUP_SUBOBJECT(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
    {
        Init();
        AddToStateObject(ContainingStateObject);
    }
    void SetHitGroupExport(LPCWSTR exportName)
    { 
        m_Desc.HitGroupExportm_Strings[0].LocalCopy(exportName, true); 
    }
    void SetHitGroupType(D3D12_HIT_GROUP_TYPE Type) { m_Desc.TypeType; }
    void SetAnyHitShaderImport(LPCWSTR importName)
    { 
        m_Desc.AnyHitShaderImportm_Strings[1].LocalCopy(importName, true); 
    }
    void SetClosestHitShaderImport(LPCWSTR importName)
    { 
        m_Desc.ClosestHitShaderImportm_Strings[2].LocalCopy(importName, true); 
    }
    void SetIntersectionShaderImport(LPCWSTR importName)
    {
        m_Desc.IntersectionShaderImportm_Strings[3].LocalCopy(importName, true); 
    }
    D3D12_STATE_SUBOBJECT_TYPE Type() const
    {
        return D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP; 
    }
    operator const D3D12_STATE_SUBOBJECT&() const { return *m_pSubobject; }
    operator const D3D12_HIT_GROUP_DESC&() const { return m_Desc; }
private:
    void Init()
    {
        SUBOBJECT_HELPER_BASE::Init();
        m_Desc{};
        for (UINT i0; i < m_NumStrings; i++)
        {
            m_Strings[i].clear();
        }
    }
    void* Data() { return &m_Desc; }
    D3D12_HIT_GROUP_DESC m_Desc;
    static const UINT m_NumStrings;
    CD3DX12_STATE_OBJECT_DESC::StringContainer 
        m_Strings[m_NumStrings]; // one string for every entrypoint name
};

//------------------------------------------------------------------------------------------------
class CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT 
    : public CD3DX12_STATE_OBJECT_DESC::SUBOBJECT_HELPER_BASE
{
public:
    CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT()
    {
        Init();
    }
    CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
    {
        Init();
        AddToStateObject(ContainingStateObject);
    }
    void Config(UINT MaxPayloadSizeInBytes, UINT MaxAttributeSizeInBytes)
    {
        m_Desc.MaxPayloadSizeInBytesMaxPayloadSizeInBytes;
        m_Desc.MaxAttributeSizeInBytesMaxAttributeSizeInBytes;
    }
    D3D12_STATE_SUBOBJECT_TYPE Type() const
    { 
        return D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG; 
    }
    operator const D3D12_STATE_SUBOBJECT&() const { return *m_pSubobject; }
    operator const D3D12_RAYTRACING_SHADER_CONFIG&() const { return m_Desc; }
private:
    void Init()
    {
        SUBOBJECT_HELPER_BASE::Init();
        m_Desc{};
    }
    void* Data() { return &m_Desc; }
    D3D12_RAYTRACING_SHADER_CONFIG m_Desc;
};

//------------------------------------------------------------------------------------------------
class CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT 
    : public CD3DX12_STATE_OBJECT_DESC::SUBOBJECT_HELPER_BASE
{
public:
    CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT()
    {
        Init();
    }
    CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
    {
        Init();
        AddToStateObject(ContainingStateObject);
    }
    void Config(UINT MaxTraceRecursionDepth)
    {
        m_Desc.MaxTraceRecursionDepthMaxTraceRecursionDepth;
    }
    D3D12_STATE_SUBOBJECT_TYPE Type() const
    { 
        return D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG; 
    }
    operator const D3D12_STATE_SUBOBJECT&() const { return *m_pSubobject; }
    operator const D3D12_RAYTRACING_PIPELINE_CONFIG&() const { return m_Desc; }
private:
    void Init()
    {
        SUBOBJECT_HELPER_BASE::Init();
        m_Desc{};
    }
    void* Data() { return &m_Desc; }
    D3D12_RAYTRACING_PIPELINE_CONFIG m_Desc;
};

//------------------------------------------------------------------------------------------------
class CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT 
    : public CD3DX12_STATE_OBJECT_DESC::SUBOBJECT_HELPER_BASE
{
public:
    CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT()
    {
        Init();
    }
    CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
    {
        Init();
        AddToStateObject(ContainingStateObject);
    }
    void SetRootSignature(ID3D12RootSignature* pRootSig)
    {
        m_pRootSigpRootSig;
    }
    D3D12_STATE_SUBOBJECT_TYPE Type() const
    { 
        return D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE; 
    }
    operator const D3D12_STATE_SUBOBJECT&() const { return *m_pSubobject; }
    operator ID3D12RootSignature*() const { return m_pRootSig.Get(); }
private:
    void Init()
    {
        SUBOBJECT_HELPER_BASE::Init();
        m_pRootSignullptr;
    }
    void* Data() { return m_pRootSig.GetAddressOf(); }
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_pRootSig;
};

//------------------------------------------------------------------------------------------------
class CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT 
    : public CD3DX12_STATE_OBJECT_DESC::SUBOBJECT_HELPER_BASE
{
public:
    CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT()
    {
        Init();
    }
    CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
    {
        Init();
        AddToStateObject(ContainingStateObject);
    }
    void SetRootSignature(ID3D12RootSignature* pRootSig)
    {
        m_pRootSigpRootSig;
    }
    D3D12_STATE_SUBOBJECT_TYPE Type() const
    { 
        return D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE; 
    }
    operator const D3D12_STATE_SUBOBJECT&() const { return *m_pSubobject; }
    operator ID3D12RootSignature*() const { return m_pRootSig.Get(); }
private:
    void Init()
    {
        SUBOBJECT_HELPER_BASE::Init();
        m_pRootSignullptr;
    }
    void* Data() { return m_pRootSig.GetAddressOf(); }
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_pRootSig;
};

//------------------------------------------------------------------------------------------------
class CD3DX12_STATE_OBJECT_CONFIG_SUBOBJECT 
    : public CD3DX12_STATE_OBJECT_DESC::SUBOBJECT_HELPER_BASE
{
public:
    CD3DX12_STATE_OBJECT_CONFIG_SUBOBJECT()
    {
        Init();
    }
    CD3DX12_STATE_OBJECT_CONFIG_SUBOBJECT(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
    {
        Init();
        AddToStateObject(ContainingStateObject);
    }
    void SetFlags(D3D12_STATE_OBJECT_FLAGS Flags)
    {
        m_Desc.FlagsFlags;
    }
    D3D12_STATE_SUBOBJECT_TYPE Type() const
    { 
        return D3D12_STATE_SUBOBJECT_TYPE_STATE_OBJECT_CONFIG; 
    }
    operator const D3D12_STATE_SUBOBJECT&() const { return *m_pSubobject; }
    operator const D3D12_STATE_OBJECT_CONFIG&() const { return m_Desc; }
private:
    void Init()
    {
        SUBOBJECT_HELPER_BASE::Init();
        m_Desc{};
    }
    void* Data() { return &m_Desc; }
    D3D12_STATE_OBJECT_CONFIG m_Desc;
};

//------------------------------------------------------------------------------------------------
class CD3DX12_NODE_MASK_SUBOBJECT 
    : public CD3DX12_STATE_OBJECT_DESC::SUBOBJECT_HELPER_BASE
{
public:
    CD3DX12_NODE_MASK_SUBOBJECT()
    {
        Init();
    }
    CD3DX12_NODE_MASK_SUBOBJECT(CD3DX12_STATE_OBJECT_DESC& ContainingStateObject)
    {
        Init();
        AddToStateObject(ContainingStateObject);
    }
    void SetNodeMask(UINT NodeMask)
    {
        m_Desc.NodeMaskNodeMask;
    }
    D3D12_STATE_SUBOBJECT_TYPE Type() const
    { 
        return D3D12_STATE_SUBOBJECT_TYPE_NODE_MASK; 
    }
    operator const D3D12_STATE_SUBOBJECT&() const { return *m_pSubobject; }
    operator const D3D12_NODE_MASK&() const { return m_Desc; }
private:
    void Init()
    {
        SUBOBJECT_HELPER_BASE::Init();
        m_Desc{};
    }
    void* Data() { return &m_Desc; }
    D3D12_NODE_MASK m_Desc;
};

#endif // #ifndef D3DX12_NO_STATE_OBJECT_HELPERS

#endif // defined( __cplusplus )

#endif //__D3DX12_H__
