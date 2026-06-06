#pragma once

#include "../handle_wrapper.h"

#include <functional>

#include <utils/math/vec.h>
#include <utils/math/rect.h>

#include <utils/matrix.h>
#include <utils/graphics/colour.h>


namespace barnack::window
	{
	struct opengl : utils::oop::non_copyable
		{
		struct pimpl_storage_t
			{
			void* a;
			void* b;
			void* c;
			};

		struct pimpl_t;
		const pimpl_t& pimpl() const noexcept;
		      pimpl_t& pimpl()       noexcept;
		alignas(pimpl_storage_t) std::array<std::byte, sizeof(pimpl_storage_t)> pimpl_storage;

		opengl(handle_observer& handle_observer);
		~opengl();
		opengl(opengl&& move) noexcept;
		opengl& operator=(opengl&& move) noexcept;

		bool is_valid() const noexcept;

		void make_active() const noexcept;
		bool is_active  () const noexcept;
		};

	}

#ifdef utils_implementation
#include "exposed_MS.h"
#endif