#pragma once

#include "public.h"

#include <utils/MS/raw/windows.h>

namespace barnack::window
	{
	struct opengl::pimpl_t
		{
		pimpl_t() noexcept;
		pimpl_t(HWND handle_window, HDC handle_device_context, HGLRC handle_opengl_context) noexcept;
		pimpl_t(pimpl_t&& move) noexcept;
		pimpl_t& operator=(pimpl_t&& move) noexcept;
		~pimpl_t();

		HWND  handle_window;
		HDC   handle_device_context;
		HGLRC handle_opengl_context;
		void destroy_if_exists() noexcept;
		};
	}

#ifdef utils_implementation
#include "definitions.cpp"
#endif