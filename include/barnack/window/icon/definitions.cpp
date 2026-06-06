#include "public.h"
#include "exposed_MS.h"

#include "../handle_wrapper/exposed_MS.h"


namespace barnack::window
	{
	icon::pimpl_t::pimpl_t() noexcept : pimpl_t{nullptr} {}
	icon::pimpl_t::pimpl_t(HICON handle) noexcept : handle{handle} {}

	icon::pimpl_t::pimpl_t(pimpl_t&& move) noexcept : pimpl_t{move.handle} { move.handle = nullptr; }
	icon::pimpl_t& icon::pimpl_t::operator=(pimpl_t&& move) noexcept
		{
		destroy_if_exists();
		handle = move.handle; 
		move.handle = nullptr; 
		return *this;
		}
	icon::pimpl_t::~pimpl_t() {}

	void icon::pimpl_t::destroy_if_exists() noexcept
		{
		if (handle) 
			{
			::DestroyIcon(handle);
			handle = nullptr;
			}
		}




	icon::icon() noexcept
		{
		static_assert(sizeof(icon::pimpl_t) == sizeof(pimpl_storage));
		new (pimpl_storage.data()) icon::pimpl_t;
		}

	icon::icon(const utils::matrix<utils::graphics::colour::rgba_u>& pixels) noexcept : icon{}
		{
		std::vector<uint8_t> adjusted_pixels(pixels.size() * 4);
		for (size_t i{0}; i < pixels.size(); i++)
			{
			const auto& pixel{pixels[i]};

			const size_t pixel_as_channel_index{i * 4};
			auto& adjusted_pixel_r{adjusted_pixels[pixel_as_channel_index + 2]};
			auto& adjusted_pixel_g{adjusted_pixels[pixel_as_channel_index + 1]};
			auto& adjusted_pixel_b{adjusted_pixels[pixel_as_channel_index + 0]};
			auto& adjusted_pixel_a{adjusted_pixels[pixel_as_channel_index + 3]};

			adjusted_pixel_r = pixel.r();
			adjusted_pixel_g = pixel.g();
			adjusted_pixel_b = pixel.b();
			adjusted_pixel_a = pixel.a();
			}

		pimpl().handle = CreateIcon(GetModuleHandleW(nullptr),
			static_cast<int>(pixels.sizes().x()),
			static_cast<int>(pixels.sizes().y()),
			1,
			32,
			nullptr,
			adjusted_pixels.data());
		}

	icon::~icon()
		{
		pimpl().~pimpl_t();
		}

	const icon::pimpl_t& icon::pimpl() const noexcept { return *std::launder(reinterpret_cast<const pimpl_t*>(pimpl_storage.data())); }
	      icon::pimpl_t& icon::pimpl()       noexcept { return *std::launder(reinterpret_cast<      pimpl_t*>(pimpl_storage.data())); }



	icon::icon           (icon&& move) noexcept { pimpl() = std::move(move.pimpl()); }
	icon& icon::operator=(icon&& move) noexcept { pimpl() = std::move(move.pimpl()); return *this; }








	bool icon::is_open() const noexcept { return pimpl().handle; }
	}