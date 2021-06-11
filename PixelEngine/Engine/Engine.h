#pragma once
#include "Window.h"
#include <d3d11.h>
#include <wrl.h>

class Engine
{
public:
	struct Color
	{
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		unsigned char a = 0;
	};
public:
	// Initialize the engine only once
	static void Init() noexcept;
	// Constructor
	Engine(const char* title, unsigned short width, unsigned short height);
	// Run bascially just calls window.Run();
	int Run() noexcept;
	// Clears the back buffer
	void BeginFrame();
	// Set a pixel color in system memory
	void SetPixel(int x, int y, Color c) noexcept;
	void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) noexcept;
	// Get width / height
	int GetWidth() const noexcept;
	int GetHeight() const noexcept;
	// Present the frame
	void EndFrame();
public:
	bool KeyIsPressed(unsigned char keycode) const noexcept;
public:
	const unsigned short width;
	const unsigned short height;
private:
	Color* systemBuffer = nullptr;
	Window window;
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backbufferview;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backbuffertexture;
	D3D11_MAPPED_SUBRESOURCE mappedSBR;
};