#pragma once
#include <Windows.h>

class Window
{
public:
	// Initialize will NOT create a window.
	static void Initialize() noexcept;
	// Main loop
	static int Run() noexcept;
	// Constructor
	Window(const char* title, unsigned short width, unsigned short height);
	// Get window handle;
	HWND GetWindowHandle() noexcept;
private:
	// Window procedure;
	static LRESULT __stdcall WinProc(HWND handleWindow, UINT message, WPARAM wParam, LPARAM lParam);
	HWND handleWindow;
	static constexpr const char* wndClassName = "Engine";
	static bool initialized;
};