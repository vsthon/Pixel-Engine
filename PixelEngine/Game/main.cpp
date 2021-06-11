#include "..\Engine\Engine.h"
#include <exception>

int __stdcall wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR pCmdLine, _In_ int)
{
	Engine::Init();
	try
	{
		Engine engine("Hello", 800, 600);
		int x = 200;
		int y = 200;
		while (engine.Run() > 0)
		{
			engine.BeginFrame();
			if (engine.KeyIsPressed('W')) { y--; }
			if (engine.KeyIsPressed('A')) { x--; }
			if (engine.KeyIsPressed('S')) { y++; }
			if (engine.KeyIsPressed('D')) { x++; }

			for (int inx = x; inx < x + 300; inx++)
			{
				for (int iny = y; iny < y + 300; iny++)
				{
					engine.SetPixel(inx, iny, 255, 255, 255);
				}
			}
			engine.EndFrame();
		}
	}
	catch (const std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Exception caught:", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(NULL, "Unknown exception", "Exception caught:", MB_OK | MB_ICONEXCLAMATION);
	}
	return 0;
}