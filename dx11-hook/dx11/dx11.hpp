#pragma once
#ifndef DX11_HPP
#define DX11_HPP

#include<d3d11.h>
#include<dxgi.h>
#include"../hook/hook.hpp"
#include<stdio.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain*, UINT, UINT);
typedef LRESULT(__stdcall* WndProc_t)(HWND, UINT, WPARAM, LPARAM);

namespace globals
{
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = nullptr;

	Present_t PresentFn = 0;
	WndProc_t WndProcFn = 0;

	HWND hWindow = 0;
	HWND hWindowHandle = 0;
	WNDCLASSEX Windowclass;
}

using namespace globals;

class DX11_t
{
public:
	__inline bool InitWindow()
	{
		Windowclass.cbSize = sizeof(WNDCLASSEX);
		Windowclass.hInstance = GetModuleHandleA(NULL);
		Windowclass.lpszClassName = L"SliverCat";
		Windowclass.lpfnWndProc = DefWindowProc;

		RegisterClassExW(&Windowclass);
		hWindowHandle = CreateWindow(Windowclass.lpszClassName,
			L"DX11 Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL,
			Windowclass.hInstance, NULL);
		if (hWindowHandle != NULL)
			return false;
		return true;
	}
	__inline bool DeleteWindow()
	{
		DestroyWindow(hWindowHandle);
		UnregisterClass(Windowclass.lpszClassName, Windowclass.hInstance);
		if (hWindowHandle == NULL)
			return false;
		return true;
	}
	__inline bool Initialize()
	{
		if (this->InitWindow() == false)
		{
			DeleteWindow(); return false;
		}

		const auto h_module = GetModuleHandleA("d3d11.dll");
		const auto h_function = GetProcAddress(h_module, "D3D11CreateDeviceAndSwapChain");

		if (!h_module || !h_function) {
			DeleteWindow();
			return false;
		}

		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0
		};

		DXGI_RATIONAL refreshRate;
		refreshRate.Numerator = 60;
		refreshRate.Denominator = 1;

		DXGI_MODE_DESC bufferDesc;
		bufferDesc.Width = 100;
		bufferDesc.Height = 100;
		bufferDesc.RefreshRate = refreshRate;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow =hWindowHandle;
		swapChainDesc.Windowed = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* swapChain;
		ID3D11Device* device;
		ID3D11DeviceContext* context;

		if (((long(__stdcall*)(
			IDXGIAdapter*,
			D3D_DRIVER_TYPE,
			HMODULE,
			UINT,
			const D3D_FEATURE_LEVEL*,
			UINT,
			UINT,
			const DXGI_SWAP_CHAIN_DESC*,
			IDXGISwapChain**,
			ID3D11Device**,
			D3D_FEATURE_LEVEL*,
			ID3D11DeviceContext**))(h_function))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
		{
			DeleteWindow();
			return false;
		}


		Table = (uint64_t*)::calloc(205, sizeof(uint64_t));
		memcpy(Table, *(uint64_t**)swapChain, 18 * sizeof(uint64_t));
		memcpy(Table + 18, *(uint64_t**)device, 43 * sizeof(uint64_t));
		memcpy(Table + 18 + 43, *(uint64_t**)context, 144 * sizeof(uint64_t));

		MH_Initialize();
		swapChain->Release();
		device->Release();
		context->Release();

		swapChain = NULL;
		device = NULL;
		context = NULL;

		DeleteWindow();
		return true;
	}
public:
	__inline void DumpSwapchain();
}; inline DX11_t* pDX11;


#endif