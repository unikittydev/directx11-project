#pragma once

#include <d3d11.h>

#include "Core/Types.h"

#include "Shader.h"
#include "ConstantBuffer.h"

class Mesh
{
private:
	struct WorldData
	{
		matrix _WorldViewProj;
	} worldData;

	struct MeshData
	{
		float4 color;
	} meshData;

	Shader* shader;

	ID3D11Buffer* vertexBuffer;
	UINT vertexSize;
	UINT vertexCount;

	ID3D11Buffer* indexBuffer;
	DXGI_FORMAT indexFormat;
	UINT indexCount;

	ConstantBuffer<WorldData> worldDataBuffer;
	ConstantBuffer<MeshData> meshDataBuffer;

public:
	Mesh();

	void Release();

	template<typename TIndex>
	void SetIndices(TIndex* indices, UINT count, DXGI_FORMAT indexFormat = DXGI_FORMAT_R32_UINT)
	{
		auto* device = Application::GetDevicePtr();
		
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		bufferDesc.ByteWidth = sizeof(TIndex) * count;

		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = indices;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		if (indexBuffer != nullptr)
			indexBuffer->Release();

		device->CreateBuffer(&bufferDesc, &bufferData, &indexBuffer);

		this->indexFormat = indexFormat;
		this->indexCount = count;
	}

	template<typename TVertex>
	void SetVertices(TVertex* vertices, UINT count)
	{
		auto* device = Application::GetDevicePtr();
		
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

		if (vertexBuffer != nullptr)
			vertexBuffer->Release();

		device->CreateBuffer(&bufferDesc, &bufferData, &vertexBuffer);

		this->vertexSize = sizeof(TVertex);
		this->vertexCount = count;
	}

	void SetShader(Shader* shader);
	
	void SetColor(float4 color);

	void Draw(matrix vp, matrix localToWorld);

	UINT GetVertexCount() const;
	UINT GetIndexCount() const;
};
