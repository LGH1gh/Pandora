#pragma once

#include "Renderer.h"

struct Vertex
{
	XMFLOAT4 pos;
	XMFLOAT2 texCoord;
	Vertex(float a, float b, float c, float d, float u, float v):
		pos(a, b, c, d), texCoord(u, v)
	{}
};

struct ConstantBuffer
{
	XMFLOAT4X4 worldViewProjection;
	XMFLOAT4X4 rotateWithY;
	XMFLOAT4X4 rotateWithZ;
};

struct SceneConstantBuffer
{
	XMFLOAT4 offset;
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
	ConstantBuffer m_constantData;
	Coordinate m_coordinate;

	DescriptorHeap m_depthStencil;
	DescriptorHeap m_constantBuffer;
	DescriptorHeap m_texture;

	std::vector<FontDesc> m_texts;

	UINT m_width, m_height;
	std::wstring m_title;
	HWND m_hwnd;
};