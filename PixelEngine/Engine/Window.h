#pragma once
#include <Windows.h>
#include <bitset>

class Window
{
private:
	friend class Engine;
private:
	class Keyboard
	{
	private:
		friend Window;
	public:
		bool KeyIsPressed(unsigned char keycode) const noexcept;
	private:
		void SetKeyIsPressed(unsigned char keycode, const bool setstate) noexcept;
	private:
		std::bitset<256> keystates;
	};
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
	Keyboard keyboard;
private:
	// Window procedure;
	static LRESULT __stdcall WinProc(HWND handleWindow, UINT message, WPARAM wParam, LPARAM lParam);
	HWND handleWindow;
	static constexpr const char* wndClassName = "Engine";
	static bool initialized;
};