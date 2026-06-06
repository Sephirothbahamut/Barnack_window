#include "public.h"
#include "exposed_MS.h"

#include <utils/string.h>
#include <utils/MS/raw/cast.h>
#include <utils/MS/window/details/DWM.h>

namespace barnack::window
	{
	handle_observer::pimpl_t::pimpl_t() noexcept : pimpl_t{nullptr} {}
	handle_observer::pimpl_t::pimpl_t(HWND handle) noexcept : handle{handle} {}

	handle_observer::pimpl_t::pimpl_t(pimpl_t&& move) noexcept : pimpl_t{move.handle} { move.handle = nullptr; }
	handle_observer::pimpl_t& handle_observer::pimpl_t::operator=(pimpl_t&& move) noexcept 
		{
		destroy_window_if_exists();
		handle = move.handle; 
		move.handle = nullptr; 
		return *this;
		}
	handle_observer::pimpl_t::~pimpl_t() {}

	void handle_observer::pimpl_t::destroy_window_if_exists() noexcept
		{
		if (handle) 
			{
			::DestroyWindow(handle);
			handle = nullptr;
			}
		}

	handle_observer::handle_observer(void* system_window_handle) noexcept : handle_observer{}
		{
		pimpl().handle = reinterpret_cast<HWND>(system_window_handle);
		}

	handle_observer::handle_observer() noexcept
		{
		static_assert(sizeof(handle_observer::pimpl_t) == sizeof(pimpl_storage));
		new (pimpl_storage.data()) handle_observer::pimpl_t;
		}
	handle_observer::~handle_observer()
		{
		pimpl().~pimpl_t();
		}

	const handle_observer::pimpl_t& handle_observer::pimpl() const noexcept { return *std::launder(reinterpret_cast<const pimpl_t*>(pimpl_storage.data())); }
	      handle_observer::pimpl_t& handle_observer::pimpl()       noexcept { return *std::launder(reinterpret_cast<      pimpl_t*>(pimpl_storage.data())); }



	handle_observer::handle_observer           (handle_observer&& move) noexcept { pimpl() = std::move(move.pimpl()); }
	handle_observer& handle_observer::operator=(handle_observer&& move) noexcept { pimpl() = std::move(move.pimpl()); return *this; }








	bool handle_observer::is_open() const noexcept { return pimpl().handle; }
	void handle_observer::close  ()       noexcept { pimpl().destroy_window_if_exists(); }

	void handle_observer::show() noexcept
		{
		::SetWindowPos(pimpl().handle, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		::ShowWindow(pimpl().handle, SW_SHOW);
		}

	void handle_observer::minimize() noexcept { ::ShowWindow(pimpl().handle, SW_MINIMIZE); }
	void handle_observer::maximize() noexcept { ::ShowWindow(pimpl().handle, SW_MAXIMIZE); }
	void handle_observer::restore () noexcept { ::ShowWindow(pimpl().handle, SW_RESTORE ); }

	bool handle_observer::is_visible  () const noexcept
		{
		bool is_cloaked{false};
		DwmGetWindowAttribute(pimpl().handle, DWMWA_CLOAKED, &is_cloaked, sizeof(is_cloaked));
		return IsWindowVisible(pimpl().handle) && !is_cloaked && !is_minimized();
		}

	bool handle_observer::is_minimized() const noexcept { return IsIconic(pimpl().handle); }
	bool handle_observer::is_maximized() const noexcept { return IsZoomed(pimpl().handle); }

	handle_observer handle_observer::parent() const noexcept
		{
		return {GetParent(pimpl().handle)};
		}

	utils::math::padding<long> handle_observer::rect_shadows_padding() const noexcept
		{
		RECT exclude_shadow;
		RECT include_shadow;

		DwmGetWindowAttribute(pimpl().handle, DWMWA_EXTENDED_FRAME_BOUNDS, &exclude_shadow, sizeof(RECT));
		GetWindowRect(pimpl().handle, &include_shadow);

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
		if (DwmGetWindowAttribute(pimpl().handle, DWMWA_EXTENDED_FRAME_BOUNDS, &rect_win32, sizeof(RECT)) == S_OK)
			{
			//Equivalent to using unpack_window_size on GetWindowRect
			return utils::MS::cast(rect_win32);
			}
		else if (GetWindowRect(pimpl().handle, &rect_win32))
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
		SetWindowPos(pimpl().handle, NULL, rect.ll(), rect.up(), rect.width(), rect.height(), SWP_NOZORDER);
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
		GetClientRect(pimpl().handle, &rect);//TODO error case
		return utils::MS::cast(rect);
		}

	void handle_observer::rect_client(const utils::math::rect_l& rect) noexcept
		{
		RECT rectangle{utils::MS::cast(rect)};

		// Updates the rect to take into account border/title bar if present
		AdjustWindowRectEx(&rectangle, static_cast<DWORD>(GetWindowLongPtr(pimpl().handle, GWL_STYLE)), false, static_cast<DWORD>(GetWindowLongPtr(pimpl().handle, GWL_EXSTYLE)));

		long width {rectangle.right  - rectangle.left};
		long height{rectangle.bottom - rectangle.top};

		SetWindowPos(pimpl().handle, NULL, rectangle.left, rectangle.top, width, height, SWP_NOZORDER);
		}

	utils::math::vec2l handle_observer::screen_space_to_client_space(const utils::math::vec2l& coords) const noexcept
		{
		POINT output{coords.x(), coords.y()};
		ScreenToClient(pimpl().handle, std::addressof(output));
		return {output.x, output.y};
		}

	utils::math::vec2l handle_observer::client_space_to_screen_space(const utils::math::vec2l& coords) const noexcept
		{
		POINT output{coords.x(), coords.y()};
		ClientToScreen(pimpl().handle, std::addressof(output));
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
	handle_owner& handle_owner::operator=(handle_owner&& move) noexcept = default;
	handle_owner::~handle_owner()
		{
		pimpl().destroy_window_if_exists();
		}

	handle_observer handle_owner::release() noexcept
		{
		HWND handle{pimpl().handle};
		pimpl().handle = nullptr;
		return handle_observer{handle};
		}


	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
		{
		HWND p{FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL)};
		HWND* ret{reinterpret_cast<HWND*>(lParam)};

		if (p)
			{
			// Gets the WorkerW Window after the current one.
			*ret = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
			}
		return true;
		}

	handle_observer find_desktop_wallpaper() noexcept
		{
		// Fetch the Progman window
		HWND progman{FindWindow(L"ProgMan", NULL)};
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