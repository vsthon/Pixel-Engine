#include "Engine.h"
#include <d3d11.h>
#include <exception>
#include <stdexcept>
#include <assert.h>
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
	systemBuffer(new Color[width * height]),
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
	UINT createdeviceandswapflags = 0u;
#ifndef NDEBUG
	createdeviceandswapflags = D3D11_CREATE_DEVICE_DEBUG;
#endif
	THROW_IF_FAILED(D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		createdeviceandswapflags, NULL, 0u, D3D11_SDK_VERSION,
		&swapChainDesc, &swapchain, &device, NULL, &context), "Error on creating deviceandswapchain!");
	// create rtv to backbuffer
	WRL::ComPtr<ID3D11Resource> backbuffer;
	THROW_IF_FAILED(swapchain->GetBuffer(0u, __uuidof(ID3D11Resource), &backbuffer), "Error on creating backbuffer!");
	THROW_IF_FAILED(device->CreateRenderTargetView(backbuffer.Get(), nullptr, &backbufferview), "Error on creating RTV for backbuffer!");

	

	// Pipeline initialization
	// The Vertex type
	struct Vertex
	{
		struct
		{
			float x;
			float y;
		} pos;
		struct
		{
			float u;
			float v;
		} uv;
	};
	// Vertex buffer
	Vertex vertexBuffer[] =
	{
		    // pos          // uv
		{ {-1.f, 1.f},    {0.f, 0.f} },
		{ {1.f, 1.f},     {1.f, 0.f} },
		{ {-1.f, -1.f},   {0.f, 1.f} },
		{ {1.f, -1.f },   { 1.f, 1.f } }
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
	THROW_IF_FAILED(device->CreateBuffer(&vBufDesc, &vBufData, &vBuf), "Error on creating vertex buffer!");
	UINT stride = sizeof(Vertex);
	UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vBuf.GetAddressOf(), &stride, &offset);

	// Index Buffer
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
	THROW_IF_FAILED(device->CreateBuffer(&iBufDesc, &iBufData, &iBuf), "Error on creating index buffer!");
	context->IASetIndexBuffer(iBuf.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// Shaders
	WRL::ComPtr<ID3DBlob> blob;
	WRL::ComPtr<ID3D11PixelShader> pixelshader;
	THROW_IF_FAILED(D3DReadFileToBlob(L"PixelShader.cso", &blob), "Error on compiling pixel shader! Did you mess up the file name?");
	THROW_IF_FAILED(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelshader), "Error on creating pixel shader!");
	context->PSSetShader(pixelshader.Get(), nullptr, 0u);

	WRL::ComPtr<ID3D11VertexShader> vertexshader;
	THROW_IF_FAILED(D3DReadFileToBlob(L"VertexShader.cso", &blob), "Error on compiling vertex shader! Did you mess up the file name?");
	THROW_IF_FAILED(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexshader), "Error on creating vertex shader!");
	context->VSSetShader(vertexshader.Get(), nullptr, 0u);

	// Topology
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Input desc
	const D3D11_INPUT_ELEMENT_DESC inputdesc[] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TextureCoord", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
	};
	WRL::ComPtr<ID3D11InputLayout> inputlayout;
	THROW_IF_FAILED(device->CreateInputLayout(inputdesc, 2u, blob->GetBufferPointer(), blob->GetBufferSize(), &inputlayout), "Error on creating input layout!");
	context->IASetInputLayout(inputlayout.Get());

	// viewport
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 0.f;

	context->RSSetViewports(1u, &viewport);

	D3D11_TEXTURE2D_DESC texturedesc = {};
	texturedesc.Width = width;
	texturedesc.Height = height;
	texturedesc.MipLevels = 1u;
	texturedesc.ArraySize = 1u;
	texturedesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texturedesc.SampleDesc.Count = 1u;
	texturedesc.SampleDesc.Quality = 0u;
	texturedesc.Usage = D3D11_USAGE_DYNAMIC;
	texturedesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texturedesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texturedesc.MiscFlags = 0u;
	D3D11_SUBRESOURCE_DATA texturedata = {};
	texturedata.pSysMem = systemBuffer;
	texturedata.SysMemPitch = sizeof(Color) * width;
	THROW_IF_FAILED(device->CreateTexture2D(&texturedesc, &texturedata, &backbuffertexture), "Error on creating systembuffer texture!");

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderviewdesc = {};
	shaderviewdesc.Format = texturedesc.Format;
	shaderviewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderviewdesc.Texture2D.MipLevels = 1u;
	WRL::ComPtr<ID3D11ShaderResourceView> texture;
	THROW_IF_FAILED(device->CreateShaderResourceView(backbuffertexture.Get(), &shaderviewdesc, &texture), "Error on creating view to the systembuffer texture!");
	context->PSSetShaderResources(0u, 1u, texture.GetAddressOf());

	D3D11_SAMPLER_DESC samplerdesc = {};
	samplerdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	WRL::ComPtr<ID3D11SamplerState> sampler;
	THROW_IF_FAILED(device->CreateSamplerState(&samplerdesc, &sampler), "Error on creating samplerstate!");
	context->PSSetSamplers(0u, 1u, sampler.GetAddressOf());
}

int Engine::Run() noexcept
{
	return window.Run();
}

void Engine::BeginFrame()
{
	for (size_t x = 0u; x < width; x++)
	{
		for (size_t y = 0u; y < height; y++)
		{
			systemBuffer[y * width + x] = { 0u, 0u, 0u, 0u };
		}
	}
}

void Engine::SetPixel(int x, int y, Color c) noexcept
{
	assert(x >= 0 && x < width && y >= 0 && y < height && "X or Y is less than zero or greater than Screen Width/Height!");
	systemBuffer[y * width + x] = c;
}

void Engine::SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) noexcept
{
	SetPixel(x, y, { r, g, b });
}

int Engine::GetWidth() const noexcept
{
	return width;
}

int Engine::GetHeight() const noexcept
{
	return height;
}

void Engine::EndFrame()
{
	// Copied from Chili's framework www.planetchili.net
	THROW_IF_FAILED(context->Map(backbuffertexture.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSBR), "Error on mapping SBR!");
	Color* pDst = reinterpret_cast<Color*>(mappedSBR.pData);
	const size_t dstPitch = mappedSBR.RowPitch / sizeof(Color);
	const size_t srcPitch = width;
	const size_t rowBytes = srcPitch * sizeof(Color);
	for (size_t y = 0u; y < height; y++)
	{
		memcpy(&pDst[y * dstPitch], &systemBuffer[y * srcPitch], rowBytes);
	}
	context->Unmap(backbuffertexture.Get(), 0u);
	context->OMSetRenderTargets(1u, backbufferview.GetAddressOf(), nullptr);
	context->DrawIndexed(6u, 0u, 0u);
	THROW_IF_FAILED(swapchain->Present(1u, 0u), "Error on presenting!");
}

bool Engine::KeyIsPressed(unsigned char keycode) const noexcept
{
	return window.keyboard.KeyIsPressed(keycode);
}
