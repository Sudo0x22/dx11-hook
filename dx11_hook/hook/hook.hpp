#pragma once
#include<Windows.h>
#include<cstdint>

class Hook_t
{
private:
	void* target = nullptr;
	size_t index = 0u;
public:
	Hook_t(void* target_, size_t index_)
	{
		target = target_;
		index = index_;
	}
	Hook_t(void) = default;
private:
	void* GetTarget() const { return target; }
	size_t GetIndex() const { return index; }
public:
	std::uintptr_t Hook(std::uintptr_t detour)
	{
		std::uintptr_t table = *(std::uintptr_t*)(this->GetTarget());
		std::uintptr_t func_addr = table + this->GetIndex() * sizeof(std::uintptr_t);
		std::uintptr_t orig_addr = *(std::uintptr_t*)(func_addr);

		unsigned long old_protect;
		VirtualProtect((LPVOID)func_addr, sizeof(std::uintptr_t), PAGE_EXECUTE_READWRITE, &old_protect);
		*(std::uintptr_t*)(func_addr) = (std::uintptr_t)detour;
		VirtualProtect((LPVOID)func_addr, sizeof(std::uintptr_t), old_protect, &old_protect);

		return orig_addr;
	}
};

std::uintptr_t CallHook(Hook_t hook, std::uintptr_t detour) {
	return hook.Hook(detour);
}