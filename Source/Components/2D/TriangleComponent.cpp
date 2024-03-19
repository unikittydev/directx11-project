#include <d3dcompiler.h>
#include <iostream>

#include "TriangleComponent.h"
#include "../../Graphics/BufferUtils.h"

void TriangleComponent::Init()
{
	CreateVertexBuffer(std::size(points), points, &vertexBuffer);
}

void TriangleComponent::Draw()
{
	shader.PrepareDraw();

	const UINT strides[] = { sizeof(Vertex) };
	const UINT offsets[] = { 0 };

	auto* ctx = Application::GetDeviceContext();

	ctx->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);

	shader.SetActive();

	ctx->Draw(3, 0);
}

void TriangleComponent::Destroy()
{
	shader.Release();
	vertexBuffer->Release();
}