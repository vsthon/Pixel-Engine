#include "Engine.h"

int __stdcall wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, LPWSTR pCmdLine, int)
{
	Engine::Init();
	Engine engine("Hello", 800, 600);

	while (engine.Run() > 0)
	{
		engine.ClearScreen(1.f, .5f, 1.f);
		engine.EndFrame();
	}
	return 0;
}