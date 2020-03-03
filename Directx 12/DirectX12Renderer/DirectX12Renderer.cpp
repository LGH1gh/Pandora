#pragma once
#include "Renderer.h"
#include "DirectX12RenderHelper.h"

DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID)
{
	if (wicFormatGUID == GUID_WICPixelFormat128bppRGBAFloat) return DXGI_FORMAT_R32G32B32A32_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBAHalf) return DXGI_FORMAT_R16G16B16A16_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBA) return DXGI_FORMAT_R16G16B16A16_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBA) return DXGI_FORMAT_R8G8B8A8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppBGRA) return DXGI_FORMAT_B8G8R8A8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppBGR) return DXGI_FORMAT_B8G8R8X8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBA1010102XR) return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;

	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBA1010102) return DXGI_FORMAT_R10G10B10A2_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppBGRA5551) return DXGI_FORMAT_B5G5R5A1_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppBGR565) return DXGI_FORMAT_B5G6R5_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppGrayFloat) return DXGI_FORMAT_R32_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppGrayHalf) return DXGI_FORMAT_R16_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppGray) return DXGI_FORMAT_R16_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat8bppGray) return DXGI_FORMAT_R8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat8bppAlpha) return DXGI_FORMAT_A8_UNORM;

	else return DXGI_FORMAT_UNKNOWN;
}

// get a dxgi compatible wic format from another wic format
WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID)
{
	if (wicFormatGUID == GUID_WICPixelFormatBlackWhite) return GUID_WICPixelFormat8bppGray;
	else if (wicFormatGUID == GUID_WICPixelFormat1bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat2bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat4bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat8bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat2bppGray) return GUID_WICPixelFormat8bppGray;
	else if (wicFormatGUID == GUID_WICPixelFormat4bppGray) return GUID_WICPixelFormat8bppGray;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppGrayFixedPoint) return GUID_WICPixelFormat16bppGrayHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppGrayFixedPoint) return GUID_WICPixelFormat32bppGrayFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppBGR555) return GUID_WICPixelFormat16bppBGRA5551;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppBGR101010) return GUID_WICPixelFormat32bppRGBA1010102;
	else if (wicFormatGUID == GUID_WICPixelFormat24bppBGR) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat24bppRGB) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppPBGRA) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppPRGBA) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppRGB) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppBGR) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppBGRA) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppPRGBA) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppPBGRA) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppRGBFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppBGRFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBAFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppBGRAFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBHalf) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppRGBHalf) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppPRGBAFloat) return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppRGBFloat) return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppRGBAFixedPoint) return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppRGBFixedPoint) return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBE) return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppCMYK) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppCMYK) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat40bppCMYKAlpha) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat80bppCMYKAlpha) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGB) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGB) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppPRGBAHalf) return GUID_WICPixelFormat64bppRGBAHalf;

	else return GUID_WICPixelFormatDontCare;
}

// get the number of bits per pixel for a dxgi format
int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat)
{
	if (dxgiFormat == DXGI_FORMAT_R32G32B32A32_FLOAT) return 128;
	else if (dxgiFormat == DXGI_FORMAT_R16G16B16A16_FLOAT) return 64;
	else if (dxgiFormat == DXGI_FORMAT_R16G16B16A16_UNORM) return 64;
	else if (dxgiFormat == DXGI_FORMAT_R8G8B8A8_UNORM) return 32;
	else if (dxgiFormat == DXGI_FORMAT_B8G8R8A8_UNORM) return 32;
	else if (dxgiFormat == DXGI_FORMAT_B8G8R8X8_UNORM) return 32;
	else if (dxgiFormat == DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM) return 32;

	else if (dxgiFormat == DXGI_FORMAT_R10G10B10A2_UNORM) return 32;
	else if (dxgiFormat == DXGI_FORMAT_B5G5R5A1_UNORM) return 16;
	else if (dxgiFormat == DXGI_FORMAT_B5G6R5_UNORM) return 16;
	else if (dxgiFormat == DXGI_FORMAT_R32_FLOAT) return 32;
	else if (dxgiFormat == DXGI_FORMAT_R16_FLOAT) return 16;
	else if (dxgiFormat == DXGI_FORMAT_R16_UNORM) return 16;
	else if (dxgiFormat == DXGI_FORMAT_R8_UNORM) return 8;
	else if (dxgiFormat == DXGI_FORMAT_A8_UNORM) return 8;
}

int LoadImageDataFromFile(BYTE** imageData, D3D12_RESOURCE_DESC& resourceDescription, LPCWSTR filename, int& bytesPerRow)
{
	static ComPtr<IWICImagingFactory> wicFactory;
	ComPtr<IWICBitmapDecoder> wicDecoder;
	ComPtr<IWICBitmapFrameDecode> wicFrame;
	ComPtr<IWICFormatConverter> wicConverter;

	bool imageConverted = false;

	if (wicFactory == nullptr)
	{
		ThrowIfFailed(CoInitialize(NULL));

		ThrowIfFailed(CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&wicFactory)
		));
	}

	ThrowIfFailed(wicFactory->CreateDecoderFromFilename(
		filename,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&wicDecoder
	));
	ThrowIfFailed(wicDecoder->GetFrame(0, &wicFrame));

	WICPixelFormatGUID pixelFormat;
	ThrowIfFailed(wicFrame->GetPixelFormat(&pixelFormat));
	UINT textureWidth, textureHeight;
	ThrowIfFailed(wicFrame->GetSize(&textureWidth, &textureHeight));

	DXGI_FORMAT dxgiFormat = GetDXGIFormatFromWICFormat(pixelFormat);

	if (dxgiFormat == DXGI_FORMAT_UNKNOWN)
	{
		WICPixelFormatGUID convertToPixelFormat = GetConvertToWICFormat(pixelFormat);
		if (convertToPixelFormat == GUID_WICPixelFormatDontCare) return 0;

		dxgiFormat = GetDXGIFormatFromWICFormat(convertToPixelFormat);

		ThrowIfFailed(wicFactory->CreateFormatConverter(&wicConverter));

		BOOL canConvert = FALSE;
		ThrowIfFailed(wicConverter->CanConvert(pixelFormat, convertToPixelFormat, &canConvert));
		if (!canConvert) return 0;

		ThrowIfFailed(wicConverter->Initialize(wicFrame.Get(), convertToPixelFormat, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom));

		imageConverted = true;
	}

	int bitsPerPixel = GetDXGIFormatBitsPerPixel(dxgiFormat);
	bytesPerRow = (textureWidth * bitsPerPixel) / 8;
	int imageSize = bytesPerRow * textureHeight;

	*imageData = (BYTE*)malloc(imageSize);

	if (imageConverted)
	{
		ThrowIfFailed(wicConverter->CopyPixels(0, bytesPerRow, imageSize, *imageData));
	}
	else
	{
		ThrowIfFailed(wicFrame->CopyPixels(0, bytesPerRow, imageSize, *imageData));
	}

	resourceDescription = {};
	resourceDescription.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDescription.Alignment = 0; // may be 0, 4KB, 64KB, or 4MB. 0 will let runtime decide between 64KB and 4MB (4MB for multi-sampled textures)
	resourceDescription.Width = textureWidth; // width of the texture
	resourceDescription.Height = textureHeight; // height of the texture
	resourceDescription.DepthOrArraySize = 1; // if 3d image, depth of 3d image. Otherwise an array of 1D or 2D textures (we only have one image, so we set 1)
	resourceDescription.MipLevels = 1; // Number of mipmaps. We are not generating mipmaps for this texture, so we have only one level
	resourceDescription.Format = dxgiFormat; // This is the dxgi format of the image (format of the pixels)
	resourceDescription.SampleDesc.Count = 1; // This is the number of samples per pixel, we just want 1 sample
	resourceDescription.SampleDesc.Quality = 0; // The quality level of the samples. Higher is better quality, but worse performance
	resourceDescription.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; // The arrangement of the pixels. Setting to unknown lets the driver choose the most efficient one
	resourceDescription.Flags = D3D12_RESOURCE_FLAG_NONE; // no flags

	return imageSize;
}


static const DXGI_FORMAT FORMATS[] =
{
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_R32G32B32A32_TYPELESS,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_UINT,
	DXGI_FORMAT_R32G32B32A32_SINT,
	DXGI_FORMAT_R32G32B32_TYPELESS,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32_UINT,
	DXGI_FORMAT_R32G32B32_SINT,
	DXGI_FORMAT_R16G16B16A16_TYPELESS,
	DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_R16G16B16A16_UNORM,
	DXGI_FORMAT_R16G16B16A16_UINT,
	DXGI_FORMAT_R16G16B16A16_SNORM,
	DXGI_FORMAT_R16G16B16A16_SINT,
	DXGI_FORMAT_R32G32_TYPELESS,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32_UINT,
	DXGI_FORMAT_R32G32_SINT,
	DXGI_FORMAT_R32G8X24_TYPELESS,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,
	DXGI_FORMAT_R10G10B10A2_TYPELESS,
	DXGI_FORMAT_R10G10B10A2_UNORM,
	DXGI_FORMAT_R10G10B10A2_UINT,
	DXGI_FORMAT_R11G11B10_FLOAT,
	DXGI_FORMAT_R8G8B8A8_TYPELESS,
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
	DXGI_FORMAT_R8G8B8A8_UINT,
	DXGI_FORMAT_R8G8B8A8_SNORM,
	DXGI_FORMAT_R8G8B8A8_SINT,
	DXGI_FORMAT_R16G16_TYPELESS,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R16G16_UNORM,
	DXGI_FORMAT_R16G16_UINT,
	DXGI_FORMAT_R16G16_SNORM,
	DXGI_FORMAT_R16G16_SINT,
	DXGI_FORMAT_R32_TYPELESS,
	DXGI_FORMAT_D32_FLOAT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32_UINT,
	DXGI_FORMAT_R32_SINT,
	DXGI_FORMAT_R24G8_TYPELESS,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT,
	DXGI_FORMAT_R8G8_TYPELESS,
	DXGI_FORMAT_R8G8_UNORM,
	DXGI_FORMAT_R8G8_UINT,
	DXGI_FORMAT_R8G8_SNORM,
	DXGI_FORMAT_R8G8_SINT,
	DXGI_FORMAT_R16_TYPELESS,
	DXGI_FORMAT_R16_FLOAT,
	DXGI_FORMAT_D16_UNORM,
	DXGI_FORMAT_R16_UNORM,
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R16_SNORM,
	DXGI_FORMAT_R16_SINT,
	DXGI_FORMAT_R8_TYPELESS,
	DXGI_FORMAT_R8_UNORM,
	DXGI_FORMAT_R8_UINT,
	DXGI_FORMAT_R8_SNORM,
	DXGI_FORMAT_R8_SINT,
	DXGI_FORMAT_A8_UNORM,
	DXGI_FORMAT_R1_UNORM,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP,
	DXGI_FORMAT_R8G8_B8G8_UNORM,
	DXGI_FORMAT_G8R8_G8B8_UNORM,
	DXGI_FORMAT_BC1_TYPELESS,
	DXGI_FORMAT_BC1_UNORM,
	DXGI_FORMAT_BC1_UNORM_SRGB,
	DXGI_FORMAT_BC2_TYPELESS,
	DXGI_FORMAT_BC2_UNORM,
	DXGI_FORMAT_BC2_UNORM_SRGB,
	DXGI_FORMAT_BC3_TYPELESS,
	DXGI_FORMAT_BC3_UNORM,
	DXGI_FORMAT_BC3_UNORM_SRGB,
	DXGI_FORMAT_BC4_TYPELESS,
	DXGI_FORMAT_BC4_UNORM,
	DXGI_FORMAT_BC4_SNORM,
	DXGI_FORMAT_BC5_TYPELESS,
	DXGI_FORMAT_BC5_UNORM,
	DXGI_FORMAT_BC5_SNORM,
	DXGI_FORMAT_B5G6R5_UNORM,
	DXGI_FORMAT_B5G5R5A1_UNORM,
	DXGI_FORMAT_B8G8R8A8_UNORM,
	DXGI_FORMAT_B8G8R8X8_UNORM,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,
	DXGI_FORMAT_B8G8R8A8_TYPELESS,
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
	DXGI_FORMAT_B8G8R8X8_TYPELESS,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,
	DXGI_FORMAT_BC6H_TYPELESS,
	DXGI_FORMAT_BC6H_UF16,
	DXGI_FORMAT_BC6H_SF16,
	DXGI_FORMAT_BC7_TYPELESS,
	DXGI_FORMAT_BC7_UNORM,
	DXGI_FORMAT_BC7_UNORM_SRGB,
	DXGI_FORMAT_AYUV,
	DXGI_FORMAT_Y410,
	DXGI_FORMAT_Y416,
	DXGI_FORMAT_NV12,
	DXGI_FORMAT_P010,
	DXGI_FORMAT_P016,
	DXGI_FORMAT_420_OPAQUE,
	DXGI_FORMAT_YUY2,
	DXGI_FORMAT_Y210,
	DXGI_FORMAT_Y216,
	DXGI_FORMAT_NV11,
	DXGI_FORMAT_AI44,
	DXGI_FORMAT_IA44,
	DXGI_FORMAT_P8,
	DXGI_FORMAT_A8P8,
	DXGI_FORMAT_B4G4R4A4_UNORM,

	DXGI_FORMAT_P208,
	DXGI_FORMAT_V208,
	DXGI_FORMAT_V408,


	DXGI_FORMAT_FORCE_UINT
};

struct SDescriptorHeap
{
	ComPtr<ID3D12DescriptorHeap> m_descHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpuDescStart;
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpuDescStart;
	UINT8* m_pData;
	size_t m_descSize;

	void Init(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = type;
		desc.NumDescriptors = numDescriptors;
		desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_descHeap)));

		m_cpuDescStart = m_descHeap->GetCPUDescriptorHandleForHeapStart();
		m_gpuDescStart = m_descHeap->GetGPUDescriptorHandleForHeapStart();
		m_descSize = device->GetDescriptorHandleIncrementSize(type);
		m_pData = nullptr;

	}

	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT offset) const
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = m_cpuDescStart;
		handle.ptr += offset * m_descSize;
		return handle;
	}

	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(UINT64 offset) const
	{
		D3D12_GPU_DESCRIPTOR_HANDLE handle = m_gpuDescStart;
		handle.ptr += offset * m_descSize;
		return handle;
	}
};

struct SCommandAllocator
{
	ComPtr<ID3D12CommandAllocator> commandAllocator;

	void Init(ID3D12Device* device)
	{
		ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
	}
};

struct SContext
{
	ComPtr<ID3D12GraphicsCommandList> commandList;
};

struct SBlendState
{
	D3D12_BLEND_DESC blendDesc;
};

struct SDevice
{
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<IDXGISwapChain3> swapChain;
	D3D_FEATURE_LEVEL featureLevel;

	Context mainContext;
	CommandAllocator commandAllocators[FrameCount];

	DescriptorHeap srvHeap;
	DescriptorHeap rtvHeap;
	DescriptorHeap dsvHeap;
	DescriptorHeap samplerHeap;

	UINT msaaSupportMask;

	BlendState blendState;

	UINT frameIndex;
	HANDLE fenceEvent;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceValues[FrameCount];

	RenderPass backBufferRenderPass;
	Texture backBuffer[FrameCount];
	RenderSetup backBufferRenderSetup;

	SDevice() {
		mainContext = new SContext();
		for (size_t n = 0; n < FrameCount; n++)
		{
			commandAllocators[n] = new SCommandAllocator();
		}
		srvHeap = new SDescriptorHeap();
		rtvHeap = new SDescriptorHeap();
		dsvHeap = new SDescriptorHeap();
		samplerHeap = new SDescriptorHeap();
		blendState = new SBlendState();
	}
};

struct STexture
{
	UINT width;
	UINT height;
	UINT depth;
	UINT slices;
	UINT mipLevels;
	TextureType m_Type;
	ImageFormat m_Format;
};

struct SRenderPass
{
	DXGI_FORMAT depthFormat;
	UINT colorTargetCount;
	DXGI_FORMAT colorFormats[1];
	RenderPassFlags flags;
	UINT msaaSamples;
};

struct SRootSignature
{
	ComPtr<ID3D12RootSignature> rootSignature;
};

struct SResourceTable
{
	UINT m_Offset;
	UINT m_Size;
};

struct SSamplerTable
{
	UINT m_Offset;
	UINT m_Count;
};

struct SRenderSetup
{
	ComPtr<ID3D12Resource> renderTargets[FrameCount];
};

struct SPipeline
{
	ComPtr<ID3D12PipelineState> pipeline;
	D3D12_PRIMITIVE_TOPOLOGY primitiveTopology;
};

struct SVertexSetup
{
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
};

struct SBuffer
{
	ComPtr<ID3D12Resource> buffer;
	UINT size;
	HeapType heapType;
};

struct SDepthStencil
{
	ComPtr<ID3D12Resource> depthStencilBuffer;
	ComPtr<ID3D12DescriptorHeap> dsDescriptorHeap;
};


void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = adapter.Detach();
}

Device CreateDevice(DeviceParams& params, HWND hwnd)
{
	Device device = new SDevice();
	UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif

	ComPtr<IDXGIFactory4> factory;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

	if (params.useWarpDevice)
	{
		ComPtr<IDXGIAdapter> warpAdapter;
		ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			warpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&device->device)
		));
	}
	else
	{
		ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(factory.Get(), &hardwareAdapter);

		ThrowIfFailed(D3D12CreateDevice(
			hardwareAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&device->device)
		));
	}

	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	ThrowIfFailed(device->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&device->commandQueue)));

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.Width = params.width;
	swapChainDesc.Height = params.height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(factory->CreateSwapChainForHwnd(
		device->commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
		hwnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	));

	// This sample does not support fullscreen transitions.
	ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

	ThrowIfFailed(swapChain.As(&device->swapChain));
	device->frameIndex = device->swapChain->GetCurrentBackBufferIndex();

	//device->srvHeap = new SDescriptorHeap();
	//device->srvHeap->Init(device->device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 64, true);
	device->rtvHeap->Init(device->device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FrameCount, false);
	//device->dsvHeap->Init(device->device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 16, false);
	//device->samplerHeap->Init(device->device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 16, true);



	for (UINT n = 0; n < FrameCount; n++)
	{
		device->commandAllocators[n]->Init(device->device.Get());
	}

	ThrowIfFailed(device->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, device->commandAllocators[device->frameIndex]->commandAllocator.Get(), nullptr, IID_PPV_ARGS(&device->mainContext->commandList)));
	device->backBufferRenderPass = CreateRenderPass(device, IMAGE_FORMAT_R8G8B8A8_UNORM, IMAGE_FORMAT_D32_FLOAT, FINAL_PRESENT);
	device->backBufferRenderSetup = CreateRenderSetup(device);

	{
		ThrowIfFailed(device->device->CreateFence(device->fenceValues[device->frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&device->fence)));
		device->fenceValues[device->frameIndex]++;
		device->fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (device->fenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}
		WaitForGPU(device);

	}
	return device;
}

RenderSetup CreateRenderSetup(Device device)
{
	RenderSetup setup = new SRenderSetup();
	for (UINT n = 0; n < FrameCount; n++)
	{
		ThrowIfFailed(device->swapChain->GetBuffer(n, IID_PPV_ARGS(&setup->renderTargets[n])));
		device->device->CreateRenderTargetView(setup->renderTargets[n].Get(), nullptr, device->rtvHeap->GetCPUHandle(n));
	}
	return setup;
}

BlendState CreateBlendState(Blend src, Blend dst, BlendOperator mode, UINT mask, bool alphaToCoverageEnable)
{
	BlendState blendState = new SBlendState();
	bool blendEnble = (src != BLEND_ONE || dst != BLEND_ZERO);

	blendState->blendDesc.AlphaToCoverageEnable = alphaToCoverageEnable;
	blendState->blendDesc.IndependentBlendEnable = false;

	static const D3D12_BLEND blend_factors[] =
	{
		D3D12_BLEND_ZERO,
		D3D12_BLEND_ONE,
		D3D12_BLEND_SRC_COLOR,
		D3D12_BLEND_INV_SRC_COLOR,
		D3D12_BLEND_SRC_ALPHA,
		D3D12_BLEND_INV_SRC_ALPHA,
		D3D12_BLEND_DEST_COLOR,
		D3D12_BLEND_INV_DEST_COLOR,
		D3D12_BLEND_DEST_ALPHA,
		D3D12_BLEND_INV_DEST_ALPHA,
	};

	static const D3D12_BLEND_OP blend_modes[] =
	{
		D3D12_BLEND_OP_ADD,
		D3D12_BLEND_OP_SUBTRACT,
		D3D12_BLEND_OP_REV_SUBTRACT,
		D3D12_BLEND_OP_MIN,
		D3D12_BLEND_OP_MAX,
	};

	for (int i = 0; i < 8; ++i)
	{
		blendState->blendDesc.RenderTarget[i].BlendEnable = blendEnble;

		blendState->blendDesc.RenderTarget[i].BlendOp = blend_modes[mode];
		blendState->blendDesc.RenderTarget[i].BlendOpAlpha = blend_modes[mode];
		blendState->blendDesc.RenderTarget[i].SrcBlend = blend_factors[src];
		blendState->blendDesc.RenderTarget[i].SrcBlendAlpha = blend_factors[src];
		blendState->blendDesc.RenderTarget[i].DestBlend = blend_factors[dst];
		blendState->blendDesc.RenderTarget[i].DestBlendAlpha = blend_factors[dst];

		blendState->blendDesc.RenderTarget[i].RenderTargetWriteMask = (UINT8)mask;
	}

	return blendState;
}

RootSignature CreateRootSignature(SDevice* device, UINT cbvCount, UINT srvCount)
{
	RootSignature rootSignature = new SRootSignature();

	XD3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_1(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	UINT totalCount = cbvCount + srvCount;
	if (totalCount)
	{
		if (cbvCount != 0 && srvCount != 0)
		{
			XD3D12_DESCRIPTOR_RANGE1 cbvTableRanges[1];
			cbvTableRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, cbvCount, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);
			XD3D12_DESCRIPTOR_RANGE1 srvTableRanges[1];
			srvTableRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, srvCount, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);

			XD3D12_ROOT_PARAMETER1 rootParameters[2];
			rootParameters[0].InitAsDescriptorTable(1, &cbvTableRanges[0], D3D12_SHADER_VISIBILITY_ALL);
			rootParameters[1].InitAsDescriptorTable(1, &srvTableRanges[0], D3D12_SHADER_VISIBILITY_ALL);

			D3D12_STATIC_SAMPLER_DESC sampler = {};
			sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			sampler.MipLODBias = 0;
			sampler.MaxAnisotropy = 0;
			sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			sampler.MinLOD = 0.0f;
			sampler.MaxLOD = D3D12_FLOAT32_MAX;
			sampler.ShaderRegister = 0;
			sampler.RegisterSpace = 0;
			sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

			D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &sampler, rootSignatureFlags);
		}
		else if (cbvCount != 0)
		{
			XD3D12_DESCRIPTOR_RANGE1 cbvTableRanges[1];
			cbvTableRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, cbvCount, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);

			XD3D12_ROOT_PARAMETER1 rootParameters[1];
			rootParameters[0].InitAsDescriptorTable(1, &cbvTableRanges[0], D3D12_SHADER_VISIBILITY_ALL);

			D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);
		}
		else
		{
			XD3D12_DESCRIPTOR_RANGE1 srvTableRanges[1];
			srvTableRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, srvCount, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);
		
			XD3D12_ROOT_PARAMETER1 rootParameters[1];
			rootParameters[0].InitAsDescriptorTable(1, &srvTableRanges[0], D3D12_SHADER_VISIBILITY_ALL);

			D3D12_STATIC_SAMPLER_DESC sampler = {};
			sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			sampler.MipLODBias = 0;
			sampler.MaxAnisotropy = 0;
			sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			sampler.MinLOD = 0.0f;
			sampler.MaxLOD = D3D12_FLOAT32_MAX;
			sampler.ShaderRegister = 0;
			sampler.RegisterSpace = 0;
			sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

			D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &sampler, rootSignatureFlags);
		}
	}

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_1, &signature, &error));
	ThrowIfFailed(device->device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature->rootSignature)));

	return rootSignature;
}

Blob CreateShaderFromFile(ShaderType shaderType, LPCWSTR filePath, std::string entryPoint, UINT flags)
{
	Blob blob = new SBlob();
	ID3DBlob* shader;
	switch (shaderType) {
	case SHADER_TYPE_VERTEX_SHADER:
		ThrowIfFailed(D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint.c_str(), "vs_5_0", flags, 0, &shader, nullptr));
		break;
	case SHADER_TYPE_PIXEL_SHADER:
		ThrowIfFailed(D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint.c_str(), "ps_5_0", flags, 0, &shader, nullptr));
		break;
	case SHADER_TYPE_HULL_SHADER:
		ThrowIfFailed(D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint.c_str(), "hs_5_0", flags, 0, &shader, nullptr));
		break;
	case SHADER_TYPE_DOMAIN_SHADER:
		ThrowIfFailed(D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint.c_str(), "ds_5_0", flags, 0, &shader, nullptr));
		break;
	case SHADER_TYPE_COMPUTE_SHADER:
		ThrowIfFailed(D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint.c_str(), "cs_5_0", flags, 0, &shader, nullptr));
		break;
	default:
		ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		return nullptr;
	}
	blob->address = shader->GetBufferPointer();
	blob->size = shader->GetBufferSize();

	return blob;
}

RenderPass CreateRenderPass(Device device, ImageFormat colorFormat, ImageFormat depthFormat, RenderPassFlags flags, UINT msaaSampler)
{
	RenderPass renderPass = new SRenderPass();
	renderPass->depthFormat = FORMATS[depthFormat];
	renderPass->colorTargetCount = 1;
	renderPass->colorFormats[0] = FORMATS[colorFormat];
	renderPass->msaaSamples = msaaSampler;
	renderPass->flags = flags;

	return renderPass;
}

Pipeline CreateGraphicsPipeline(SDevice* device, PipelineParams& params)
{
	Pipeline pipeline = new SPipeline();

	static const DXGI_FORMAT dxgi_format[] =
	{
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R32G32B32A32_UINT,
		DXGI_FORMAT_R16G16B16A16_UINT,

		DXGI_FORMAT_R32G32B32_FLOAT,
		DXGI_FORMAT_R32G32B32_UINT,

		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_R16G16_FLOAT,
		DXGI_FORMAT_R32G32_UINT,
		DXGI_FORMAT_R16G16_UINT,

		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_R32_UINT,
	};

	static const UINT dxgi_format_size[] =
	{
		16, 8, 16, 8,
		12, 12,
		8, 4, 8, 4,
		4, 4
	};

	D3D12_INPUT_ELEMENT_DESC inputElementDesc[16];
	UINT offsets[4] = {};

	for (UINT n = 0; n < params.attributeCount; ++n)
	{
		UINT stream = params.attributes[n].stream;
		AttributeFormat format = params.attributes[n].format;

		inputElementDesc[n].SemanticName = params.attributes[n].name;
		inputElementDesc[n].SemanticIndex = 0;
		inputElementDesc[n].Format = dxgi_format[format];
		inputElementDesc[n].InputSlot = stream;
		inputElementDesc[n].AlignedByteOffset = offsets[stream];
		inputElementDesc[n].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		inputElementDesc[n].InstanceDataStepRate = 0;

		offsets[stream] += dxgi_format_size[format];
	}

	XD3D12_BLEND_DESC blendDesc(D3D12_DEFAULT);

	XD3D12_DEPTH_STENCIL_DESC depthStencilDesc(D3D12_DEFAULT);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = params.rootSignature->rootSignature.Get();
	psoDesc.InputLayout = { inputElementDesc, params.attributeCount };
	psoDesc.VS.BytecodeLength = params.vs.size;
	psoDesc.VS.pShaderBytecode = params.vs.address;
	psoDesc.PS.BytecodeLength = params.ps.size;
	psoDesc.PS.pShaderBytecode = params.ps.address;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.RasterizerState.CullMode = (D3D12_CULL_MODE)(params.cullMode + 1);
	psoDesc.RasterizerState.DepthClipEnable = true;
	psoDesc.RasterizerState.MultisampleEnable = true;
	//psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xF;
	//if (params.blendState)
	//{
	//	psoDesc.BlendState = params.blendState->blendDesc;
	//}
	psoDesc.BlendState = blendDesc;
	//psoDesc.DepthStencilState.DepthEnable = params.depthTest;
	//psoDesc.DepthStencilState.DepthWriteMask = params.depthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
	//psoDesc.DepthStencilState.DepthFunc = (D3D12_COMPARISON_FUNC)(params.depthFunc + 1);
	psoDesc.DepthStencilState = depthStencilDesc;
	const UINT32 renderTargetCount = params.renderPass->colorTargetCount;
	psoDesc.NumRenderTargets = renderTargetCount;
	memcpy(psoDesc.RTVFormats, params.renderPass->colorFormats, renderTargetCount * sizeof(DXGI_FORMAT));
	psoDesc.DSVFormat = params.renderPass->depthFormat;

	psoDesc.SampleDesc.Count = params.renderPass->msaaSamples;
	psoDesc.SampleMask = UINT_MAX;

	psoDesc.PrimitiveTopologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)((params.primitiveTopologyType + 3) >> 1);
	pipeline->primitiveTopology = (D3D12_PRIMITIVE_TOPOLOGY)(params.primitiveTopologyType + 1);

	ThrowIfFailed(device->device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipeline->pipeline)));
	
	return pipeline;
}

VertexSetup CreateVertexSetup(SBuffer* vertexBuffer, UINT vertexBufferStride, SBuffer* indexBuffer, UINT indexBufferStride)
{
	VertexSetup vertexSetup = new SVertexSetup();
	if (indexBuffer)
	{
		vertexSetup->indexBufferView.BufferLocation = indexBuffer->buffer->GetGPUVirtualAddress();
		vertexSetup->indexBufferView.SizeInBytes = indexBuffer->size;
		vertexSetup->indexBufferView.Format = indexBufferStride == sizeof(UINT16) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	}

	if (vertexBuffer)
	{
		vertexSetup->vertexBufferView.BufferLocation = vertexBuffer->buffer->GetGPUVirtualAddress();
		vertexSetup->vertexBufferView.SizeInBytes = vertexBuffer->size;
		vertexSetup->vertexBufferView.StrideInBytes = vertexBufferStride;
	}

	return vertexSetup;
}

Buffer CreateBuffer(Device device, const BufferParams& params)
{
	Buffer buffer = new SBuffer();;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = (D3D12_HEAP_TYPE)(params.heapType + 1);
	D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = params.size;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_RESOURCE_STATES resourceState = (D3D12_RESOURCE_STATES)params.resourceState;

	ID3D12Resource* d3dBuffer = nullptr;
	ThrowIfFailed(device->device->CreateCommittedResource(&heapProp, flags, &desc, resourceState, nullptr, IID_PPV_ARGS(&d3dBuffer)));

	buffer->buffer = d3dBuffer;
	buffer->size = params.size;
	buffer->heapType = params.heapType;

	return buffer;
}

Buffer CreateVertexBuffer(SDevice* device, const void* pData, UINT size)
{
	BufferParams vertexBufferParams(size, HEAP_TYPE_DEFAULT, RESOURCE_STATE_COPY_DEST);
	Buffer vertexBuffer = CreateBuffer(device, vertexBufferParams);
	BufferParams vertexUploadBufferParams(size, HEAP_TYPE_UPLOAD, RESOURCE_STATE_GENERIC_READ);
	Buffer vertexUploadBuffer = CreateBuffer(device, vertexUploadBufferParams);
	SubresourceParams vertexData = { pData, size, size };
	Subresource(device, vertexBuffer, vertexUploadBuffer, vertexData);
	return vertexBuffer;
}

Buffer CreateIndexBuffer(SDevice* device, const void* pData, UINT size)
{
	BufferParams indexBufferParams(size, HEAP_TYPE_DEFAULT, RESOURCE_STATE_COPY_DEST);
	Buffer indexBuffer = CreateBuffer(device, indexBufferParams);
	BufferParams indexUploadBufferParams(size, HEAP_TYPE_UPLOAD, RESOURCE_STATE_GENERIC_READ);
	Buffer indexUploadBuffer = CreateBuffer(device, indexUploadBufferParams);
	SubresourceParams indexData = { pData, size, size };
	Subresource(device, indexBuffer, indexUploadBuffer, indexData);
	return indexBuffer;
}

DescriptorHeap CreateConstantBuffer(SDevice* device, const void* pData, UINT count, UINT size)
{
	DescriptorHeap result = new SDescriptorHeap();
	result->Init(device->device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, count, true);


	BufferParams constantBufferParams((size + 255) & ~255, HEAP_TYPE_UPLOAD, RESOURCE_STATE_GENERIC_READ);
	Buffer constantBuffer = CreateBuffer(device, constantBufferParams);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constantBuffer->buffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (size + 255) & ~255;
	device->device->CreateConstantBufferView(&cbvDesc, result->GetCPUHandle(0));

	XD3D12_RANGE readRange(0, 0);
	ThrowIfFailed(constantBuffer->buffer->Map(0, &readRange, reinterpret_cast<void**>(&result->m_pData)));
	memcpy(result->m_pData, &pData, size);
	return result;
}

DescriptorHeap CreateTexture(SDevice* device)
{
	SDescriptorHeap* result = new SDescriptorHeap();
	ComPtr<ID3D12DescriptorHeap> mainDescriptorHeap;

	ComPtr<ID3D12Resource> textureBuffer;
	ComPtr<ID3D12Resource> textureBufferUploadHeap;

	D3D12_RESOURCE_DESC textureDesc;
	int imageBytesPerRow;
	BYTE* imageData;

	int imageSize = LoadImageDataFromFile(&imageData, textureDesc, L"D:\\Pandora\\Directx 12\\x64\\Debug\\braynzar.jpg", imageBytesPerRow);

	if (imageSize <= 0)
	{
		ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}

	ThrowIfFailed(device->device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&textureBuffer)
	));

	UINT64 textureUploadBufferSize;
	device->device->GetCopyableFootprints(&textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);

	ThrowIfFailed(device->device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Buffer(textureUploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureBufferUploadHeap)
	));

	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = &imageData[0];
	textureData.RowPitch = imageBytesPerRow;
	textureData.SlicePitch = imageBytesPerRow * textureDesc.Height;

	UpdateSubresources(device->mainContext->commandList.Get(), textureBuffer.Get(), textureBufferUploadHeap.Get(), 0, 0, 1, &textureData);
	device->mainContext->commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(textureBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	ThrowIfFailed(device->device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mainDescriptorHeap)));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	device->device->CreateShaderResourceView(textureBuffer.Get(), &srvDesc, mainDescriptorHeap->GetCPUDescriptorHandleForHeapStart());


	result->m_descHeap = mainDescriptorHeap;
	result->m_cpuDescStart = mainDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	result->m_gpuDescStart = mainDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	
	return result;
}


DepthStencil CreateDepthStencil(SDevice* device, UINT width, UINT height)
{
	DepthStencil depthStencil = new SDepthStencil();

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ThrowIfFailed(device->device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&depthStencil->dsDescriptorHeap)));

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	device->device->CreateCommittedResource(
		&XD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&XD3D12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, width, height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(&depthStencil->depthStencilBuffer)
	);
	depthStencil->dsDescriptorHeap->SetName(L"Depth/Stencil Resource Heap");

	device->device->CreateDepthStencilView(depthStencil->depthStencilBuffer.Get(), &depthStencilDesc, depthStencil->dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	return depthStencil;
}

void WaitForGPU(SDevice* device)
{
	// Schedule a Signal command in the queue.
	ThrowIfFailed(device->commandQueue->Signal(device->fence.Get(), device->fenceValues[device->frameIndex]));

	// Wait until the fence has been processed.
	ThrowIfFailed(device->fence->SetEventOnCompletion(device->fenceValues[device->frameIndex], device->fenceEvent));
	WaitForSingleObjectEx(device->fenceEvent, INFINITE, FALSE);

	// Increment the fence value for the current frame.
	device->fenceValues[device->frameIndex]++;
}

void MoveToNextFrame(SDevice* device)
{
	// Schedule a Signal command in the queue.
	const UINT64 currentFenceValue = device->fenceValues[device->frameIndex];
	ThrowIfFailed(device->commandQueue->Signal(device->fence.Get(), currentFenceValue));

	// Update the frame index.
	device->frameIndex = device->swapChain->GetCurrentBackBufferIndex();

	// If the next frame is not ready to be rendered yet, wait until it is ready.
	if (device->fence->GetCompletedValue() < device->fenceValues[device->frameIndex])
	{
		ThrowIfFailed(device->fence->SetEventOnCompletion(device->fenceValues[device->frameIndex], device->fenceEvent));
		WaitForSingleObjectEx(device->fenceEvent, INFINITE, FALSE);
	}

	// Set the fence value for the next frame.
	device->fenceValues[device->frameIndex] = currentFenceValue + 1;
}

void Subresource(SDevice* device, SBuffer* destinationResource, SBuffer* intermediate, SubresourceParams& params)
{
	D3D12_SUBRESOURCE_DATA data;
	data.pData = params.pData;
	data.RowPitch = params.rowPitch;
	data.SlicePitch = params.slicePitch;
	UpdateSubresources<1>(device->mainContext->commandList.Get(), destinationResource->buffer.Get(), intermediate->buffer.Get(), 0, 0, 1, &data);
	device->mainContext->commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(destinationResource->buffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
}

void UpdateBuffer(SDescriptorHeap* descriptor, void* pData, UINT size)
{
	memcpy(descriptor->m_pData, pData, size);
}

void Reset(SDevice* device, SPipeline* pipeline)
{
	ThrowIfFailed(device->commandAllocators[device->frameIndex]->commandAllocator->Reset());
	ThrowIfFailed(device->mainContext->commandList->Reset(device->commandAllocators[device->frameIndex]->commandAllocator.Get(), pipeline->pipeline.Get()));
}

void BeginRenderPass(SDevice* device, const DeviceParams& params, SDepthStencil* depthStencil, const float* clearColor)
{
	D3D12_VIEWPORT vp = { 0.0f, 0.0f, static_cast<float>(params.width), static_cast<float>(params.height), 0.0f, 1.0f };
	D3D12_RECT sr = { 0, 0, static_cast<LONG>(params.width), static_cast<LONG>(params.height) };
	device->mainContext->commandList->RSSetViewports(1, &vp);
	device->mainContext->commandList->RSSetScissorRects(1, &sr);
	device->mainContext->commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(device->backBufferRenderSetup->renderTargets[device->frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(device->rtvHeap->GetCPUHandle(device->frameIndex));
	if (depthStencil == nullptr)
	{
		device->mainContext->commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
		device->mainContext->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	}
	else
	{
		XD3D12_CPU_DESCRIPTOR_HANDLE dsvHandle(depthStencil->dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		device->mainContext->commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		device->mainContext->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		device->mainContext->commandList->ClearDepthStencilView(depthStencil->dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
	
}

void SetGraphicsRootSignature(SDevice* device, SRootSignature* rootSignature)
{
	device->mainContext->commandList->SetGraphicsRootSignature(rootSignature->rootSignature.Get());
}

void SetConstantBuffer(SDevice* device, SDescriptorHeap* descriptor)
{
	ID3D12DescriptorHeap* ppHeaps[] = { descriptor->m_descHeap.Get() };
	device->mainContext->commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	device->mainContext->commandList->SetGraphicsRootDescriptorTable(0, descriptor->GetGPUHandle(0));
}

void SetTextureBuffer(SDevice* device, SDescriptorHeap* descriptor)
{
	ID3D12DescriptorHeap* ppHeaps[] = { descriptor->m_descHeap.Get() };
	device->mainContext->commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	device->mainContext->commandList->SetGraphicsRootDescriptorTable(1, descriptor->GetGPUHandle(0));
}

void SetPipeline(SDevice* device, SPipeline* pipeline)
{
	device->mainContext->commandList->SetPipelineState(pipeline->pipeline.Get());
	device->mainContext->commandList->IASetPrimitiveTopology(pipeline->primitiveTopology);
}

void SetVertexSetup(SDevice* device, SVertexSetup* vertexSetup)
{
	if (vertexSetup->indexBufferView.BufferLocation)
	{
		device->mainContext->commandList->IASetIndexBuffer(&vertexSetup->indexBufferView);
	}
	if (vertexSetup->vertexBufferView.BufferLocation)
	{
		device->mainContext->commandList->IASetVertexBuffers(0, 1, &vertexSetup->vertexBufferView);
	}
}

void Draw(SDevice* device, UINT start, UINT count)
{
	device->mainContext->commandList->DrawInstanced(count, 1, start, 0);
}

void DrawIndexed(SDevice* device, UINT start, UINT count)
{
	device->mainContext->commandList->DrawIndexedInstanced(count, 1, start, 0, 0);
}

void DrawIndexInstanced(SDevice* device, UINT StartIndexLocation, UINT IndexCountPerInstance, UINT StartInstanceLocation, UINT InstanceCount)
{
	device->mainContext->commandList->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, 0, StartInstanceLocation);
}

void EndRenderPass(SDevice* device)
{
	device->mainContext->commandList->ResourceBarrier(1, &XD3D12_RESOURCE_BARRIER::Transition(device->backBufferRenderSetup->renderTargets[device->frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void ExecuteCommand(SDevice* device)
{
	ThrowIfFailed(device->mainContext->commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { device->mainContext->commandList.Get() };
	device->commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

void Present(SDevice* device)
{
	device->swapChain->Present(0, 0);
	MoveToNextFrame(device);
}

Context GetContext(SDevice* device)
{
	return device->mainContext;
}

RenderPass GetRenderPass(SDevice* device)
{
	return device->backBufferRenderPass;
}

void Destory(SDevice* device)
{
	CloseHandle(device->fenceEvent);
}
