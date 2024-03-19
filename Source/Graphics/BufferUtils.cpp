#include "BufferUtils.h"

void CreateIndexBuffer(UINT count, UINT* indices, ID3D11Buffer** buffer)
{
	// Create buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = sizeof(UINT) * count;

	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = indices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	Application::GetDevicePtr()->CreateBuffer(&bufferDesc, &bufferData, buffer);
}