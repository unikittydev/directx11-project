#pragma once

#include <d3d11.h>

#include "Core/Types.h"

#include "Shader.h"
#include "ConstantBuffer.h"

struct Bounds
{
	float3 center;
	float3 extents;

	float3 Min() const
	{
		return center - .5f * extents;
	}

	float3 Max() const
	{
		return center + .5f * extents;
	}

	static float3 Center(const float3& min, const float3& max)
	{
		return (min + max) * .5f;
	}
	
	static float3 Extents(const float3& min, const float3& max)
	{
		return max - min;
	}
};

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

	Bounds bounds;
public:
	Mesh();

	void Release();

	template<typename TIndex>
	void SetIndices(TIndex* indices, UINT count, DXGI_FORMAT indexFormat);

	template<typename TVertex>
	void SetVertices(TVertex* vertices, UINT count);

	void SetShader(Shader* shader);
	
	void SetColor(float4 color);

	void SetBounds(const Bounds& bounds)
	{
		this->bounds = bounds;
	}
	
	void Draw(matrix vp, matrix localToWorld);

	UINT GetVertexCount() const;
	UINT GetIndexCount() const;

	Bounds GetBounds() const
	{
		return bounds;
	}
};

#include "Mesh.hpp"