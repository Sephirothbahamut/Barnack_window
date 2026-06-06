#include "public.h"
#include "exposed_MS.h"

#include <utils/string.h>

#include "../handle_wrapper/exposed_MS.h"

namespace barnack::window
	{
	base factory::create()
		{
		HWND handle = ::CreateWindowExW
			(
			0, //style_ex
			window_class_name, 
			utils::string::cast<wchar_t>(title).c_str(),
			WS_OVERLAPPEDWINDOW, //style
			position.has_value() ? position.value().x() : CW_USEDEFAULT,
			position.has_value() ? position.value().y() : CW_USEDEFAULT,
			size    .has_value() ? size    .value().x() : CW_USEDEFAULT,
			size    .has_value() ? size    .value().y() : CW_USEDEFAULT,
			parent_window ? parent_window->pimpl().handle : nullptr,
			nullptr, nullptr, this
			);

		base ret{handle};
		ret.callbacks = callbacks;
		return ret;
		}
	}