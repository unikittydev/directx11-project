#pragma once

#include <d3d11.h>

class DeviceWrapper
{
private:
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;

public:
	DeviceWrapper()
	{
		UINT creationFlags = 0;
#if _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

		HRESULT res = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, featureLevels, 2, D3D11_SDK_VERSION, &device, &featureLevel, &context);

		if (res != S_OK)
			throw;
	}

	void Release()
	{
		device->Release();
		context->Release();
	}

	ID3D11Device* GetDevice()
	{
		return device;
	}

	ID3D11DeviceContext* GetContext()
	{
		return context;
	}
};