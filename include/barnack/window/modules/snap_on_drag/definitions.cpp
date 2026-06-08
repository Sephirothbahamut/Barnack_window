#include "public.h"

#include <utils/MS/raw/cast.h>

#include "../../handle_wrapper/exposed_MS.h"

namespace barnack::window::module
	{
	struct lparam_pass
		{
		HWND caller_hwnd;
		std::vector<utils::math::rect_l>& rects;
		};

	BOOL CALLBACK enumWindowCallback(HWND hwnd, LPARAM lparam) noexcept
		{
		lparam_pass& lparams{*reinterpret_cast<utils::observer_ptr<lparam_pass>>(lparam)};
		HWND caller_hwnd{lparams.caller_hwnd};
		auto& rects{lparams.rects};

		window::handle_observer wrapped{hwnd};

		bool different{caller_hwnd != wrapped.get()};
		bool visible{wrapped.is_visible()};
		bool parentless{!wrapped.parent().get()};

		if (caller_hwnd != wrapped.get() && wrapped.is_visible() && parentless)
			{
			rects.push_back(wrapped.rect_window());
			}
		return TRUE;
		}

	void get_windows(HWND caller_hwnd, std::vector<utils::math::rect_l>& rects) noexcept
		{
		lparam_pass lparam_pass{caller_hwnd, rects};
		EnumWindows(enumWindowCallback, reinterpret_cast<LPARAM>(&lparam_pass)); 
		}



	void eval_delta(const long& snap_max_distance, const long& target, const long& other, long& closest_distance, long& closest)
		{
		long distance{std::abs(target - other)};
		if (distance <= snap_max_distance)
			{
			if (closest_distance == -1 || distance < closest_distance)
				{
				closest_distance = distance;
				closest = other;
				}
			}
		}





	void snap_on_drag::move_begin(window::base& window) noexcept
		{
		get_windows(window.get(), existing_windows_rects);
		target = result_previous = window.rect_window();
		}

	utils::math::rect_l snap_on_drag::move(const utils::math::rect_l& attempted_result) noexcept
		{
		const utils::math::vec2l delta{attempted_result.position() - result_previous.position()};
		target.proxy_position() += delta;


		utils::math::rect<long> closest;
		utils::math::rect<long> distance{-1, -1, -1, -1};

		for (const auto& rect : existing_windows_rects)
			{
			eval_delta(snap_max_distance, target.ll(), rect.rr(), distance.ll(), closest.ll());
			eval_delta(snap_max_distance, target.up(), rect.dw(), distance.up(), closest.up());
			eval_delta(snap_max_distance, target.rr(), rect.ll(), distance.rr(), closest.rr());
			eval_delta(snap_max_distance, target.dw(), rect.up(), distance.dw(), closest.dw());
			}

		utils::math::rect<bool> snap
			{
			distance.ll() != -1 && (distance.rr() == -1 || distance.ll() < distance.rr()),
			distance.up() != -1 && (distance.dw() == -1 || distance.up() < distance.dw()),
			distance.rr() != -1 && (distance.ll() == -1 || distance.rr() < distance.ll()),
			distance.dw() != -1 && (distance.up() == -1 || distance.dw() < distance.up()),
			};

		if (snap.ll() || snap.up() || snap.rr() || snap.dw())
			{
			utils::math::rect<long> result_new{target};

			if (snap.ll()) { result_new.proxy_x() = closest.ll(); }
			if (snap.up()) { result_new.proxy_y() = closest.up(); }
			if (snap.rr()) { result_new.proxy_x() = closest.rr() - target.width(); }
			if (snap.dw()) { result_new.proxy_y() = closest.dw() - target.height(); }

			result_previous = result_new;
			return result_new;
			}
		else
			{
			result_previous = attempted_result;
			return attempted_result;
			}
		}

	void snap_on_drag::move_end() noexcept
		{
		existing_windows_rects.clear();
		}
	}