#include "Window.h"
#include <cassert>

bool Window::initialized = false;

void Window::Initialize() noexcept
{
	// check if window class is already initialized
	assert(initialized == false);
	// Window class initialization
	WNDCLASSA wndClass = {};
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = WinProc;
	wndClass.hInstance = GetModuleHandle(nullptr);
	wndClass.hIcon = NULL;
	wndClass.hCursor = NULL;
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = wndClassName;
	// and register
	RegisterClassA(&wndClass);
	initialized = true;
}

int Window::Run() noexcept
{
	MSG message = {};
	// Loop will run even when there are no messages
	while (PeekMessageA(&message, NULL, 0u, 0u, PM_REMOVE) > 0)
	{
		if (message.message == WM_QUIT)
		{
			return 0;
		}
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return 1;
}

Window::Window(const char* title, unsigned int width, unsigned int height)
{
	// Check if the window is not initialized
	assert(initialized == true && "Window is not initialized");
	// Create the window handle
	handleWindow = CreateWindowA(wndClassName, title, WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, width, height, NULL, NULL, GetModuleHandle(nullptr), NULL);
	// Show the window
	ShowWindow(handleWindow, SW_SHOW);
}

HWND Window::GetWindowHandle() noexcept
{
	return handleWindow;
}

LRESULT __stdcall Window::WinProc(HWND handleWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(handleWindow);
		PostQuitMessage(0);
		break;
	default:
		// return the default window proc for any other messages
		return DefWindowProc(handleWindow, message, wParam, lParam);
	}
	return 0;
}
