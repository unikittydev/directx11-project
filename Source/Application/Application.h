#pragma once

#include "Input/InputDevice.h"
#include "DeviceWrapper.h"
#include "WindowWrapper.h"
#include "SwapchainWrapper.h"

class Application
{
private:
	DeviceWrapper device{};
	InputDevice input{ window.hWnd };
	WindowWrapper window{};
	SwapchainWrapper swapChain{ window, device };

	static Application* s_instance;

public:
	Application()
	{
		s_instance = this;
	}

	~Application()
	{
		device.Release();
	}

	static DeviceWrapper& GetDevice();

	static ID3D11Device* GetDevicePtr();

	static ID3D11DeviceContext* GetDeviceContext();

	static InputDevice& GetInput();

	static WindowWrapper& GetWindow();

	static SwapchainWrapper& GetSwapchain();

	static IDXGISwapChain1* GetSwapchainPtr();

	static UINT WindowWidth();

	static UINT WindowHeight();
};
