#pragma once
#ifndef HOOK_HPP
#define HOOK_HPP

#include<cstdint>
#include<cassert>
#include"minhook/include/MinHook.h"

typedef uintptr_t uint;
typedef void* ptr_t;

// if building for x86 change uint64_t* to uint32_t*
static uint64_t* Table = nullptr;

class Hook_t
{
public:
	__inline uint vmt_hook(ptr_t target_addr,
		size_t index, uint detour_addr);
	__inline bool hook(uint16_t index, ptr_t detour_addr, ptr_t* original_addr);
}; inline Hook_t* pHook = nullptr;

__inline uint Hook_t::vmt_hook(ptr_t target_addr,
	size_t index, uint detour_addr)
{
	uint table = *(uint*)(target_addr);
	uint func_addr = table + index * sizeof(uint);
	uint orig_addr = *(uint*)(func_addr);

	return orig_addr;
}

inline bool Hook_t::hook(uint16_t index, ptr_t detour_addr, ptr_t* original_addr)
{
	assert(index >= 0 && detour_addr != 0 && original_addr != 0);
	ptr_t target_addr = (ptr_t)Table[index];
	if (MH_CreateHook(target_addr, detour_addr, original_addr) != MH_OK ||
		MH_EnableHook(target_addr) != MH_OK)
		return false;
	return true;
}



#endif


