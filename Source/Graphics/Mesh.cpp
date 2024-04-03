#include "Mesh.h"

#include "Shader.h"
#include "Shaders.h"
#include "Application/Application.h"
#include <Games/Game.h>

Mesh::Mesh() : meshDataBuffer(meshData, 1)
{
	shader = Shaders::Get(L"./Shaders/Default.hlsl", Position | VertexColor | UV0, RastState::Solid);
}

void Mesh::Prepare()
{
	meshData.smoothness = 0.5f;
	meshData.specular = float4{ 0.7f, 0.7f, 0.7f, 1.0f };

	auto* ctx = Application::GetDeviceContext();
	meshDataBuffer.SetDataAndBind(ctx, meshData, 0);
	
	const UINT strides[] = { vertexSize };
	const UINT offsets[] = { 0 };
	ctx->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
	ctx->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// If no texture return
	if (mainTexture.GetSRV() == nullptr) return;

	Application::GetDeviceContext()->PSSetShaderResources(0, 1, mainTexture.GetSRV());
	Application::GetDeviceContext()->PSSetSamplers(0, 1, mainTexture.GetSampler());
}

void Mesh::UpdateMeshData(const matrix& localToWorld, const float3& pos)
{
	meshData.ltw = localToWorld;
	meshData.pos = float4{ pos };
}

void Mesh::SetShader(Shader* shader)
{
	this->shader = shader;
}

void Mesh::SetColor(const float4& color)
{
	meshData.color = color;
}

void Mesh::SetSpecular(const float4& specular)
{
	meshData.specular = specular;
}

void Mesh::SetSmoothness(float smoothness)
{
	meshData.smoothness = smoothness;
}

void Mesh::Release()
{
	vertexBuffer->Release();
	indexBuffer->Release();
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


Shader* Mesh::GetShader() const
{
	return shader;
}

const Texture2D& Mesh::GetMainTexture() const
{
	return mainTexture;
}

void Mesh::SetMainTexture(const Texture2D& texture)
{
	mainTexture = texture;
}

matrix Mesh::GetLocalToWorld() const
{
	return meshData.ltw;
}