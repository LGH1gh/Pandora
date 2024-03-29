#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

#include <windows.h>

#include "d3dx12.h"
#include <d3d12.h>

#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <pix.h>

#include <string>
#include <wrl.h>
#include <shellapi.h>

#pragma comment ( lib, "d3d12.lib")
#pragma comment ( lib, "dxgi.lib")
#pragma comment ( lib, "d3dcompiler.lib" )
#pragma comment ( lib, "dxguid.lib" )