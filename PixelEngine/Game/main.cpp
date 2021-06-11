#include "..\Engine\Engine.h"
#include <exception>

int __stdcall wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR pCmdLine, _In_ int)
{
	Engine::Init();
	try
	{
		Engine engine("Hello", 800, 600);
		Engine engine2("Hello2", 800, 600);
		int x1 = 200;
		int y1 = 200;
		int x2 = 200;
		int y2 = 200;
		while (engine.Run() > 0 && engine2.Run() > 0)
		{
			engine.BeginFrame();
			engine2.BeginFrame();
			// Engine 1
			if (engine.KeyIsPressed('W')) { y1--; }
			if (engine.KeyIsPressed('A')) { x1--; }
			if (engine.KeyIsPressed('S')) { y1++; }
			if (engine.KeyIsPressed('D')) { x1++; }

			for (int inx = x1; inx < x1 + 300; inx++)
			{
				for (int iny = y1; iny < y1 + 300; iny++)
				{
					engine.SetPixel(inx, iny, 255, 255, 255);
				}
			}
			// Engine 2
			if (engine2.KeyIsPressed('W')) { y2--; }
			if (engine2.KeyIsPressed('A')) { x2--; }
			if (engine2.KeyIsPressed('S')) { y2++; }
			if (engine2.KeyIsPressed('D')) { x2++; }

			for (int inx = x2; inx < x2 + 300; inx++)
			{
				for (int iny = y2; iny < y2 + 300; iny++)
				{
					engine2.SetPixel(inx, iny, 255, 255, 255);
				}
			}
			engine2.EndFrame();
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