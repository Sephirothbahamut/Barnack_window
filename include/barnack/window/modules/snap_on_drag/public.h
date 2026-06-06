#pragma once

#include <array>

#include <utils/math/rect.h>

#include "../../base.h"
#include "../../handle_wrapper.h"

namespace barnack::window::module
	{
	struct snap_on_drag
		{
		long snap_max_distance{16};

		void move_begin(window::base& window) noexcept;
		utils::math::rect_l move(const utils::math::rect_l& rect) noexcept;
		void move_end() noexcept;

		utils::math::rect_l target;
		utils::math::rect_l attempted_result;
		utils::math::rect_l result_previous;
		std::vector<utils::math::rect_l> existing_windows_rects;
		};

	}

#ifdef utils_implementation
#include "definitions.cpp"
#endif