#include "public.h"
#include "exposed_MS.h"

#include "../handle_wrapper/exposed_MS.h"


namespace barnack::window
	{
	//opengl::pimpl_t::pimpl_t() noexcept :
	//	handle_window        {nullptr},
	//	handle_device_context{nullptr},
	//	handle_opengl_context{nullptr}
	//	{}
	//opengl::pimpl_t::pimpl_t(HWND handle_window, HDC handle_device_context, HGLRC handle_opengl_context) noexcept :
	//	handle_window{handle_window},
	//	handle_device_context{handle_device_context},
	//	handle_opengl_context{handle_opengl_context}
	//	{
	//	}
	//opengl::pimpl_t::pimpl_t(pimpl_t&& move) noexcept : 
	//	pimpl_t
	//		{
	//		move.handle_window,
	//		move.handle_device_context,
	//		move.handle_opengl_context
	//		}
	//	{ 
	//	move.handle_window         = nullptr;
	//	move.handle_device_context = nullptr;
	//	move.handle_opengl_context = nullptr;
	//	}
	//opengl::pimpl_t& opengl::pimpl_t::operator=(pimpl_t&& move) noexcept
	//	{
	//	destroy_if_exists();
	//	
	//	handle_window         = move.handle_window        ;
	//	handle_device_context = move.handle_device_context;
	//	handle_opengl_context = move.handle_opengl_context;
	//	                        move.handle_window         = nullptr;
	//	                        move.handle_device_context = nullptr;
	//	                        move.handle_opengl_context = nullptr;
	//
	//	return *this;
	//	}
	//opengl::pimpl_t::~pimpl_t() {}
	//
	//void opengl::pimpl_t::destroy_if_exists() noexcept
	//	{
	//	if (handle_opengl_context) { wglDeleteContext(handle_opengl_context)        ; handle_opengl_context = nullptr; }
	//	if (handle_device_context) { ReleaseDC(handle_window, handle_device_context); handle_device_context = nullptr; }
	//	}
	//
	//
	//void initialize_window_for_opengl(HWND hwnd)
	//	{
	//	HDC hDC{GetDC(hwnd)};
	//	
	//	PIXELFORMATDESCRIPTOR pfd
	//		{
	//		.nSize     {sizeof(pfd)},
	//		.nVersion  {1},
	//		.dwFlags   {PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL /*| flags*/},
	//		.iPixelType{PFD_TYPE_RGBA},
	//		.cColorBits{32}
	//		};
	//
	//	int pf{ChoosePixelFormat(hDC, &pfd)};
	//	if (pf == 0) 
	//		{
	//		throw std::runtime_error{"ChoosePixelFormat() failed: Cannot find a suitable pixel format."};
	//		}
	//
	//	if (SetPixelFormat(hDC, pf, &pfd) == FALSE) 
	//		{
	//		throw std::runtime_error{"SetPixelFormat() failed: Cannot set format specified."};
	//		}
	//
	//	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	//
	//	ReleaseDC(hwnd, hDC);
	//	}
	//
	//
	//opengl::opengl(handle_observer& handle_observer)
	//	{
	//	static_assert(sizeof(opengl::pimpl_t) == sizeof(pimpl_storage));
	//	new (pimpl_storage.data()) opengl::pimpl_t;
	//
	//	initialize_window_for_opengl(handle_observer.pimpl().handle);
	//
	//	pimpl().handle_window         = handle_observer.pimpl().handle;
	//	pimpl().handle_device_context = GetDC(handle_observer.pimpl().handle);
	//	pimpl().handle_opengl_context = wglCreateContext(pimpl().handle_device_context);
	//	}
	//
	//opengl::~opengl()
	//	{
	//	pimpl().~pimpl_t();
	//	}
	//
	//const opengl::pimpl_t& opengl::pimpl() const noexcept { return *std::launder(reinterpret_cast<const pimpl_t*>(pimpl_storage.data())); }
	//      opengl::pimpl_t& opengl::pimpl()       noexcept { return *std::launder(reinterpret_cast<      pimpl_t*>(pimpl_storage.data())); }
	//
	//
	//
	//opengl::opengl           (opengl&& move) noexcept { pimpl() = std::move(move.pimpl()); }
	//opengl& opengl::operator=(opengl&& move) noexcept { pimpl() = std::move(move.pimpl()); return *this; }
	//
	//
	//
	//
	//
	//
	//
	//
	//bool opengl::is_valid() const noexcept { return pimpl().handle_window && pimpl().handle_device_context && pimpl().handle_opengl_context; }
	//
	//
	//
	//
	//void opengl::make_active() const noexcept
	//	{
	//	wglMakeCurrent(pimpl().handle_device_context, pimpl().handle_opengl_context);
	//	}
	//
	//bool opengl::is_active() const noexcept
	//	{
	//	return wglGetCurrentContext() == pimpl().handle_opengl_context;
	//	}
	}