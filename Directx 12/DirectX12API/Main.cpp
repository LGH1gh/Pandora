#include "stdafx.h"
#include "D3D12Camera.h"

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	D3D12Camera sample(1280, 720, L"D3D12 Frame Work");
	return Win32Application::Run(&sample, hInstance, nShowCmd);

}