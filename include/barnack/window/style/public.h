#pragma once

#include "../handle_wrapper.h"

#include <functional>

#include <utils/math/vec.h>
#include <utils/math/rect.h>

#include <utils/matrix.h>
#include <utils/graphics/colour.h>


namespace barnack::window
	{
	struct style
		{
		struct pimpl_t;

		enum class transparency_t
			{
			//Working blurred transparency
			composition_attribute,
			//Not actually blurred, just partially transparent
			DWM_blurbehind,
			//Fully see through
			DWM_transparent,
			//Doesn't seem to work anymore :(
			DWM_margin,
			//"manual" non-blurred transparency, uses custom alpha value
			layered,
			none
			};
		
		transparency_t transparency{transparency_t::none};
		//Doesn't seem to work anymore :(
		std::optional<bool> borders{std::nullopt};
		//Doesn't seem to work anymore :(
		std::optional<bool> shadow {std::nullopt};
		/// <summary> Only used by layered transparency </summary>
		uint8_t alpha{255};

		void apply_to(handle_observer& window) const noexcept;
		};

	}

#ifdef utils_implementation
#include "exposed_MS.h"
#endif