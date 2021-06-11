#pragma once
#define NOMINMAX
#include <Windows.h>
#include <bitset>

class Window
{
private:
	class Keyboard
	{
		friend Window;
	public:
		bool KeyIsPressed(unsigned char keycode) const noexcept;
	private:
		void OnKeyPressed(unsigned char keycode) noexcept;
		void OnKeyReleased(unsigned char keycode) noexcept;
	private:
		std::bitset<256> keystates;
	};
	class Mouse
	{
		friend Window;
	public:
		int GetX() const noexcept;
		int GetY() const noexcept;
	private:
		void OnMouseMove(int inx, int iny);
	private:
		int x = 0;
		int y = 0;
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
public:
	Keyboard keyboard;
	Mouse mouse;
private:
	// Window procedure;
	static LRESULT __stdcall WinProc(HWND handleWindow, UINT message, WPARAM wParam, LPARAM lParam);
	HWND handleWindow;
	static constexpr const char* wndClassName = "Engine";
	static bool initialized;
};