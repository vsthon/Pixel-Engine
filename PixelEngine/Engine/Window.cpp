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
	wndClass.hInstance = NULL;
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

Window::Window(const char* title, unsigned short width, unsigned short height)
{
	// Check if the window is not initialized
	assert(initialized == true && "Window is not initialized");
	// Create the window handle
	RECT windowrect = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&windowrect, WS_OVERLAPPEDWINDOW, FALSE);
	handleWindow = CreateWindowExA(0u, wndClassName, title, WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, windowrect.right - windowrect.left, windowrect.bottom - windowrect.top, NULL, NULL, GetModuleHandle(nullptr), this);
	// Show the window
	ShowWindow(handleWindow, SW_SHOW);
}

HWND Window::GetWindowHandle() noexcept
{
	return handleWindow;
}

LRESULT __stdcall Window::WinProc(HWND handleWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCCREATE)
	{
		SetWindowLongPtr(handleWindow, GWLP_USERDATA, (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
	}
	Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(handleWindow, GWLP_USERDATA));
	switch (message)
	{
	case WM_KEYDOWN:
		window->keyboard.OnKeyPressed((unsigned char)wParam);
		break;
	case WM_KEYUP:
		window->keyboard.OnKeyReleased((unsigned char)wParam);
		break;
	case WM_MOUSEMOVE:
		POINTS coordinates = MAKEPOINTS(lParam);
		window->mouse.OnMouseMove(coordinates.x, coordinates.y);
		break;
	case WM_LBUTTONDOWN:	
		// By default, windows will not generate WM_MOUSEMOVE if the cursor goes outside the window which is not what we want. SetCapture will keep generating WM_MOUSEMOVE
		// even if outside the client region as long as you hold one mouse button down.
		SetCapture(handleWindow);
		break;
	case WM_RBUTTONDOWN:
		SetCapture(handleWindow);
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		break;
	case WM_RBUTTONUP:
		ReleaseCapture();
		break;
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

bool Window::Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return keystates[keycode];
}

void Window::Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	keystates[keycode] = true;
}

void Window::Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	keystates[keycode] = false;
}

int Window::Mouse::GetX() const noexcept
{
	return x;
}

int Window::Mouse::GetY() const noexcept
{
	return y;
}

void Window::Mouse::OnMouseMove(int inx, int iny)
{
	x = inx;
	y = iny;
}
