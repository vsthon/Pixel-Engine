#include "Engine.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace WRL = Microsoft::WRL;

void Engine::Init() noexcept
{
	Window::Initialize();
}

Engine::Engine(const char* title, int width, int height)
	:
	window(title, width, height)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = 0u;
	swapChainDesc.BufferDesc.Height = 0u;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1u;
	swapChainDesc.SampleDesc.Quality = 0u;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2u;
	swapChainDesc.OutputWindow = window.GetWindowHandle();
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.Flags = 0u;

	D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,D3D11_CREATE_DEVICE_DEBUG,NULL,0u,D3D11_SDK_VERSION,&swapChainDesc,&swapchain,&device,NULL,&context);

	WRL::ComPtr<ID3D11Resource> backbuffer;
	swapchain->GetBuffer(0u, __uuidof(ID3D11Resource), &backbuffer);
	device->CreateRenderTargetView(backbuffer.Get(), nullptr, &backbufferview);
}

int Engine::Run() noexcept
{
	return window.Run();
}

void Engine::BeginFrame()
{}

void Engine::ClearScreen(float r, float g, float b)
{
	float color[] = { r, g, b, 1.f };
	context->ClearRenderTargetView(backbufferview.Get(), color);
}

void Engine::EndFrame()
{
	swapchain->Present(1u, 0u);
}