#include "Mesh.h"

#include "Shaders.h"
#include "Application/Application.h"

Mesh::Mesh() : worldDataBuffer(worldData, 1), meshDataBuffer(meshData, 1)
{
	shader = Shaders::Get(L"./Shaders/Default.hlsl", Position | VertexColor | UV0);
}

void Mesh::Draw(matrix vp, matrix localToWorld)
{
	// TODO: DON'T CHANGE RAST STATE IN EVERY DRAW CALL
	shader->PrepareDraw();

	// Setup const buffers
	worldData._WorldViewProj = (localToWorld * vp).Transpose();

	auto* ctx = Application::GetDeviceContext();

	auto* pWorldData = worldDataBuffer.Map(ctx);
	pWorldData[0] = worldData;
	worldDataBuffer.Unmap(ctx);

	auto* pMeshData = meshDataBuffer.Map(ctx);
	pMeshData[0] = meshData;
	meshDataBuffer.Unmap(ctx);

	worldDataBuffer.Bind(ctx, 0);
	meshDataBuffer.Bind(ctx, 1);

	// Setup vertex and index buffers
	const UINT strides[] = { vertexSize };
	const UINT offsets[] = { 0 };
	ctx->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
	ctx->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	shader->SetActive();

	ctx->DrawIndexed(indexCount, 0, 0);
}

void Mesh::SetShader(Shader* shader)
{
	this->shader = shader;
}

void Mesh::SetColor(float4 color)
{
	meshData.color = color;
}

void Mesh::Release()
{
	vertexBuffer->Release();
	indexBuffer->Release();
	worldDataBuffer.Release();
	meshDataBuffer.Release();
}


UINT Mesh::GetVertexCount() const
{
	return vertexCount;
}

UINT Mesh::GetIndexCount() const
{
	return indexCount;
}