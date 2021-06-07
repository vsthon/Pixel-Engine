#include "Window.h"

int __stdcall wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, LPWSTR pCmdLine, int)
{
	Window::Initialize();

	Window win("Hello world", 800, 600);
	while (win.Run() > 0);
	return 0;
}