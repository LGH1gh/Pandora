#include "stdafx.h"
#include "DXBase.h"

using namespace Microsoft::WRL;
DXBase::DXBase(UINT width, UINT height, std::wstring name) :
	m_width(width),
	m_height(height),
	m_title(name),
	m_useWarpDevice(false)
{
	m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

DXBase::~DXBase()
{
}

void DXBase::SetCustomWindowText(LPCWSTR text)
{
	std::wstring windowText = m_title + L":" + text;
	SetWindowText(Win32Application::GetHwnd(), windowText.c_str());
}
