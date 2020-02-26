#pragma once

#include "Renderer.h"

class BaseApp
{
public:
	BaseApp(UINT width, UINT height, std::wstring title);
	virtual ~BaseApp();

	UINT GetWidth() const { return m_deviceParams.width; }
	UINT GetHeight() const { return m_deviceParams.height; }
	const WCHAR* GetTitle() const { return m_title.c_str(); }

	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnRender();

	virtual void OnKeyUp(UINT8 wParam) {}
	virtual void OnKeyDown(UINT8 wParam) {}

	void ParseCommandLineArgs(WCHAR* argv[], int argc);
	void SetWindow(HWND hwnd) { m_hwnd = hwnd; }
	HWND GetWindow() { return m_hwnd; }

protected:
	void PopulateCommand();

	struct Vertex
	{
		VFLOAT3 pos;
		VFLOAT4 color;
	};

	Device m_device;
	DeviceParams m_deviceParams;
	RootSignature m_rootSignature;
	Pipeline m_pipeline1;
	Pipeline m_pipeline2;
	VertexSetup m_vertexSetup;

	BlendState m_blendState;

	std::wstring m_title;
	HWND m_hwnd;
};