#include "stdafx.h"
#include "Win32Application.h"

HWND Win32Application::m_hwndnullptr;

int Win32Application::Run(BaseApp* pBaseApp, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX windowClass{};
	windowClass.cbSizesizeof(WNDCLASSEX);
	windowClass.styleCS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProcWindowProc;
	windowClass.hInstancehInstance;
	windowClass.hCursorLoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassNameL"BaseAppClass";
	RegisterClassEx(&windowClass);

	RECT windowRect{ 0, 0, static_cast<LONG>(pBaseApp->GetWidth()), static_cast<LONG>(pBaseApp->GetHeight()) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	m_hwndCreateWindow(
		windowClass.lpszClassName,
		pBaseApp->GetTitle(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		hInstance,
		pBaseApp);

	pBaseApp->OnInit();
	ShowWindow(m_hwnd, nCmdShow);

	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	pBaseApp->OnDestroy();

	return static_cast<char>(msg.wParam);
}

LRESULT CALLBACK Win32Application::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BaseApp* pBasereinterpret_cast<BaseApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	switch (message)
	{
	case WM_CREATE:
	{
		LPCREATESTRUCT pCreateStructreinterpret_cast<LPCREATESTRUCT> (lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
	}
	return 0;

	case WM_KEYDOWN:
		if (pBase)
		{
			pBase->OnKeyDown(static_cast<UINT8> (wParam));
		}
		return 0;

	case WM_KEYUP:
		if (pBase)
		{
			pBase->OnKeyUp(static_cast<UINT8> (wParam));
		}
		return 0;

	case WM_PAINT:
		if (pBase)
		{
			pBase->OnUpdate();
			pBase->OnRender();
		}
		return 0;


	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}