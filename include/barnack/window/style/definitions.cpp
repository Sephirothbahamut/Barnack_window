#include "public.h"
#include "exposed_MS.h"


namespace barnack::window
	{
	bool style::pimpl_t::composition_enabled() const noexcept
		{
		BOOL composition_enabled = FALSE;
		bool success = ::DwmIsCompositionEnabled(&composition_enabled) == S_OK;
		return composition_enabled && success;
		}

	namespace win32_style_presets
		{
		inline static constexpr DWORD windowed        {WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX};
		inline static constexpr DWORD aero_borderless {WS_POPUP            | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX};
		inline static constexpr DWORD basic_borderless{WS_POPUP            | WS_THICKFRAME | WS_SYSMENU              | WS_MINIMIZEBOX | WS_MAXIMIZEBOX};
		}

	DWORD style::pimpl_t::select_borderless_style() const noexcept
		{
		return composition_enabled() ? win32_style_presets::aero_borderless : win32_style_presets::basic_borderless;
		}

	void style::pimpl_t::set_shadow(HWND handle, bool enabled) const noexcept
		{
		if (composition_enabled())
			{
			static const MARGINS shadow_state[2]{{ 0,0,0,0 },{ 1,1,1,1 }};
			::DwmExtendFrameIntoClientArea(handle, &shadow_state[enabled]);
			}
		}

	void style::pimpl_t::apply_to_DWORDS(DWORD& win32_style_ex, DWORD& win32_style) const noexcept
		{
		if (style.transparency == transparency_t::layered)
			{
			win32_style_ex |= WS_EX_LAYERED;
			}

		if (style.borders && (*style.borders == false))
			{
			DWORD style{static_cast<DWORD>(win32_style_presets::windowed)};
			win32_style |= style;
			}
		}

	void style::pimpl_t::apply_transparency(HWND handle) const noexcept
		{
		switch (style.transparency)
			{
			case style::transparency_t::composition_attribute:
				details::make_glass_CompositionAttribute(handle);
				break;
			case style::transparency_t::DWM_blurbehind:
				details::make_glass_DWM_BlurBehind      (handle);
				break;
			case style::transparency_t::DWM_transparent:
				details::make_transparent_DWM_BlurBehind(handle);
				break;
			case style::transparency_t::DWM_margin:
				details::make_glass_DWM_margin          (handle);
				break;
			case style::transparency_t::layered:
				details::make_transparent_Layered       (handle, style.alpha);
				break;

			case style::transparency_t::none:
			default:
				break;
			}

		if (style.borders && (*style.borders == false) && style.shadow)
			{
			set_shadow(handle, *style.shadow);
			}
		}

	void style::apply_to(handle_observer& window) const noexcept
		{
		DWORD win32_style_ex{(DWORD)(GetWindowLong(window.get(), GWL_EXSTYLE))};
		DWORD win32_style   {(DWORD)(GetWindowLong(window.get(), GWL_STYLE  ))};

		const pimpl_t pimpl{*this};
		pimpl.apply_to_DWORDS(win32_style_ex, win32_style);

		SetWindowLong(window.get(), GWL_EXSTYLE, win32_style_ex);
		SetWindowLong(window.get(), GWL_STYLE  , win32_style   );

		pimpl.apply_transparency(window.get());

		SetWindowPos(window.get(), 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
		}
	}