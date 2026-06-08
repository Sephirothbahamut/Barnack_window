#pragma once

#include "public.h"

#include "DWM.h"

namespace barnack::window
	{
	struct style::pimpl_t
		{
		const style& style;

		bool composition_enabled() const noexcept;
		DWORD select_borderless_style() const noexcept;

		void set_shadow(HWND handle, bool enabled) const noexcept;

		bool maximized(HWND hwnd) const noexcept;

		void apply_to_DWORDS(DWORD& win32_style_ex, DWORD& win32_style) const noexcept;

		void apply_transparency(HWND handle) const noexcept;
		};
	}

#ifdef utils_implementation
#include "definitions.cpp"
#endif