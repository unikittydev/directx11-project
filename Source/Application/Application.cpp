#include "Application.h"
#include "Graphics/TextureImporter.h"

Application* Application::s_instance = nullptr;

Application::Application()
{
	s_instance = this;
	TextureImporter::Init();
}

Application::~Application()
{
	device.Release();
}

DeviceWrapper& Application::GetDevice()
{
	return s_instance->device;
}

ID3D11Device* Application::GetDevicePtr()
{
	return GetDevice().GetDevice();
}

ID3D11DeviceContext* Application::GetDeviceContext()
{
	return GetDevice().GetContext();
}

InputDevice& Application::GetInput()
{
	return s_instance->input;
}

WindowWrapper& Application::GetWindow()
{
	return s_instance->window;
}

SwapchainWrapper& Application::GetSwapchain()
{
	return s_instance->swapChain;
}

IDXGISwapChain1* Application::GetSwapchainPtr()
{
	return GetSwapchain().GetSwapChain();
}

UINT Application::WindowWidth()
{
	return GetWindow().Width();
}

UINT Application::WindowHeight()
{
	return GetWindow().Height();
}