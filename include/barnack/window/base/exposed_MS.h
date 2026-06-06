#pragma once

#include "public.h"

#include <utils/MS/raw/windows.h>

namespace barnack::window
	{
	inline static constexpr LPCWSTR window_class_name{L"barnack/window/base"};
	LRESULT __stdcall window_procedure(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;

	void set_window_ptr(HWND handle, utils::observer_ptr<base> leaf_pointer);
	utils::observer_ptr<base> get_window_ptr(HWND handle);
	}

#ifdef utils_implementation
#include "definitions.cpp"
#include "window_procedure.cpp"
#include "factory.cpp"
#endif