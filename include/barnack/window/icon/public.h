#pragma once

#include "../handle_wrapper.h"

#include <functional>

#include <utils/math/vec.h>
#include <utils/math/rect.h>

#include <utils/matrix.h>
#include <utils/graphics/colour.h>


namespace barnack::window
	{
	struct icon : utils::oop::non_copyable
		{
		struct pimpl_t;
		const pimpl_t& pimpl() const noexcept;
		      pimpl_t& pimpl()       noexcept;
		alignas(void*) std::array<std::byte, sizeof(void*)> pimpl_storage;

		icon() noexcept;
		icon(const utils::matrix<utils::graphics::colour::rgba_u>& pixels) noexcept;
		~icon();
		icon(icon&& move) noexcept;
		icon& operator=(icon&& move) noexcept;

		bool is_open() const noexcept;
		};

	}

#ifdef utils_implementation
#include "exposed_MS.h"
#endif