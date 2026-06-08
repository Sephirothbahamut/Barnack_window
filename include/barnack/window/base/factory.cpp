#include "public.h"
#include "exposed_MS.h"

#include <utils/string.h>

#include "../handle_wrapper/exposed_MS.h"
#include "../style/exposed_MS.h"

namespace barnack::window
	{
	base factory::create()
		{
		DWORD win32_style_ex{0};
		DWORD win32_style{WS_OVERLAPPEDWINDOW};

		if (style) 
			{
			window::style::pimpl_t{*style}.apply_to_DWORDS(win32_style_ex, win32_style);
			}

		HWND handle = ::CreateWindowExW
			(
			win32_style_ex,
			window_class_name, 
			utils::string::cast<wchar_t>(title).c_str(),
			win32_style,
			position.has_value() ? position.value().x() : CW_USEDEFAULT,
			position.has_value() ? position.value().y() : CW_USEDEFAULT,
			size    .has_value() ? size    .value().x() : CW_USEDEFAULT,
			size    .has_value() ? size    .value().y() : CW_USEDEFAULT,
			parent_window ? parent_window->get() : nullptr,
			nullptr, nullptr, this
			);

		if (style)
			{
			window::style::pimpl_t{*style}.apply_transparency(handle);
			}

		base ret{handle};
		ret.callbacks = callbacks;

		if (icon) { icon->assign_to(ret); }


		return ret;
		}
	}