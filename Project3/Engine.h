#pragma once
#include "Window.h"
#include <d3d11.h>
#include <wrl.h>

class Engine
{
public:
	// Initialize the engine only once
	static void Init() noexcept;
	// Constructor
	Engine(const char* title, int width, int height);
	// Run bascially just calls window.Run();
	int Run() noexcept;
	// Clears the back buffer
	void BeginFrame(float r, float g, float b);
	void EndFrame();
private:
	Window window;
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backbufferview;
};