#pragma once

#include <d3d11.h>
#include <dxgi.h>

#include "WindowWrapper.h"
#include "DeviceWrapper.h"

class SwapchainWrapper
{
private:
	IDXGISwapChain1* swapChain = nullptr;
	ID3D11RenderTargetView* rtv = nullptr;

	void CreateRenderTargetView(DeviceWrapper& device)
	{
		ID3D11Texture2D* tex;

		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tex);
		device.GetDevice()->CreateRenderTargetView(tex, NULL, &rtv);
		tex->Release();
	}

	void ReleaseRenderTargetView(DeviceWrapper& device)
	{
		if (rtv)
		{
			device.GetContext()->OMSetRenderTargets(0, 0, 0);
			rtv->Release();
			rtv = nullptr;
		}
	}

public:
	SwapchainWrapper(WindowWrapper& window, DeviceWrapper& device)
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
		res = device.GetDevice()->QueryInterface(__uuidof(IDXGIDevice2), (void**)&dxgiDevice);

		IDXGIAdapter* dxgiAdapter;
		res = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

		IDXGIFactory2* dxgiFactory;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory);

		dxgiFactory->CreateSwapChainForHwnd(device.GetDevice(), window.hWnd, &swapDesc, nullptr, nullptr, &swapChain);
	
		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();

		CreateRenderTargetView(device);
	}

	void Resize(DeviceWrapper& device, UINT width, UINT height)
	{
		ReleaseRenderTargetView(device);
		swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		CreateRenderTargetView(device);
	}

	IDXGISwapChain1* GetSwapChain()
	{
		return swapChain;
	}

	ID3D11RenderTargetView*& GetRenderTargetView()
	{
		return rtv;
	}
};