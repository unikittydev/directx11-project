#pragma once

#include <d3d11.h>

#include "../Application/Application.h"

template<typename TVertex>
void CreateVertexBuffer(UINT count, TVertex* vertices, ID3D11Buffer** buffer)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = sizeof(TVertex) * count;

	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = vertices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	Application::GetDevicePtr()->CreateBuffer(&bufferDesc, &bufferData, buffer);
}

template<typename TBuffer>
void CreateDynamicBuffer(UINT count, TBuffer* data, ID3D11Buffer** buffer)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = sizeof(TBuffer) * count;

	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = data;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	Application::GetDevicePtr()->CreateBuffer(&bufferDesc, &bufferData, buffer);
}

void CreateIndexBuffer(UINT count, UINT* indices, ID3D11Buffer** buffer);