#ifndef MESH_HPP
#define MESH_HPP
#include "Mesh.h"

template<typename TIndex>
void Mesh::SetIndices(TIndex* indices, UINT count, DXGI_FORMAT indexFormat = DXGI_FORMAT_R32_UINT)
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
void Mesh::SetVertices(TVertex* vertices, UINT count)
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
#endif // MESH_HPP
