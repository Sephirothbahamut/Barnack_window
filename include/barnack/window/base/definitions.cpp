#include "public.h"
#include "exposed_MS.h"

#include "../handle_wrapper/exposed_MS.h"
#include "../icon/exposed_MS.h"


namespace barnack::window
	{
	initializer::initializer()
		{
		auto hBrush{CreateSolidBrush(RGB(0, 0, 0))};
		static const WNDCLASSEXW wcx
			{
			.cbSize        {sizeof(wcx)                     },
			.style         {CS_HREDRAW | CS_VREDRAW         },
			.lpfnWndProc   {std::addressof(window_procedure)},
			.hInstance     {nullptr                         },
			.hCursor       {::LoadCursor(nullptr, IDC_ARROW)},
			.hbrBackground {hBrush                          },
			.lpszClassName {window_class_name},
			};
		RegisterClassExW(&wcx);
		}
	initializer::~initializer() { UnregisterClass(window_class_name, nullptr); }





	void set_window_ptr(HWND handle, utils::observer_ptr<base> leaf_pointer)
		{
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(leaf_pointer));
		}
	utils::observer_ptr<base> get_window_ptr(HWND handle)
		{
		LONG_PTR user_data{GetWindowLongPtr(handle, GWLP_USERDATA)};
		return reinterpret_cast<utils::observer_ptr<base>>(user_data);
		}

	base::base() noexcept = default;
	base::base(void* system_window_handle) noexcept : handle_owner(system_window_handle)
		{
		set_window_ptr(handle_owner::pimpl().handle, this);
		}
	base::base(base&& move) noexcept : handle_owner{std::move(move)}
		{
		set_window_ptr(handle_owner::pimpl().handle, this);
		}
	base& base::operator=(base&& move) noexcept 
		{
		handle_owner::operator=(std::move(move));
		set_window_ptr(handle_owner::pimpl().handle, this);
		return *this;
		}

	bool base::poll_event() const
		{
		MSG msg;
		bool ret{static_cast<bool>(PeekMessage(&msg, handle_owner::pimpl().handle, 0, 0, PM_REMOVE))};
		if (ret)
			{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			return true;
			}
		else { return false; }
		}

	void base::wait_event() const
		{
		MSG msg;
		bool ret{static_cast<bool>(GetMessage(&msg, handle_owner::pimpl().handle, 0, 0))}; //true if WM_QUIT, false otherwise
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		}










	void base::style_proxy::set_icon(const window::icon& icon) noexcept
		{
		if (icon.pimpl().handle)
			{
			SendMessageW(base.pimpl().handle, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon.pimpl().handle));
			SendMessageW(base.pimpl().handle, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon.pimpl().handle));
			}
		}
		
	base::style_proxy::style_proxy(window::base& base) noexcept : base{base} {}

	[[nodiscard]] base::style_proxy base::style() noexcept { return {*this}; }
	}