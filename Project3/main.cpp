#include "Engine.h"
#include <exception>

int __stdcall wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR pCmdLine, _In_ int)
{
	Engine::Init();
	try
	{
		Engine engine("Hello", 800, 600);
		while (engine.Run() > 0)
		{
			engine.BeginFrame(1.f, .5f, 1.f);
			engine.EndFrame();
		}
	}
	catch (const std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Exception caught:", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{

	}
	return 0;
}