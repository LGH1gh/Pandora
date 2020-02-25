#pragma once

enum ImageFormat
{
	IMAGE_FORMAT_UNKNOWN,

	IMAGE_FORMAT_R8_UNORM,
	IMAGE_FORMAT_R8_UINT,

	IMAGE_FORMAT_R8G8_UNORM,
	IMAGE_FORMAT_R8G8_UINT,

	IMAGE_FORMAT_R8G8B8A8_UNORM,
	IMAGE_FORMAT_R8G8B8A8_UINT,

	IMAGE_FORMAT_R16_UNORM,
	IMAGE_FORMAT_R16_UINT,
	IMAGE_FORMAT_R16_FLOAT,

	IMAGE_FORMAT_R16G16_UNORM,
	IMAGE_FORMAT_R16G16_UINT,
	IMAGE_FORMAT_R16G16_FLOAT,

	IMAGE_FORMAT_R16G16B16A16_UNORM,
	IMAGE_FORMAT_R16G16B16A16_UINT,
	IMAGE_FORMAT_R16G16B16A16_FLOAT,

	IMAGE_FORMAT_R32_UINT,
	IMAGE_FORMAT_R32_FLOAT,

	IMAGE_FORMAT_R32G32_UINT,
	IMAGE_FORMAT_R32G32_FLOAT,

	IMAGE_FORMAT_R32G32B32A32_UINT,
	IMAGE_FORMAT_R32G32B32A32_FLOAT,

	IMAGE_FORMAT_BC1_UNORM,
	IMAGE_FORMAT_BC2_UNORM,
	IMAGE_FORMAT_BC3_UNORM,
	IMAGE_FORMAT_BC4_UNORM,
	IMAGE_FORMAT_BC5_UNORM,
	IMAGE_FORMAT_BC7_UNORM,

	IMAGE_FORMAT_D16_UNORM,
};
