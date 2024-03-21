#pragma once

#include "Games/Game.h"
#include "Mesh.h"

// TODO: PRIMITIVES DON'T NEED VERTEX COLOR

inline Mesh CubePrimitive(const float4& color = { 1.0f, 1.0f, 1.0f, 1.0f })
{
	struct Vertex
	{
		float4 position;
		float4 color;
		float4 uv;
	};
	
	Mesh mesh{};
	mesh.SetColor(color);

	Vertex vertices[] =
	{
		// Front face
		{float4(-1, -1, -1, 0), float4(1, 1, 1, 1), float4(0, 0, 0, 0)},
		{float4(1, -1, -1, 0), float4(1, 1, 1, 1), float4(1, 0, 0, 0)},
		{float4(1, 1, -1, 0), float4(1, 1, 1, 1), float4(1, 1, 0, 0)},
		{float4(-1, 1, -1, 0), float4(1, 1, 1, 1), float4(0, 1, 0, 0)},

		// Back face
		{float4(-1, -1, 1, 0), float4(1, 1, 1, 1), float4(0, 0, 0, 0)},
		{float4(1, -1, 1, 0), float4(1, 1, 1, 1), float4(1, 0, 0, 0)},
		{float4(1, 1, 1, 0), float4(1, 1, 1, 1), float4(1, 1, 0, 0)},
		{float4(-1, 1, 1, 0), float4(1, 1, 1, 1), float4(0, 1, 0, 0)},

		// Left face
		{float4(-1, -1, -1, 0), float4(1, 1, 1, 1), float4(0, 0, 0, 0)},
		{float4(-1, 1, -1, 0), float4(1, 1, 1, 1), float4(1, 0, 0, 0)},
		{float4(-1, 1, 1, 0), float4(1, 1, 1, 1), float4(1, 1, 0, 0)},
		{float4(-1, -1, 1, 0), float4(1, 1, 1, 1), float4(0, 1, 0, 0)},

		// Right face
		{float4(1, -1, -1, 0), float4(1, 1, 1, 1), float4(0, 0, 0, 0)},
		{float4(1, 1, -1, 0), float4(1, 1, 1, 1), float4(1, 0, 0, 0)},
		{float4(1, 1, 1, 0), float4(1, 1, 1, 1), float4(1, 1, 0, 0)},
		{float4(1, -1, 1, 0), float4(1, 1, 1, 1), float4(0, 1, 0, 0)},

		// Top face
		{float4(-1, 1, -1, 0), float4(1, 1, 1, 1), float4(0, 0, 0, 0)},
		{float4(1, 1, -1, 0), float4(1, 1, 1, 1), float4(1, 0, 0, 0)},
		{float4(1, 1, 1, 0), float4(1, 1, 1, 1), float4(1, 1, 0, 0)},
		{float4(-1, 1, 1, 0), float4(1, 1, 1, 1), float4(0, 1, 0, 0)},

		// Bottom face
		{float4(-1, -1, -1, 0), float4(1, 1, 1, 1), float4(0, 0, 0, 0)},
		{float4(1, -1, -1, 0), float4(1, 1, 1, 1), float4(1, 0, 0, 0)},
		{float4(1, -1, 1, 0), float4(1, 1, 1, 1), float4(1, 1, 0, 0)},
		{float4(-1, -1, 1, 0), float4(1, 1, 1, 1), float4(0, 1, 0, 0)},
	};

	// Define index array for the cube
	UINT indices[] =
	{
		// Front face
		0, 2, 1, 0, 3, 2,
		// Back face
		4, 5, 6, 6, 7, 4,
		// Left face
		8, 10, 9, 10, 8, 11,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 18, 17, 16, 19, 18,
		// Bottom face
		20, 21, 22, 22, 23, 20,
	};

	mesh.SetIndices(indices, ARRAYSIZE(indices), DXGI_FORMAT_R32_UINT);
	mesh.SetVertices(vertices, ARRAYSIZE(vertices));

	return std::move(mesh);
}

inline Mesh SpherePrimitive(const float4& color = { 1.0f, 1.0f, 1.0f, 1.0f })
{
	struct Vertex
	{
		float4 position;
		float4 color;
		float4 uv;
	};

	Mesh mesh{};
	mesh.SetColor(color);

	const float thau = 6.28318530718f;

	const int ringsAmount = 6;
	const int resolution = 6;
	const int vertexCount = (ringsAmount + 1) * resolution;
	Vertex vertices[vertexCount];

	for (int j = 0; j <= ringsAmount; j++)
	{
		float height = (float)j / ringsAmount;
		float t = sqrt(height * (1.0f - height));

		for (int i = 0; i < resolution; i++)
		{
			float sin, cos;

			DirectX::XMScalarSinCos(&sin, &cos, (thau * i) / resolution);

			float3 position = float3(cos * t, height - 0.5f, sin * t);

			vertices[j * resolution + i] = Vertex{ float4(position.x, position.y, position.z, 1.0f), float4(1, 1, 1, 1), float4((thau * i) / resolution, height, 0, 0) };
		}
	}

	const int indexCount = ringsAmount * resolution * 6;
	UINT indices[indexCount];

	for (int j = 0; j < ringsAmount; j++)
		for (int i = 0; i < resolution; i++)
		{
			int index = j * resolution + i;

			indices[6 * index    ] = index;
			indices[6 * index + 1] = index + resolution - 1;
			indices[6 * index + 2] = index + resolution;
			indices[6 * index + 3] = index + resolution;
			indices[6 * index + 4] = index + 1;
			indices[6 * index + 5] = index;
		}

	mesh.SetIndices(indices, indexCount, DXGI_FORMAT_R32_UINT);
	mesh.SetVertices(vertices, vertexCount);

	return std::move(mesh);
}