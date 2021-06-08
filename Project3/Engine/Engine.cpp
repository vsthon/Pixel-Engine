#include "Engine.h"
#include <d3d11.h>
#include <exception>
#include <stdexcept>
#include <string>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#define THROW_IF_FAILED(hresult, message) if(FAILED(hresult)) throw std::logic_error(message)

namespace WRL = Microsoft::WRL;

void Engine::Init() noexcept
{
	Window::Initialize();
}

Engine::Engine(const char* title, unsigned short width, unsigned short height)
	:
	width(width),
	height(height),
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
	THROW_IF_FAILED(swapchain->GetBuffer(0u, __uuidof(ID3D11Resource), &backbuffer), "Error on creating backbuffer!");
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
	struct Vertex
	{
		float x;
		float y;
	};
	Vertex vertexBuffer[] =
	{
		{-1.f, 1.f},
		{1.f, 1.f},
		{-1.f, -1.f},
		{1.f, -1.f}
	};
	D3D11_BUFFER_DESC vBufDesc = {};
	vBufDesc.ByteWidth = sizeof(vertexBuffer);
	vBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufDesc.CPUAccessFlags = 0u;
	vBufDesc.MiscFlags = 0u;
	vBufDesc.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA vBufData = {};
	vBufData.pSysMem = vertexBuffer;
	WRL::ComPtr<ID3D11Buffer> vBuf;
	device->CreateBuffer(&vBufDesc, &vBufData, &vBuf);
	UINT stride = sizeof(Vertex);
	UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vBuf.GetAddressOf(), &stride, &offset);


	unsigned short indexBuffer[] =
	{
		0,1,2,
		1,3,2
	};

	D3D11_BUFFER_DESC iBufDesc = {};
	iBufDesc.ByteWidth = sizeof(vertexBuffer);
	iBufDesc.Usage = D3D11_USAGE_DEFAULT;
	iBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iBufDesc.CPUAccessFlags = 0u;
	iBufDesc.MiscFlags = 0u;
	iBufDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA iBufData = {};
	iBufData.pSysMem = indexBuffer;
	WRL::ComPtr<ID3D11Buffer> iBuf;
	device->CreateBuffer(&iBufDesc, &iBufData, &iBuf);
	context->IASetIndexBuffer(iBuf.Get(), DXGI_FORMAT_R16_UINT, 0u);

	WRL::ComPtr<ID3DBlob> blob;
	WRL::ComPtr<ID3D11PixelShader> pixelshader;
	D3DReadFileToBlob(L"PixelShader.cso", &blob);
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelshader);
	context->PSSetShader(pixelshader.Get(), nullptr, 0u);

	WRL::ComPtr<ID3D11VertexShader> vertexshader;
	D3DReadFileToBlob(L"VertexShader.cso", &blob);
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexshader);
	context->VSSetShader(vertexshader.Get(), nullptr, 0u);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_INPUT_ELEMENT_DESC inputdesc = {};
	inputdesc.SemanticName = "Position";
	inputdesc.SemanticIndex = 0u;
	inputdesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	inputdesc.InputSlot = 0u;
	inputdesc.AlignedByteOffset = 0u;
	inputdesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputdesc.InstanceDataStepRate = 0u;
	WRL::ComPtr<ID3D11InputLayout> inputlayout;
	device->CreateInputLayout(&inputdesc, 1u, blob->GetBufferPointer(), blob->GetBufferSize(), &inputlayout);
	context->IASetInputLayout(inputlayout.Get());

	context->OMSetRenderTargets(1u, backbufferview.GetAddressOf(), nullptr);
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 0.f;

	context->RSSetViewports(1u, &viewport);

	context->DrawIndexed(6u, 0u, 0u);
	THROW_IF_FAILED(swapchain->Present(1u, 0u), "Error on presenting!");
}