#include "lux.hpp"
#include <cstdlib>
#include <SDL2/SDL.h>

/******************************************************************************
 * USERDATA
 *
 * Make the following SDL types available to Lua scripts with the given names.
 */

template <> const char *lux_Type<SDL_Event*>::name = "Event";
template <> const char *lux_Type<SDL_Window*>::name = "Window";
template <> const char *lux_Type<SDL_Surface*>::name = "Surface";
template <> const char *lux_Type<SDL_Renderer*>::name = "Renderer";
template <> const char *lux_Type<SDL_Texture*>::name = "Texture";
template <> const char *lux_Type<SDL_Point*>::name = "Point";
template <> const char *lux_Type<SDL_Rect*>::name = "Rect";
template <> const char *lux_Type<SDL_Color*>::name = "Color";
template <> const char *lux_Type<SDL_Palette*>::name = "Palette";
template <> const char *lux_Type<SDL_PixelFormat*>::name = "PixelFormat";
template <> const char *lux_Type<SDL_RendererInfo*>::name = "RendererInfo";
template <> const char *lux_Type<SDL_DisplayMode*>::name = "DisplayMode";

/******************************************************************************
 * STACK
 *
 * SDL has its own enum types which need their own rules.
 */

template <> void lux_push<SDL_bool>(lua_State *vm, SDL_bool value)
{
	lua_pushboolean(vm, value);
}

template <> SDL_bool lux_to<SDL_bool>(lua_State *vm, int index)
{
	return lua_toboolean(vm, index) ? SDL_TRUE : SDL_FALSE;
}

/******************************************************************************
 * FUNCTIONS
 *
 * Some SDL functions return values through their arguments. A special case.
 */

static int CreateWindowAndRenderer(lua_State *vm)
{
	int w, h, f;
	lux_to(vm, 1, &w, &h, &f);
	SDL_Window *wnd;
	SDL_Renderer *rend;
	if (!SDL_CreateWindowAndRenderer(w, h, f, &wnd, &rend))
	 return lux_push(vm, wnd, rend);
	return 0;
}

static int GetWindowPosition(lua_State *vm)
{
	SDL_Window *wnd = lux_to<SDL_Window*>(vm, 1);
	int x, y;
	SDL_GetWindowPosition(wnd, &x, &y);
	return lux_push(vm, x, y);
}

static int GetWindowSize(lua_State *vm)
{
	SDL_Window *wnd = lux_to<SDL_Window*>(vm, 1);
	int w, h;
	SDL_GetWindowSize(wnd, &w, &h);
	return lux_push(vm, w, h);
}

static int GetRGB(lua_State *vm)
{
	Uint32 pix;
	SDL_PixelFormat *fmt;
	lux_to(vm, 1, &pix, &fmt);
	Uint8 r, g, b;
	SDL_GetRGB(pix, fmt, &r, &g, &b);
	return lux_push(vm, r, g, b);
}

static int GetRGBA(lua_State *vm)
{
	Uint32 pix;
	SDL_PixelFormat *fmt;
	lux_to(vm, 1, &pix, &fmt);
	Uint8 r, g, b, a;
	SDL_GetRGBA(pix, fmt, &r, &g, &b, &a);
	return lux_push(vm, r, g, b, a);
}

static int PixelFormatEnumToMasks(lua_State *vm)
{
	Uint32 fmt = lux_to<Uint32>(vm, 1);
	int bpp;
	Uint32 r, g, b, a;
	if (SDL_PixelFormatEnumToMasks(fmt, &bpp, &r, &g, &b, &a))
	 return lux_push(vm, bpp, r, g, b, a);
	return 0;
}

static int GetRenderDrawColor(lua_State *vm)
{
	Uint8 r, g, b, a;
	SDL_Renderer *rend = lux_to<SDL_Renderer*>(vm, 1);
	if (!SDL_GetRenderDrawColor(rend, &r, &g, &b, &a))
	 return lux_push(vm, r, g, b, a);
	return 0;
}

/******************************************************************************
 * CONSTANTS
 *
 * Access numerical constants through the __index metamethod and the standard
 * binary search C routine. Must be alphabetical.
 */

#define LX(par) {#par, SDL_##par},

static lux_Entry params [] =
{
	LX(MESSAGEBOX_ERROR)
	LX(MESSAGEBOX_INFORMATION)
	LX(MESSAGEBOX_WARNING)

	LX(PIXELFORMAT_ABGR1555)
	LX(PIXELFORMAT_ABGR4444)
	LX(PIXELFORMAT_ABGR8888)
	LX(PIXELFORMAT_ARGB1555)
	LX(PIXELFORMAT_ARGB2101010)
	LX(PIXELFORMAT_ARGB4444)
	LX(PIXELFORMAT_ARGB8888)
	LX(PIXELFORMAT_BGR24)
	LX(PIXELFORMAT_BGR555)
	LX(PIXELFORMAT_BGR565)
	LX(PIXELFORMAT_BGR888)
	LX(PIXELFORMAT_BGRA4444)
	LX(PIXELFORMAT_BGRA5551)
	LX(PIXELFORMAT_BGRA8888)
	LX(PIXELFORMAT_BGRX8888)
	LX(PIXELFORMAT_INDEX1LSB)
	LX(PIXELFORMAT_INDEX1MSB)
	LX(PIXELFORMAT_INDEX4LSB)
	LX(PIXELFORMAT_INDEX4MSB)
	LX(PIXELFORMAT_INDEX8)
	LX(PIXELFORMAT_IYUV)
	LX(PIXELFORMAT_RGB24)
	LX(PIXELFORMAT_RGB332)
	LX(PIXELFORMAT_RGB444)
	LX(PIXELFORMAT_RGB555)
	LX(PIXELFORMAT_RGB565)
	LX(PIXELFORMAT_RGB888)
	LX(PIXELFORMAT_RGBA4444)
	LX(PIXELFORMAT_RGBA5551)
	LX(PIXELFORMAT_RGBA8888)
	LX(PIXELFORMAT_RGBX8888)
	LX(PIXELFORMAT_UNKNOWN)
	LX(PIXELFORMAT_UYVY)
	LX(PIXELFORMAT_YUY2)
	LX(PIXELFORMAT_YV12)
	LX(PIXELFORMAT_YVYU)

	LX(QUIT)

	LX(WINDOWPOS_CENTERED)
	LX(WINDOWPOS_UNDEFINED)

	LX(WINDOW_ALLOW_HIGHDPI)
	LX(WINDOW_BORDERLESS)
	LX(WINDOW_FOREIGN)
	LX(WINDOW_FULLSCREEN)
	LX(WINDOW_FULLSCREEN_DESKTOP)
	LX(WINDOW_HIDDEN)
	LX(WINDOW_INPUT_FOCUS)
	LX(WINDOW_INPUT_GRABBED)
	LX(WINDOW_MAXIMIZED)
	LX(WINDOW_MINIMIZED)
	LX(WINDOW_MOUSE_FOCUS)
	LX(WINDOW_OPENGL)
	LX(WINDOW_RESIZABLE)
	LX(WINDOW_SHOWN)
};

static int index(lua_State *vm)
{
	const char *key = luaL_checkstring(vm, 2);
	int value = lux_Entry::mask(key, params, lengthof(params));
	if (value < 0) return luaL_error(vm, "%s undefined", key);
	lua_pushinteger(vm, value);
	return 1;
}

/******************************************************************************
 * REGISTRY
 *
 * The functions which will be exported to the Lua state machine.
 */

#define lx(fun) {#fun, lux_wrap(SDL_##fun)},

static luaL_Reg funcs [] =
{
	// Display and Window Management //

	lx(CreateWindow)
	{"CreateWindowAndRenderer", CreateWindowAndRenderer},
	lx(CreateWindowFrom)
	lx(DestroyWindow)
	lx(DisableScreenSaver)
	lx(EnableScreenSaver)
	lx(GetClosestDisplayMode)
	lx(GetCurrentDisplayMode)
	lx(GetCurrentVideoDriver)
	lx(GetDesktopDisplayMode)
	lx(GetDisplayBounds)
	lx(GetDisplayMode)
	lx(GetDisplayName)
	lx(GetNumDisplayModes)
	lx(GetNumVideoDisplays)
	lx(GetNumVideoDrivers)
	lx(GetVideoDriver)
	lx(GetWindowBrightness)
	lx(GetWindowData)
	lx(GetWindowDisplayIndex)
	lx(GetWindowDisplayMode)
	lx(GetWindowFlags)
	lx(GetWindowFromID)
	lx(GetWindowGammaRamp)
	lx(GetWindowGrab)
	lx(GetWindowID)
	lx(GetWindowMaximumSize)
	lx(GetWindowMinimumSize)
	lx(GetWindowPixelFormat)
	{"GetWindowPosition", GetWindowPosition},
	{"GetWindowSize", GetWindowSize},
	lx(GetWindowSurface)
	lx(GetWindowTitle)
//	lx(GetWindowWMInfo)
	lx(HideWindow)
	lx(IsScreenSaverEnabled)
	lx(MaximizeWindow)
	lx(MinimizeWindow)
	lx(RaiseWindow)
	lx(RestoreWindow)
	lx(SetWindowBordered)
	lx(SetWindowBrightness)
	lx(SetWindowData)
	lx(SetWindowDisplayMode)
	lx(SetWindowFullscreen)
	lx(SetWindowGammaRamp)
	lx(SetWindowGrab)
	lx(SetWindowIcon)
	lx(SetWindowMaximumSize)
	lx(SetWindowMinimumSize)
	lx(SetWindowPosition)
	lx(SetWindowSize)
	lx(SetWindowTitle)
//	lx(ShowMessageBox)
	lx(ShowSimpleMessageBox)
	lx(ShowWindow)
	lx(UpdateWindowSurface)
	lx(UpdateWindowSurfaceRects)

	// Surface Creation and Simple Drawing //

	lx(BlitScaled)
	lx(BlitSurface)
	lx(ConvertPixels)
	lx(ConvertSurfaceFormat)
	lx(CreateRGBSurface)
	lx(CreateRGBSurfaceFrom)
	lx(FillRect)
	lx(FillRects)
	lx(FreeSurface)
	lx(GetClipRect)
	lx(GetColorKey)
	lx(GetSurfaceAlphaMod)
//	lx(GetSurfaceBlendMode)
	lx(GetSurfaceColorMod)
//	lx(LoadBMP)
//	lx(LoadBMP_RW)
	lx(LockSurface)
	lx(LowerBlit)
	lx(LowerBlitScaled)
//	lx(MUSTLOCK)
//	lx(SaveBMP)
//	lx(SaveBMP_RW)
	lx(SetClipRect)
	lx(SetColorKey)
	lx(SetSurfaceAlphaMod)
//	lx(SetSurfaceBlendMode)
	lx(SetSurfaceColorMod)
	lx(SetSurfacePalette)
	lx(SetSurfaceRLE)
	lx(UnlockSurface)

	// 2D Accelerated Rendering //

	lx(CreateRenderer)
	lx(CreateSoftwareRenderer)
	lx(CreateTexture)
	lx(CreateTextureFromSurface)
	lx(DestroyRenderer)
	lx(DestroyTexture)
	lx(GetNumRenderDrivers)
//	lx(GetRenderDrawBlendMode)
	{"GetRenderDrawColor", GetRenderDrawColor},
//	lx(GetRenderDriverInfo)
	lx(GetRenderTarget)
	lx(GetRenderer)
	lx(GetRendererInfo)
//	lx(GetRendererOutputSize)
//	lx(GetTextureAlphaMod)
//	lx(GetTextureBlendMode)
//	lx(GetTextureColorMod)
//	lx(LockTexture)
//	lx(QueryTexture)
	lx(RenderClear)
//	lx(RenderCopy)
//	lx(RenderCopyEx)
	lx(RenderDrawLine)
	lx(RenderDrawLines)
	lx(RenderDrawPoint)
	lx(RenderDrawPoints)
	lx(RenderDrawRect)
	lx(RenderDrawRects)
	lx(RenderFillRect)	
	lx(RenderFillRects)
	lx(RenderGetClipRect)
//	lx(RenderGetLogicalSize)
//	lx(RenderGetScale)
	lx(RenderGetViewport)
//	lx(RenderIsClipEnabled)
	lx(RenderPresent)
//	lx(RenderReadPixels)
	lx(RenderSetClipRect)
	lx(RenderSetLogicalSize)
	lx(RenderSetScale)
	lx(RenderSetViewport)
	lx(RenderTargetSupported)
//	lx(SetRenderDrawBlendMode)
	lx(SetRenderDrawColor)
	lx(SetRenderTarget)
	lx(SetTextureAlphaMod)
//	lx(SetTextureBlendMode)
	lx(SetTextureColorMod)
//	lx(UnlockTexture)
//	lx(UpdateTexture)
	lx(UpdateYUVTexture)

	// Pixel Formats and Conversion Routines //

	lx(AllocFormat)
	lx(AllocPalette)
	lx(CalculateGammaRamp)
	lx(FreeFormat)
	lx(FreePalette)
	lx(GetPixelFormatName)
	{"GetRGB", GetRGB},
	{"GetRGBA", GetRGBA},
	lx(MapRGB)
	lx(MapRGBA)
	lx(MasksToPixelFormatEnum)
	{"PixelFormatEnumToMasks", PixelFormatEnumToMasks},
	lx(SetPaletteColors)
	lx(SetPixelFormatPalette)

	// Input Events //

//	lx(AddEventWatch)
//	lx(DelEventWatch)
	lx(EventState)
//	lx(FilterEvents)
	lx(FlushEvent)
	lx(FlushEvents)
//	lx(GetEventFilter)
//	lx(GetNumTouchDevices)
//	lx(GetNumTouchFingers)
//	lx(GetTouchDevice)
//	lx(GetTouchFinger)
	lx(HasEvent)
	lx(HasEvents)
//	lx(LoadDollarTemplates)
//	lx(PeepEvents)
	lx(PollEvent)
	lx(PushEvent)
//	lx(QuitRequested)
//	lx(RecordGesture)
	lx(RegisterEvents)
//	lx(SaveAllDollarTemplates)
//	lx(SaveDollarTemplate)
//	lx(SetEventFilter)
	lx(WaitEvent)
	lx(WaitEventTimeout)

	// Timer Support //

//	lx(AddTimer)
	lx(Delay)
//	lx(GetPerformanceCounter)
//	lx(GetPerformanceFrequency)
	lx(GetTicks)
//	lx(RemoveTimer)

	// Terminal //

	{NULL, NULL}
};

/******************************************************************************
 * INITIALIZE
 *
 * Setup the Lua state to use the binding to SDL done above.
 */

int luxopen_SDL(lua_State *vm)
{
	if (!luaL_newmetatable(vm, "__SDL"))
	{
	 fprintf(stderr, "SDL functions loaded already\n");
	 return 0;
	}

	// Access constants through metatable __index

	lua_pushliteral(vm, "__index");
	lua_pushcfunction(vm, index);
	lua_settable(vm, -3);

	// Access functions through table "SDL"

	luaL_newlib(vm, funcs);
	luaL_setmetatable(vm, "__SDL");
	lua_setglobal(vm, "SDL");

	// Register data structures with fields

	lux_Record<SDL_Event>::open(vm);
	lux_member(SDL_Event, type);

	lux_Record<SDL_Rect>::open(vm);
	lux_member(SDL_Rect, x);
	lux_member(SDL_Rect, y);
	lux_member(SDL_Rect, w);
	lux_member(SDL_Rect, h);

	lux_Record<SDL_Point>::open(vm);
	lux_member(SDL_Point, x);
	lux_member(SDL_Point, y);
	
	lux_Record<SDL_Surface>::open(vm);
	lux_member(SDL_Surface, format);
	lux_member(SDL_Surface, w);
	lux_member(SDL_Surface, h);
	lux_member(SDL_Surface, pitch);

	lux_Record<SDL_PixelFormat>::open(vm);
	lux_member(SDL_PixelFormat, format);
//	lux_member(SDL_PixelFormat, palette);
	lux_member(SDL_PixelFormat, BitsPerPixel);
	lux_member(SDL_PixelFormat, BytesPerPixel);
	lux_member(SDL_PixelFormat, Rmask);
	lux_member(SDL_PixelFormat, Gmask);
	lux_member(SDL_PixelFormat, Bmask);
	lux_member(SDL_PixelFormat, Amask);

	lux_Record<SDL_Color>::open(vm);
	lux_member(SDL_Color, r);
	lux_member(SDL_Color, g);
	lux_member(SDL_Color, b);
	lux_member(SDL_Color, a);

	lux_Record<SDL_DisplayMode>::open(vm);
	lux_member(SDL_DisplayMode, format);
	lux_member(SDL_DisplayMode, w);
	lux_member(SDL_DisplayMode, h);
	lux_member(SDL_DisplayMode, refresh_rate);
	lux_member(SDL_DisplayMode, driverdata);

	lua_pop(vm, 1);

	return 0;
}

/******************************************************************************
 * TEST
 *
 * Run a script to check if everything above works properly
 */

int main(int argc, char **argv)
{
	// Initialise SDL

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL_Init: %s", SDL_GetError());
		return EXIT_SUCCESS;
	}
	else atexit(SDL_Quit);

	// Test that our constants are ordered alphabetically

	if (!lux_Entry::check(params, lengthof(params)))
	{
		fputs("FIXME!!!", stderr);
	}

	// Run a Lua script to test the binding	

	lua_State *vm = luaL_newstate();
	lux_throwatpanic(vm);
	luaL_openlibs(vm);
	luxopen_array(vm);
	luxopen_SDL(vm);
	luaL_dofile(vm, "test8.lua");
	lux_stackdump(vm);
	lua_close(vm);
}

