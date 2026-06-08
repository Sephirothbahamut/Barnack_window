#pragma once

#include "../handle_wrapper.h"
#include "../icon.h"
#include "../style.h"

#include <functional>

#include <utils/math/vec.h>
#include <utils/math/rect.h>



namespace barnack::window
	{
	struct base;

	struct initializer
		{
		initializer();
		~initializer();
		};

	enum class simple_event_return_type { handled, use_default_handler };
	enum class hit_result
		{
		resize_right,
		resize_left,
		resize_up,
		resize_down,
		resize_up_right,
		resize_up_left,
		resize_down_right,
		resize_down_left,
		client,
		hole,
		//input passthrough to windows created by the same thread
		same_thread_hole,
		drag,
		use_default_handler
		};

	struct callbacks
		{
		std::function<simple_event_return_type(base&)> on_close;

		std::function<simple_event_return_type(base&)> on_move_begin;
		std::function<std::optional<utils::math::rect_l>(base&, const utils::math::rect_l&)> on_move;
		std::function<simple_event_return_type(base&)> on_move_end;

		std::function<simple_event_return_type(base&)> on_resize_begin;
		std::function<std::optional<utils::math::rect_l>(base&, const utils::math::rect_l&)> on_resize;
		std::function<simple_event_return_type(base&)> on_resize_end;

		/// <summary> Parameter is in screen space, not client space. </summary>
		std::function<hit_result(base&, const utils::math::vec2l&)> on_hit_test;

		std::optional<utils::math::vec2l> minimum_size;
		};

	struct base : handle_owner
		{
		base() noexcept;
		base(HWND system_window_handle) noexcept;
		base(base&& move) noexcept;
		base& operator=(base&& move) noexcept;

		callbacks callbacks;

		bool poll_event() const;
		void wait_event() const;
		};

	struct factory
		{
		struct size
			{
			enum class type { client, window };
			type type;
			utils::math::vec2l value;
			};

		std::string title{"default_window_title"};
		utils::observer_ptr<base> parent_window{nullptr};
		std::optional<utils::math::vec2l> position;
		std::optional<utils::math::vec2l> size;
		std::optional<utils::math::vec2l> minimum_size;

		callbacks callbacks;

		std::optional<style> style;

		std::optional<icon> icon;

		base create();
		};

	}

#ifdef utils_implementation
#include "exposed_MS.h"
#endif