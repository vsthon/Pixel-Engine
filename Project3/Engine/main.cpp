#include "Engine.h"
#include <exception>

int __stdcall wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR pCmdLine, _In_ int)
{
	Engine::Init();
	try
	{
		Engine engine("Hello", 800, 600);
		Engine engine2("Hello", 800, 600);
		while (engine.Run() > 0 && engine2.Run() > 0)
		{
			engine.BeginFrame();
			engine2.BeginFrame();
			for (int i = 200; i < 300; i++)
			{
				for(int j = 200; j < 300; j++)
				{
					engine.SetPixel(i, j, 255, 255, 255);
				}
			}

			for (int i = 100; i < 300; i++)
			{
				for (int j = 100; j < 300; j++)
				{
					engine2.SetPixel(i, j, 255, 255, 255);
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