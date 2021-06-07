#include "Engine.h"

int __stdcall wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR pCmdLine, _In_ int)
{
	Engine::Init();
	Engine engine("Hello", 800, 600);

	while (engine.Run() > 0)
	{
		engine.BeginFrame(1.f, .5f, 1.f);
		engine.EndFrame();
	}
	return 0;
}