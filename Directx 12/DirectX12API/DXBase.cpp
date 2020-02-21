#include "stdafx.h"
#include "BaseApp.h"

using namespace Microsoft::WRL;
BaseApp::BaseApp(UINT width, UINT height, std::wstring name) :
	m_width(width),
	m_height(height),
	m_title(name),
	m_useWarpDevice(true)
{
	WCHAR assetsPath[512];
	GetAssetsPath(assetsPath, _countof(assetsPath));
	m_assetsPathassetsPath;

	m_aspectRatiostatic_cast<float>(width) / static_cast<float>(height);
}

BaseApp::~BaseApp()
{
}

std::wstring BaseApp::GetAssetFullPath(LPCWSTR assetName)
{
	return m_assetsPath + assetName;
}

void BaseApp::SetCustomWindowText(LPCWSTR text)
{
	std::wstring windowTextm_title + L":" + text;
	SetWindowText(Win32Application::GetHwnd(), windowText.c_str());
}

void BaseApp::GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapternullptr;

	for (UINT adapterIndex0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapteradapter.Detach();
}