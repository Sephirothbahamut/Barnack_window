#include "public.h"
#include "exposed_MS.h"

#include "../handle_wrapper/exposed_MS.h"


namespace barnack::window
	{
	LRESULT __stdcall window_procedure(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
		{
		auto* window_instance_ptr{get_window_ptr(handle)};
		if (!window_instance_ptr) { DefWindowProc(handle, msg, wparam, lparam); }
		auto& window_instance{*window_instance_ptr};

		switch (msg)
			{
			case WM_CLOSE:
				{
				if (window_instance.callbacks.on_close)
					{
					const auto result{window_instance.callbacks.on_close(window_instance)};
					if (result != simple_event_return_type::use_default_handler)
						{
						return 0;
						}
					}
				window_instance.close();
				return 0;
				} break;
			case WM_NCHITTEST:
				{
				if (window_instance.callbacks.on_hit_test)
					{
					utils::math::vec2l coords{GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)};
					const auto result{window_instance.callbacks.on_hit_test(window_instance, coords)};
					switch (result)
						{
						case hit_result::resize_right       : return HTRIGHT      ;
						case hit_result::resize_left        : return HTLEFT       ;
						case hit_result::resize_up          : return HTTOP        ;
						case hit_result::resize_down        : return HTBOTTOM     ;
						case hit_result::resize_up_right    : return HTTOPRIGHT   ;
						case hit_result::resize_up_left     : return HTTOPLEFT    ;
						case hit_result::resize_down_right  : return HTBOTTOMRIGHT;
						case hit_result::resize_down_left   : return HTBOTTOMLEFT ;
						case hit_result::client             : return HTCLIENT     ;
						case hit_result::hole               : return HTNOWHERE    ;
						case hit_result::same_thread_hole   : return HTTRANSPARENT;
						case hit_result::drag               : return HTCAPTION    ;
						}
					}
				} break;

			case WM_ENTERSIZEMOVE:
				{
				if (window_instance.callbacks.on_move_begin)
					{
					const auto result{window_instance.callbacks.on_move_begin(window_instance)};
					if (result == simple_event_return_type::handled) { return 0; }
					}
				} break;
			case WM_MOVING:
				{
				if (window_instance.callbacks.on_move)
					{
					RECT& rect{*reinterpret_cast<utils::observer_ptr<RECT>>(lparam)};
					
					const auto result{window_instance.callbacks.on_move(window_instance, {rect.left, rect.top, rect.right, rect.bottom})};
					if (result) 
						{
						rect = {result->ll(), result->up(), result->rr(), result->dw()};
						return TRUE; 
						}
					}
				} break;
			case WM_EXITSIZEMOVE:
				{
				if (window_instance.callbacks.on_move_end)
					{
					const auto result{window_instance.callbacks.on_move_end(window_instance)};
					if (result == simple_event_return_type::handled) { return 0; }
					}
				} break;
			}

		return DefWindowProc(handle, msg, wparam, lparam);
		}
	}