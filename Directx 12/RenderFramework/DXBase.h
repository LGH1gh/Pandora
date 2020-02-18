#pragma once
#include "Win32Application.h"
#include "Geometry.h"
#include "DXBasehelper.h"

class DXBase
{
public:
	DXBase(UINT width, UINT height, std::wstring name);
	virtual ~DXBase();

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;

	virtual void SetGeometry(Geometry* geometry) = 0;

	virtual void OnKeyDown(UINT8) {}
	virtual void OnKeyUp(UINT8) {}

	UINT GetWidth() const { return m_width; }
	UINT GetHeight() const { return m_height; }
	const WCHAR* GetTitle() const { return m_title.c_str(); }

protected:
	std::wstring GetAssetFullPath(LPCWSTR assetName);
	void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
	void SetCustomWindowText(LPCWSTR text);

	UINT m_width;
	UINT m_height;
	float m_aspectRatio;

	bool m_useWarpDevice;

private:
	std::wstring m_assetPath;
	std::wstring m_title;
};