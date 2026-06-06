#pragma once

#include "public.h"

#include <utils/MS/raw/windows.h>

namespace barnack::window
	{
	struct icon::pimpl_t
		{
		pimpl_t() noexcept;
		pimpl_t(HICON handle) noexcept;
		pimpl_t(pimpl_t&& move) noexcept;
		pimpl_t& operator=(pimpl_t&& move) noexcept;
		~pimpl_t();

		HICON handle{nullptr};

		void destroy_if_exists() noexcept;
		};
	}

#ifdef utils_implementation
#include "definitions.cpp"
#endif