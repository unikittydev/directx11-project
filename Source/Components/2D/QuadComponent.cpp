#include <d3dcompiler.h>
#include <iostream>

#include "QuadComponent.h"
#include "../../Graphics/BufferUtils.h"

void QuadComponent::Init()
{
	CreateVertexBuffer(std::size(vertices), vertices, &vertexBuffer);
	CreateIndexBuffer(std::size(indices), indices, &indexBuffer);
	CreateDynamicBuffer(1, &location, &constantBuffer);
}

void QuadComponent::Draw()
{
	shader.PrepareDraw();

	const UINT strides[] = { sizeof(Vertex) };
	const UINT offsets[] = { 0 };

	auto* ctx = Application::GetDeviceContext();
	ctx->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
	ctx->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	shader.SetActive();

	ctx->VSSetConstantBuffers(0, 1, &constantBuffer);

	ctx->DrawIndexed(6, 0, 0);
}

void QuadComponent::Update()
{

}

void QuadComponent::Destroy()
{
	shader.Release();
	vertexBuffer->Release();
	indexBuffer->Release();
	constantBuffer->Release();
}