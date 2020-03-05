#pragma once

#include "Renderer.h"

struct Vertex
{
	XMFLOAT4 pos;
	XMFLOAT4 color;
};

struct ConstantBuffer
{
	XMFLOAT4X4 worldViewProjection;
	XMFLOAT4X4 rotateWithY;
};

struct Coordinate
{
	XMFLOAT3 cameraPosition;
	XMFLOAT3 lookDirection;
	XMFLOAT3 upDirection;
	float yaw, pitch;
	Coordinate() :
		cameraPosition(0, 0, 10.0f),
		yaw(XM_PI),
		pitch(0.0f),
		lookDirection(0, 0, -1),
		upDirection(0, 1, 0)
	{
	}

	XMMATRIX GetViewMatrix()
	{
		return XMMatrixLookAtRH(XMLoadFloat3(&cameraPosition), XMLoadFloat3(&lookDirection), XMLoadFloat3(&upDirection));
	}

	XMMATRIX GetProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		return XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane);
	}

};

class BaseApp
{
public:
	BaseApp(UINT width, UINT height, std::wstring title);
	virtual ~BaseApp();

	UINT GetWidth() const { return m_width; }
	UINT GetHeight() const { return m_height; }
	const WCHAR* GetTitle() const { return m_title.c_str(); }

	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnRender();

	virtual void OnKeyUp(UINT8 wParam) {}
	virtual void OnKeyDown(UINT8 wParam) {}

	void SetWindow(HWND hwnd) { m_hwnd = hwnd; }
	HWND GetWindow() { return m_hwnd; }

protected:
	void PopulateCommand();
	Kernel m_kernel;
	RootSignature m_rootSignature;
	Pipeline m_pipeline;
	VertexSetup m_vertexSetup;

	UINT m_width, m_height;
	std::wstring m_title;
	HWND m_hwnd;
};