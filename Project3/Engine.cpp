#include "Engine.h"
#include <d3d11.h>
#include <exception>
#include <stdexcept>
#include <string>
#pragma comment(lib, "d3d11.lib")
#define THROW_IF_FAILED(hresult, message) if(FAILED(hresult)) throw std::logic_error(message)

namespace WRL = Microsoft::WRL;

void Engine::Init() noexcept
{
	Window::Initialize();
}

Engine::Engine(const char* title, int width, int height)
	:
	window(title, width, height)
{
	// Swap chain desc
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
	// create device, swapchain, and context
	THROW_IF_FAILED(D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		D3D11_CREATE_DEVICE_DEBUG, NULL, 0u, D3D11_SDK_VERSION,
		&swapChainDesc, &swapchain, &device, NULL, &context), "Error on creating deviceandswapchain!");
	// create rtv to backbuffer
	WRL::ComPtr<ID3D11Resource> backbuffer;
	THROW_IF_FAILED(swapchain->GetBuffer(6u, __uuidof(ID3D11Resource), &backbuffer), "Error on creating backbuffer!");
	THROW_IF_FAILED(device->CreateRenderTargetView(backbuffer.Get(), nullptr, &backbufferview), "Error on creating RTV for backbuffer!");
}

int Engine::Run() noexcept
{
	return window.Run();
}

void Engine::BeginFrame(float r, float g, float b)
{
	float color[] = { r, g, b, 1.f };
	context->ClearRenderTargetView(backbufferview.Get(), color);
}

void Engine::EndFrame()
{
	THROW_IF_FAILED(swapchain->Present(1u, 0u), "Error on presenting!");
}