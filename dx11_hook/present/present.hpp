#pragma once

#include"../hook/hook.hpp"
#include"../swapchain/swapchain.hpp"

#include"../imgui/imgui.h"
#include"../imgui/imgui_impl_dx11.h"
#include"../imgui/imgui_impl_win32.h"

typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain*, UINT, UINT);
typedef LRESULT(__stdcall* WndProc_t)(HWND, UINT, WPARAM, LPARAM);
extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

Present_t PresentFn = 0;
WndProc_t WndProcFn = 0;

bool init_imgui = false;
bool render_menu = false;

ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;
HWND hWindow = 0;

LRESULT __stdcall WndProc(HWND hWnd, UINT uInt, WPARAM wParam, LPARAM lParam)
{
	if (render_menu)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uInt, wParam, lParam);
		return TRUE;
	}
	return CallWindowProcA(WndProcFn, hWnd, uInt, wParam, lParam);
}

HRESULT __stdcall PresentHk(IDXGISwapChain* this_, UINT uInt, UINT uFlags)
{
	if (!init_imgui)
	{
		if (SUCCEEDED(this_->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			ImGui::CreateContext();
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC desc;
			this_->GetDesc(&desc);
			hWindow = desc.OutputWindow;
			if (hWindow == 0)
				return PresentFn(this_, uInt, uFlags);
			ID3D11Texture2D* pBackBuffer;
			this_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
			pBackBuffer->Release();
			WndProcFn = (WNDPROC)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
			ImGui_ImplWin32_Init(hWindow);
			ImGui_ImplDX11_Init(pDevice, pContext);
			init_imgui = true;
		}
		else
			return PresentFn(this_, uInt, uFlags);
	}
	
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();	
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_INSERT) & 0x1)
		render_menu = !render_menu;

	if (render_menu == true)
	{
		ImGui::Begin("DX11-Hook", nullptr, 0);
		ImGui::SetWindowSize(ImVec2(450, 450));
		ImGui::End();
	}
	
	ImGui::EndFrame();
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return PresentFn(this_, uInt, uFlags);
}