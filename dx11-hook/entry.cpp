#include"dx11/dx11.hpp"
#include <cstdio>

HRESULT __stdcall Present_hk(IDXGISwapChain* pSwapChain, UINT uInt, UINT uFlags)
{
	printf("[ LOG ]: Hooked Present Function\n");
	printf("[ LOG ]: SwapChain: 0x%p\n", pSwapChain);
	return PresentFn(pSwapChain, uInt, uFlags);
}

bool __stdcall DllMain(void* h_module, unsigned long call_reasons,
	void* call_buffer)
{
	if (call_reasons != 0x1)
		return false;

	AllocConsole();
	FILE* in = {};
	freopen_s(&in, "CONOUT$", "w", stdout);

	if (pDX11->Initialize() == false)
	{
		printf("[ DEBUG ]: Error Initializing DX11\n");
	}

	printf("[ LOG ]: Initialized DX11 Hooking Present now\n");
	pHook->hook(8, (void*)Present_hk, (void**)&PresentFn);

	return true;
}