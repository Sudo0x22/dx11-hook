#include"present/present.hpp"

static void EntryPoint(LPVOID hBuffer)
{
	AllocConsole();
	FILE* in = {};
	freopen_s(&in, "CONOUT$", "w", stdout);
	
	Swapchain_t swapchain(FindWindowA("grcWindow", nullptr), GetModuleHandleA("d3d11.dll"));
	swapchain.GetSwapchain();
	printf("swapchain: 0x%p\n", (void*)swapchain.pSwapchain);

	Hook_t present(swapchain.pSwapchain, 8);
	PresentFn = (Present_t)CallHook(present, (std::uintptr_t)PresentHk);
}

static bool DllMain(HMODULE hModule, DWORD hReasons, LPVOID hBuffer)
{
	if (hReasons != 0x1)
		return false;

	void* thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)EntryPoint, 0, 0, 0);
	if (thread != 0) CloseHandle(thread);

	return true;
}