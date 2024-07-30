#pragma once

#include<d3d11.h>
#include<dxgi.h>

#include<cstdint>
#include<stdio.h>

class Swapchain_t
{
private:
	HWND hWindow = 0;
	HMODULE hD3D11Module = 0;
public:
	IDXGISwapChain* pSwapchain = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
public:
	Swapchain_t(HWND class_, HMODULE module_)
	{
		hWindow = class_;
		hD3D11Module = module_;
	}
	Swapchain_t(void) = default;
private:
	HWND GetWindow() const { return hWindow; }
	HMODULE GetD3D11Module() const { return hD3D11Module; }
public:
	void GetSwapchain()
	{
		if (this->GetD3D11Module() == 0 || this->GetWindow() == 0)
			return;

		void* CreateDeviceAndSwapchain = ::GetProcAddress(this->GetD3D11Module(), "D3D11CreateDeviceAndSwapChain");
		if (CreateDeviceAndSwapchain == nullptr)
			return;

		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

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
		swapChainDesc.OutputWindow = GetWindow();
		swapChainDesc.Windowed = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

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
			ID3D11DeviceContext**))(CreateDeviceAndSwapchain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &pSwapchain, &pDevice, &featureLevel, &pContext) < 0)
		{
			return;
		}
	}
};
