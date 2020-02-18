#include "stdafx.h"
#include "Vertices.h"
#include "SceneConstantBuffer.h"
#include "D3D12Render.h"

struct Text
{
	XMFLOAT4 abv;
};

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	D3D12Render sample(720, 720, L"D3D12 Frame Work");
	
	Vertices vertices;
	vertices.AddVertex(Vertex({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	vertices.AddVertex(Vertex({ 0.25f, 0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }));
	vertices.AddVertex(Vertex({ 0.0f, 0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }));
	vertices.AddVertex(Vertex({ 0.25f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }));
	vertices.SetIndex({
		0, 2, 1,
		0, 1, 3
	});
	sample.SetGeometry(&vertices);

	SceneConstantBuffer constantBufferData;
	sample.SetConstantBuffer(&constantBufferData);

	sample.SetVertexShader(L"D:\\Pandora\\Directx 12\\RenderFramework\\Shader.hlsl", "VSMain");
	sample.SetPixelShader(L"D:\\Pandora\\Directx 12\\RenderFramework\\Shader.hlsl", "PSMain");

	return Win32Application::Run(&sample, hInstance, nShowCmd);

}