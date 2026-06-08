#include "public.h"
#include "exposed_MS.h"

#include <utils/string.h>
#include <utils/MS/raw/cast.h>
#include "../style/DWM.h"

namespace barnack::window
	{
	handle_observer::handle_observer() noexcept = default;

	handle_observer::handle_observer(HWND system_window_handle) noexcept : handle{system_window_handle}
		{
		}

	handle_observer::~handle_observer()
		{
		}

	HWND handle_observer::get() const noexcept
		{
		return handle;
		}

	HWND handle_observer::release() noexcept
		{
		HWND ret{handle};
		handle = nullptr;
		return ret;
		}

	handle_observer::handle_observer(handle_observer&& move) noexcept : handle{move.handle} 
		{
		move.handle = nullptr;
		}

	handle_observer& handle_observer::operator=(handle_observer&& move) noexcept
		{
		handle = move.handle;
		move.handle = nullptr;
		return *this;
		}

	handle_observer handle_owner::release() noexcept
		{
		HWND hwnd{handle};
		handle = nullptr;
		return handle_observer{hwnd};
		}

	bool handle_observer::is_open() const noexcept { return handle; }
	void handle_observer::close  ()       noexcept 
		{
		if (handle)
			{
			::DestroyWindow(handle);
			handle = nullptr;
			}
		}

	void handle_observer::show() noexcept
		{
		::SetWindowPos(handle, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		::ShowWindow(handle, SW_SHOW);
		}

	void handle_observer::minimize() noexcept { ::ShowWindow(handle, SW_MINIMIZE); }
	void handle_observer::maximize() noexcept { ::ShowWindow(handle, SW_MAXIMIZE); }
	void handle_observer::restore () noexcept { ::ShowWindow(handle, SW_RESTORE ); }

	bool handle_observer::is_visible  () const noexcept
		{
		bool is_cloaked{false};
		DwmGetWindowAttribute(handle, DWMWA_CLOAKED, &is_cloaked, sizeof(is_cloaked));
		return IsWindowVisible(handle) && !is_cloaked && !is_minimized();
		}

	bool handle_observer::is_minimized() const noexcept { return IsIconic(handle); }
	bool handle_observer::is_maximized() const noexcept { return IsZoomed(handle); }

	handle_observer handle_observer::parent() const noexcept
		{
		return {GetParent(handle)};
		}

	utils::math::padding<long> handle_observer::rect_shadows_padding() const noexcept
		{
		RECT exclude_shadow;
		RECT include_shadow;

		DwmGetWindowAttribute(handle, DWMWA_EXTENDED_FRAME_BOUNDS, &exclude_shadow, sizeof(RECT));
		GetWindowRect(handle, &include_shadow);

		return
			{
			-(include_shadow.left   - exclude_shadow.left  ), // As of Win10 value = -7 if shadows enabled for hwnd, 0 otherwise.
			-(include_shadow.top    - exclude_shadow.top   ), // As of Win10 value =  0 if shadows enabled for hwnd, 0 otherwise.
			-(include_shadow.right  - exclude_shadow.right ), // As of Win10 value = +7 if shadows enabled for hwnd, 0 otherwise.
			-(include_shadow.bottom - exclude_shadow.bottom), // As of Win10 value = +7 if shadows enabled for hwnd, 0 otherwise.
			};
		}

	utils::math::padding<long> handle_observer::borders_padding() const noexcept
		{
		const auto window_rect{rect_window()};
		const auto client_rect{client_space_to_screen_space(rect_client())};
		return utils::math::padding<long>::create::rect_difference(window_rect, client_rect);
		}

	utils::math::rect_l handle_observer::rect_window() const noexcept
		{
		RECT rect_win32;
		if (DwmGetWindowAttribute(handle, DWMWA_EXTENDED_FRAME_BOUNDS, &rect_win32, sizeof(RECT)) == S_OK)
			{
			//Equivalent to using unpack_window_size on GetWindowRect
			return utils::MS::cast(rect_win32);
			}
		else if (GetWindowRect(handle, &rect_win32))
			{
			return utils::MS::cast(rect_win32);
			}
		else
			{
			//TODO error case
			return {};
			}
		}

	void handle_observer::rect_window(const utils::math::rect_l& rect) noexcept
		{
		SetWindowPos(handle, NULL, rect.ll(), rect.up(), rect.width(), rect.height(), SWP_NOZORDER);
		}

	utils::math::rect_l handle_observer::rect_full() const noexcept
		{
		return rect_window() + rect_shadows_padding();
		}

	void handle_observer::rect_full(const utils::math::rect_l& rect) noexcept
		{
		rect_window(rect - rect_shadows_padding());
		}

	utils::math::rect_l handle_observer::rect_client() const noexcept
		{
		RECT rect;
		GetClientRect(handle, &rect);//TODO error case
		return utils::MS::cast(rect);
		}

	void handle_observer::rect_client(const utils::math::rect_l& rect) noexcept
		{
		RECT rectangle{utils::MS::cast(rect)};

		// Updates the rect to take into account border/title bar if present
		AdjustWindowRectEx(&rectangle, static_cast<DWORD>(GetWindowLongPtr(handle, GWL_STYLE)), false, static_cast<DWORD>(GetWindowLongPtr(handle, GWL_EXSTYLE)));

		long width {rectangle.right  - rectangle.left};
		long height{rectangle.bottom - rectangle.top};

		SetWindowPos(handle, NULL, rectangle.left, rectangle.top, width, height, SWP_NOZORDER);
		}

	utils::math::vec2l handle_observer::screen_space_to_client_space(const utils::math::vec2l& coords) const noexcept
		{
		POINT output{coords.x(), coords.y()};
		ScreenToClient(handle, std::addressof(output));
		return {output.x, output.y};
		}

	utils::math::vec2l handle_observer::client_space_to_screen_space(const utils::math::vec2l& coords) const noexcept
		{
		POINT output{coords.x(), coords.y()};
		ClientToScreen(handle, std::addressof(output));
		return {output.x, output.y};
		}

	utils::math::rect_l handle_observer::screen_space_to_client_space(const utils::math::rect_l& rect) const noexcept
		{
		return utils::math::rect_l::create::from_ul_dr
			(
			screen_space_to_client_space(rect.ul()), 
			screen_space_to_client_space(rect.dr())
			);
		}

	utils::math::rect_l handle_observer::client_space_to_screen_space(const utils::math::rect_l& rect) const noexcept
		{
		return utils::math::rect_l::create::from_ul_dr
			(
			client_space_to_screen_space(rect.ul()),
			client_space_to_screen_space(rect.dr())
			);
		}
	
	handle_owner::handle_owner           (handle_owner&& move) noexcept = default;
	handle_owner& handle_owner::operator=(handle_owner&& move) noexcept
		{
		close();
		handle_observer::operator=(std::move(move));
		return *this;
		}
	handle_owner::~handle_owner()
		{
		close();
		}







	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
		{
		HWND p{FindWindowExW(hwnd, NULL, L"SHELLDLL_DefView", NULL)};
		HWND* ret{reinterpret_cast<HWND*>(lParam)};

		if (p)
			{
			// Gets the WorkerW Window after the current one.
			*ret = FindWindowExW(NULL, hwnd, L"WorkerW", NULL);
			}
		return true;
		}

	handle_observer find_desktop_wallpaper() noexcept
		{
		// Fetch the Progman window
		HWND progman{FindWindowW(L"ProgMan", NULL)};
		// Send 0x052C to Progman. This message directs Progman to spawn a 
		// WorkerW behind the desktop icons. If it is already there, nothing 
		// happens.
		SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
		// We enumerate all Windows, until we find one, that has the SHELLDLL_DefView 
		// as a child. 
		// If we found that window, we take its next sibling and assign it to workerw.
		HWND wallpaper_hwnd = nullptr;
		EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&wallpaper_hwnd));
		// Return the handle you're looking for.
		return {wallpaper_hwnd};
		}
	}