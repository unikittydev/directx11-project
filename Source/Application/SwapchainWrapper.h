#pragma once

#include "Core/Types.h"
#include <d3d11.h>

class DeviceWrapper;
class WindowWrapper;

class SwapchainWrapper
{
private:
	IDXGISwapChain1* swapChain = nullptr;
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = nullptr;

	void CreateSwapchainAndRTV(const WindowWrapper& window, DeviceWrapper& device);
	
	void CreateDepthStencilView(const WindowWrapper& window, DeviceWrapper& device);
	
	void CreateRenderTargetView(DeviceWrapper& device);

	void ReleaseRenderTargetView(DeviceWrapper& device);

public:
	SwapchainWrapper(const WindowWrapper& window, DeviceWrapper& device);

	void Resize(DeviceWrapper& device, uint width, uint height);

	IDXGISwapChain1* GetSwapChain() const;
	
	ID3D11RenderTargetView*& GetRenderTargetView();

	ID3D11DepthStencilView* GetDepthStencilView() const;
};
