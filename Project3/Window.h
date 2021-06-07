#pragma once
#include <Windows.h>

class Window
{
public:
	// Initialize will NOT create a window.
	static void Initialize() noexcept;
	// Constructor
	Window(const char* title, unsigned int width, unsigned int height);
	// Main loop
	static int Run() noexcept;
private:
	// Window procedure;
	static LRESULT __stdcall WinProc(HWND handleWindow, UINT message, WPARAM wParam, LPARAM lParam);
	HWND handleWindow;
	static constexpr const char* wndClassName = "Engine";
	static bool initialized;
};