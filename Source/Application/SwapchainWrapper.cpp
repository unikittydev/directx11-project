#pragma once

#include "SwapchainWrapper.h"

#include <dxgi.h>

#include "WindowWrapper.h"
#include "DeviceWrapper.h"
#include "Application/Application.h"

void SwapchainWrapper::CreateSwapchainAndRTV(const WindowWrapper& window, DeviceWrapper& device)
{
	DXGI_SWAP_CHAIN_DESC1 swapDesc{};
	swapDesc.Width = window.Width();
	swapDesc.Height = window.Height();
	swapDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapDesc.SampleDesc = { 1, 0 };
	swapDesc.BufferCount = 2;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.Scaling = DXGI_SCALING_STRETCH;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	HRESULT res;

	IDXGIDevice2* dxgiDevice;
	res = device.GetDevice()->QueryInterface(__uuidof(IDXGIDevice2), reinterpret_cast<void**>(&dxgiDevice));

	IDXGIAdapter* dxgiAdapter;
	res = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));

	IDXGIFactory2* dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory));

	dxgiFactory->CreateSwapChainForHwnd(device.GetDevice(), window.hWnd, &swapDesc, nullptr, nullptr, &swapChain);

	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();

	CreateRenderTargetView(device);
}

void SwapchainWrapper::CreateDepthStencilView(const WindowWrapper& window, DeviceWrapper& device)
{
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.Width = window.Width();
	depthTextureDesc.Height = window.Height();
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.MiscFlags = 0;

	ID3D11Texture2D *pDepthStencilTexture;
	
	HRESULT result = device.GetDevice()->CreateTexture2D(&depthTextureDesc, nullptr, &pDepthStencilTexture);

	if (result != S_OK)
		std::cerr << "Failed to create depth texture!" << std::endl;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = depthTextureDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	result = device.GetDevice()->CreateDepthStencilView(pDepthStencilTexture, &dsvDesc, &dsv);
	pDepthStencilTexture->Release();

	if (result != S_OK)
		std::cerr << "Failed to create depth stencil view!" << std::endl;
}

void SwapchainWrapper::CreateRenderTargetView(DeviceWrapper& device)
{
	ID3D11Texture2D* tex;

	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&tex));
	device.GetDevice()->CreateRenderTargetView(tex, nullptr, &rtv);
	tex->Release();
}

void SwapchainWrapper::ReleaseRenderTargetView(DeviceWrapper& device)
{
	if (rtv)
	{
		device.GetContext()->OMSetRenderTargets(0, nullptr, nullptr);
		rtv->Release();
		rtv = nullptr;
	}
}

SwapchainWrapper::SwapchainWrapper(const WindowWrapper& window, DeviceWrapper& device)
{
	CreateSwapchainAndRTV(window, device);
	CreateDepthStencilView(window, device);
}

void SwapchainWrapper::Resize(DeviceWrapper& device, UINT width, UINT height)
{
	ReleaseRenderTargetView(device);
	swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	CreateRenderTargetView(device);
}

IDXGISwapChain1* SwapchainWrapper::GetSwapChain() const
{
	return swapChain;
}

ID3D11RenderTargetView*& SwapchainWrapper::GetRenderTargetView()
{
	return rtv;
}

ID3D11DepthStencilView* SwapchainWrapper::GetDepthStencilView() const
{
	return dsv;
}