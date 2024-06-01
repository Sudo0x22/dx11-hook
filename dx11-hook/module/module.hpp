#pragma once
#ifndef MODULE_HPP
#define MODULE_HPP

#include<Windows.h>
#include<memory>

class Module_t
{
public:
	template<typename Type>
	__inline Type GetModule(const wchar_t* sz_image);
}; inline Module_t* pModule = nullptr;

template<typename Type>
inline Type Module_t::GetModule(const wchar_t* sz_image)
{
	const auto h_module = (Type)(GetModuleHandleW(sz_image));
	if (!h_module) return nullptr;
	return h_module;
}

#endif
