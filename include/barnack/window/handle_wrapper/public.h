#pragma once

#include <array>

#include <utils/math/rect.h>
#include <utils/oop/disable_move_copy.h>

struct HWND__;
using HWND = HWND__*;

namespace barnack::window
	{
	struct handle_observer : utils::oop::non_copyable
		{
		handle_observer(HWND system_window_handle) noexcept;
		handle_observer() noexcept;
		~handle_observer();
		handle_observer(handle_observer&& move) noexcept;
		handle_observer& operator=(handle_observer&& move) noexcept;

		HWND get() const noexcept;
		HWND release() noexcept;
		void destroy() noexcept;

		bool is_open() const noexcept;
		void close  ()       noexcept;

		void show    () noexcept;
		void minimize() noexcept;
		void maximize() noexcept;
		void restore () noexcept;
		
		bool is_visible  () const noexcept;
		bool is_minimized() const noexcept;
		bool is_maximized() const noexcept;

		handle_observer parent() const noexcept;

		/// <summary> Padding around window rect to window rect with shadows. </summary>
		utils::math::padding<long> rect_shadows_padding() const noexcept;
		/// <summary> Padding around client rect to window rect without shadows. </summary>
		utils::math::padding<long> borders_padding() const noexcept;

		/// <summary> Client area, borders and title bar. Screen space coordinates. </summary>
		utils::math::rect_l rect_window() const noexcept;
		/// <summary> Client area, borders and title bar. Screen space coordinates. </summary>
		void   rect_window(const utils::math::rect_l& rect) noexcept;

		/// <summary> Window rect plus shadows. Screen space coordinates. </summary>
		utils::math::rect_l rect_full() const noexcept;
		/// <summary> Window rect plus shadows. Screen space coordinates. </summary>
		void   rect_full(const utils::math::rect_l& rect) noexcept;

		/// <summary>Internal window area, excluding borders and title bar. Client space cooridnates. </summary>
		utils::math::rect_l rect_client() const noexcept;
		/// <summary>Internal window area, excluding borders and title bar. Client space cooridnates. </summary>
		void   rect_client(const utils::math::rect_l& rect) noexcept;

		utils::math::vec2l  screen_space_to_client_space(const utils::math::vec2l & coords) const noexcept;
		utils::math::vec2l  client_space_to_screen_space(const utils::math::vec2l & coords) const noexcept;
		utils::math::rect_l screen_space_to_client_space(const utils::math::rect_l& rect  ) const noexcept;
		utils::math::rect_l client_space_to_screen_space(const utils::math::rect_l& rect  ) const noexcept;

		protected:
			HWND handle;
		};

	struct handle_owner : handle_observer
		{
		using handle_observer::handle_observer;
		handle_owner(handle_owner&& move) noexcept;
		handle_owner& operator=(handle_owner&& move) noexcept;
		~handle_owner();

		handle_observer release() noexcept;
		};

	handle_observer find_desktop_wallpaper() noexcept;
	}

#ifdef utils_implementation
#include "exposed_MS.h"
#endif