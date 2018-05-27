#include <SDL2/SDL_config.h>
#include "Window.hpp"

namespace sys::wm
{
	Info::Info(SDL_Window *window) : target(nullptr)
	{
		if (SDL_GetWindowWMInfo(window, this))
		{
			target = window;
		}
	}

	SDL_Window *Info::Sub(SDL_Rect const &rect)
	{
	    if (target) switch (subsystem)
	    {
	    default:
	    case SDL_SYSWM_UNKNOWN:
		break;

	    #ifdef SDL_VIDEO_DRIVER_X11
	    case SDL_SYSWM_X11:
	    {
	    	struct WindowAttributes : XWindowAttributes
	    	{
				WindowAttributes(Display *display, Window window)
				{
					status = XGetWindowAttributes(display, window, this);
				}

				Status status;

			} wa(info.x11.display, info.x11.window);

			if (Success == wa.status)
			{
		    	union
				{
					Window window;
					void *data;
				};
				unsigned long const pixel = BlackPixelOfScreen(wa.screen);
				window = XCreateSimpleWindow
				(
				 info.x11.display,
				 info.x11.window,
				 rect.x,
				 rect.y,
				 rect.w,
				 rect.h,
				 0,
				 pixel,
				 pixel
				);
				XMapRaised(info.x11.display, window);
				return SDL_CreateWindowFrom(data);
			}
	    }
	    break;
	    #endif

	    #ifdef SDL_VIDEO_DRIVER_WINDOWS
	    case SDL_SYSWM_WINDOWS:
	    {
			static struct WindowClass : WNDCLASS
			{
				ATOM atom;

				WindowClass()
				{
				    style = 0;
				    lpfnWndProc = WndProc;
				    cbClsExtra = 0;
				    cbWndExtra = 0;
				    hInstance = GetModuleHandle(nullptr);
				    hIcon = LoadIcon(nullptr, IDI_APPLICATION);
				    hCursor = LoadCursor(nullptr, IDC_ARROW);
				    hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
				    lpszMenuName = nullptr;
				    lpszClassName = "SDL Child Window";

				    atom = RegisterClass(this);
				}

			private:

				static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
				{
				    switch (msg)
				    {
				    case WM_CLOSE:
				        DestroyWindow(hwnd);
				        break;
				    case WM_DESTROY:
				        PostQuitMessage(0);
				        break;
				    default:
				        return DefWidnowProc(hwnd, msg, wParam, lParam);
				    }
				    return 0;
				}

			} wc;

			if (wc.atom)
			{
				union
				{
				    HWND window;
				    void *data;
				};
				window = CreateWindow
				(
				 wc.lpszClassName,
				 "",
				 WS_CHILD,
				 rect.x,
				 rect.y,
				 rect.w,
				 rect.h,
				 info.win.window,
				 wc.lpszMenuName,
				 info.win.hinstance,
				 nullptr
				);
				ShowWindow(window, SW_SHOW);
				return SDL_CreateWindowFrom(data);
			}
	    }
	    break;
	    #endif
	    }
	    return nullptr;
	}
}
