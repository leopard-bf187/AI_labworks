#include "win32_classes.h"



LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	CPlatformWindow* window = nullptr;
	window = reinterpret_cast<CPlatformWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (!window)
		return DefWindowProc(hwnd, msg, wp, lp);

	IPlatformWindowCallback* callback = window->m_callback;
	SNativeHandle* wndHandle = &window->m_window;

	if(!callback || !wndHandle)
		return DefWindowProc(hwnd, msg, wp, lp);

	//if (msg == WM_NCCREATE)
	//{
	//	return 0;
	//}

	//if (msg == WM_NCCALCSIZE)
	//{
	//	return 0;
	//}

	//if (msg == WM_ERASEBKGND)
	//{
	//	return 0;
	//}

	//if (msg == WM_NCHITTEST)
	//{
	//	return 0;
	//}

	if (msg == WM_ACTIVATE || msg == WM_ACTIVATEAPP)
	{
		callback->OnActivate(wndHandle, false);
	}

	if (msg == WM_CREATE)
	{
		callback->OnCreate(wndHandle);
	}

	if (msg == WM_SIZE || msg == WM_SIZING)
	{
		callback->OnSize(wndHandle, 0, 0);
	}

	if (msg == WM_DESTROY)
	{
		callback->OnDestroy(wndHandle);
	}

	if (msg == WM_PAINT)
	{
		PAINTSTRUCT ps{};
		HDC hdc = BeginPaint(hwnd, &ps);
		callback->OnPaint(wndHandle);
		EndPaint(hwnd, &ps);
		return 0;
	}

	if (msg == WM_CHAR)
	{
		callback->OnChar(wndHandle);
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}