#pragma once
#include "Window.h"
#include <d3d11.h>
#include <wrl.h>

class Engine
{
public:
	static void Init() noexcept;
	Engine(const char* title, int width, int height);
	int Run() noexcept;
	void BeginFrame();
	void ClearScreen(float r, float g, float b);
	void EndFrame();
private:
	Window window;
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backbufferview;
};