#pragma once

#include <d3d11.h>

#include "Application/Application.h"

template<typename TBuffer>
class ConstantBuffer
{
private:
	ID3D11Buffer* buffer;

public:
	ConstantBuffer(TBuffer& data, int count = 1)
	{
		auto* device = Application::GetDevicePtr();
		
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		bufferDesc.ByteWidth = sizeof(TBuffer) * count;

		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = &data;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		device->CreateBuffer(&bufferDesc, &bufferData, &buffer);
	}

	void Release()
	{
		buffer->Release();
	}

	void Bind(ID3D11DeviceContext* ctx, UINT slot)
	{
		ctx->VSSetConstantBuffers(slot, 1, &buffer);
	}

	TBuffer* Map(ID3D11DeviceContext* ctx)
	{
		D3D11_MAPPED_SUBRESOURCE res = {};
		ctx->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
		return reinterpret_cast<TBuffer*>(res.pData);
	}

	void Unmap(ID3D11DeviceContext* ctx)
	{
		ctx->Unmap(buffer, 0);
	}
};
